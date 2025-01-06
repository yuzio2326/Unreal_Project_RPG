// Fill out your copyright notice in the Description page of Project Settings.

#include "HOR/Monster/BossMonster.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/UserWidgetBase.h"
#include "HOR/Monster/MonsterAIController.h"	//Changed
//#include "PaperSprite.h"
#include "Kismet/KismetMathLibrary.h"
#include "HOR/ItemPack.h"	//For Spawn Drop item

//AI Controller도 바꿀까 생각중


ABossMonster::ABossMonster(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	//AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));

	SkeletalMeshComponent->SetupAttachment(DefaultSceneRoot);
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//HUD만들고 ㄱㄱ
	{
		ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/UI_HPBar.UI_HPBar_C'"));
		HPWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWidgetComponent"));
		HPWidgetComponent->SetupAttachment(DefaultSceneRoot);
		HPWidgetComponent->SetRelativeLocation(FVector(0., 0., 130.0));
		HPWidgetComponent->SetDrawSize(FVector2D(256.3, 17.0));
		HPWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		HPWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		HPWidgetComponent->SetWidgetClass(WidgetClass.Class);
	}
	//Paper Burn 효과	-> data로 받아서 해야 할듯
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveAsset(TEXT("/Script/Engine.CurveFloat'/Game/Blueprint/Effect/CV_PaperBurn.CV_PaperBurn'"));
		check(CurveAsset.Object);
		PaperBurnEffectTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("PaperBurnEffectTimelineComponent"));

		FOnTimelineFloat Delegate;
		Delegate.BindDynamic(this, &ThisClass::OnPaperBurnEffect);
		PaperBurnEffectTimelineComponent->AddInterpFloat(CurveAsset.Object, Delegate);

		FOnTimelineEvent EndDelegate;
		EndDelegate.BindDynamic(this, &ThisClass::OnPaperBurnEffectEnd);
		PaperBurnEffectTimelineComponent->SetTimelineFinishedFunc(EndDelegate);
	}


	MovementComponent = CreateDefaultSubobject<UAdvanceFloatingPawnMovement>(TEXT("MovementComponent"));
	check(MovementComponent);
	StatusComponent = CreateDefaultSubobject<UStatusComponent>(TEXT("StatusComponent"));
	check(StatusComponent);
	MonsterSkillComponent = CreateDefaultSubobject<UMonsterSkillComponent>(TEXT("MonsterSkillComponent"));
	check(MonsterSkillComponent);


	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISenseConfig_Sight"));
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = true;
	AISenseConfig_Sight->SightRadius = 500.f;
	AISenseConfig_Sight->LoseSightRadius =700.f;
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 120.f;
	AIPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);


	/*
	* Monsterskill Use 완성하고 Attack 관련 버그 해결 하기
	* 코드상으로는 ㄱㅊ은데 Black Board 쪽인거 같음 
	* 	if (!MonsterData->BossVision)
	{
	}
	else
	{
		AISenseConfig_Sight->SightRadius = 1000.f;
		AISenseConfig_Sight->LoseSightRadius = 3000.f;
		AISenseConfig_Sight->PeripheralVisionAngleDegrees = 360.f;
	}
	*/
}

void ABossMonster::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	MonsterDataTableRowHandle = InDataTableRowHandle;
	if (MonsterDataTableRowHandle.IsNull()) { return; }
	FBossMonsterTableRow* Data = MonsterDataTableRowHandle.GetRow<FBossMonsterTableRow>(TEXT("Enemy"));
	if (!Data) { ensure(false); return; }
	MonsterData = Data;

	AIControllerClass = MonsterData->AIControllerClass;



	MovementComponent->MaxSpeed = MonsterData->MovementMaxSpeed;

	if (!IsValid(CollisionComponent) || CollisionComponent->GetClass() != MonsterData->CollisionClass)
	{
		EObjectFlags SubobjectFlags = GetMaskedFlags(RF_PropagateToSubObjects) | RF_DefaultSubObject;
		CollisionComponent = NewObject<UShapeComponent>(this, MonsterData->CollisionClass, TEXT("CollisionComponent"), SubobjectFlags);
		CollisionComponent->RegisterComponent();
		CollisionComponent->SetCollisionProfileName(CollisionProfileName::Enemy);
		CollisionComponent->SetCanEverAffectNavigation(false);
		SetRootComponent(CollisionComponent);
		DefaultSceneRoot->SetRelativeTransform(FTransform::Identity);
		DefaultSceneRoot->AttachToComponent(CollisionComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}

	if (USphereComponent* SphereComponent = Cast<USphereComponent>(CollisionComponent))
	{
		SphereComponent->SetSphereRadius(MonsterData->CollisionSphereRadius);
	}
	else if (UBoxComponent* BoxComponent = Cast<UBoxComponent>(CollisionComponent))
	{
		BoxComponent->SetBoxExtent(MonsterData->CollisionBoxExtent);
	}
	else if (UCapsuleComponent* CapsuleComponent = Cast<UCapsuleComponent>(CollisionComponent))
	{
		CapsuleComponent->SetCapsuleSize(MonsterData->CollisionCapsuleRadius, MonsterData->CollisionCapsuleHalfHeight);
	}

	SkeletalMeshComponent->SetSkeletalMesh(MonsterData->SkeletalMesh);
	SkeletalMeshComponent->SetAnimClass(MonsterData->AnimClass);
	SkeletalMeshComponent->SetRelativeTransform(MonsterData->MeshTransform);

	if (MonsterData->HP != 0 && MonsterData->DMG != 0)
	{
		StatusComponent->StatusSetting(MonsterData->HP, MonsterData->DMG);
		MonsterDMG = MonsterData->DMG;
		MonsterHP = MonsterData->HP;
	}

	AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	//data안에 있는 각 스킬의 데미지를 뽑아서 statuscomponenet에 전달하게함
	//status 안에 있는 데이터 뽑아서 DMG Animnotify에 넣어줄테니까
	//Status안에 박아야됌. -> skillcomponent 만들면 시간 좀 걸리니 일단 여기에 박고
	// 나중에 시간 남으면 skillcomponentㄱㄱ
	//int64 SkillNum = MonsterData->SkillDataRow.Num();
	//TArray<FSkillDataRow> SkillData = MonsterData->SkillDataRow;
	//for (int i = 0; i = SkillNum; i++)
	//{
	//	StatusComponent->SetSkillDMG(i, SkillData[i].Damage);
	//	StatusComponent->SetMonsterSkillCooltime(i, SkillData[i].SkillCoolTime);
	//}

	// 그런데 이걸 뽑아서 사용을 할때 이떻게 줘야하지?
	// Animnotify로 하고있어서 얘가 호출될때 지금 사용중인 스킬이 뭔지 알려줘야 하지 않나? 
	// 일단 데이터 세팅은 해 놨고 그러면 스킬쿨은? status가 아닌 skill data row에 
	// 데미지랑 쿨타임이 잡혀있는데?
	// 이미 스킬 Array는 자기 쿨 빼와서 결국 component에 박아 놨고
	// monster도 그러면 skillcomponent를 사용해야하고 빼와서 만들고
	// 거기에 맞게 다시 갈아 엎어야 하는데?
	// player는 anim index를 찾아서 dmg설정 한거에 맞게 적용하는건데?
	// 그러면 monster도 anim index찾아서 여기 안에 있는 dmg를 박아서 사용하게 하면 되지 않을까?
	// 사용하는 스킬은 skillbase에 박아 놨으니 skillcomponent를..
	// player에 박은거 그대로 사용하고 저걸로 바꾸기만 하면 되지 않을까?
	// skill anim index가져오고 skilldmg arrray에 박아 놓는거지
	// 해보니까 skillcomponent밖에 BP로 만든것들 때문에 안맞음
	// 사용하게 할거면 skillcomponent를 갈아 엎어야됌.
	// 일단 여기에 데이터 세팅을 다 해놨어도 사용이 안되는게 문제임.
	// 그냥 밖으로 빼서 사용 가능하게 세팅해 놓는게 좋을듯함.
	// 그냥 skillcomponent기능을 빼서 status에 박아가지고 사용하게 만들자
	// player status랑 다르니까 일단 박아서 넣고 없으면 활성화 안시키면 되잖아
	// 
	// 결론 여기다가 박아서 만들자 객체 자신이 쿨타임 관리를 하겠지만 skillcomponent만들
	// 시간이 있다면 만들고 데이터 넣고 관리를 하겠지만 시간이 없음 ㅈ같음
	//


}

void ABossMonster::SetSkillData(const FDataTableRowHandle& InSkillDataTableRowHandle)
{
	
	MonsterData->MonsterSkillDataTableRowHandle = InSkillDataTableRowHandle;

	MonsterData->MonsterSkillDataTableRowHandle = InSkillDataTableRowHandle;
	if (MonsterData->MonsterSkillDataTableRowHandle.IsNull()) { return; }
	FSkillTableRow* Data = MonsterData->MonsterSkillDataTableRowHandle.GetRow<FSkillTableRow>(TEXT("MonsterSkillData"));
	if (!Data) { ensure(false); return; }
	MonsterSkillData = Data;

	MonsterSkillComponent->SetData(InSkillDataTableRowHandle);
	

	int a = 0;

}

void ABossMonster::PostDuplicate(EDuplicateMode::Type DuplicateMode)
{
	Super::PostDuplicate(DuplicateMode);
	if (DuplicateMode == EDuplicateMode::Normal)
	{
		FTransform Backup = GetActorTransform();
		CollisionComponent->DestroyComponent();
		SetData(MonsterDataTableRowHandle);
		SetActorTransform(Backup);
	}
}

void ABossMonster::PostLoad()
{
	Super::PostLoad();
}

void ABossMonster::PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph)
{
	Super::PostLoadSubobjects(OuterInstanceGraph);
}

void ABossMonster::OnDie()
{
	//Paper Burn 효과 만들어야됌
	AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	AnimInstance->Montage_Pause(CurrentDieMontage);

	
	const int32 MaterialNum = SkeletalMeshComponent->GetSkinnedAsset()->GetMaterials().Num();
	MaterialInstanceDynamics.Reserve(MaterialNum);
	for (int32 i = 0; i < MaterialNum; ++i)
	{
		MaterialInstanceDynamics.Add(SkeletalMeshComponent->CreateDynamicMaterialInstance(i));
	}


	if (PaperBurnEffectTimelineComponent)
	{
		PaperBurnEffectTimelineComponent->Play();
	}
	else
	{

		//Spawnactor
		Destroy();
	}
	
}

void ABossMonster::OnStun()
{
	StatusComponent->SetStun(false);
}

void ABossMonster::OnAttack()
{
	//UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	int a = 0;

}

void ABossMonster::OnSpawnBullet()
{

}

void ABossMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//!BasicAnimInstance->Montage_IsPlaying(nullptr)
	

	//UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	//AnimInstance->Montage_Play(CurrentDieMontage);

	if (!MonsterData->MonsterSkillDataTableRowHandle.IsNull())
	{
		IntervalSkillTime -= DeltaTime;
	}
	if (ChangeYawTime > 0) { ChangeYawTime -= DeltaTime; }
	else if (ChangeYawTime<=0)
	{
		bUseControllerRotationYaw = false;
		//if(!Controller.IsNull())
		//	Controller->SetControlRotation(DefaultRotator);
	}

	//animinsta
	// 모든 몽타주가 재생 중이지 않을 때 + 스킬 사용 내부쿨이 0보다 적을때 스킬 사용
	//if (IntervalSkillTime <= 0&& !AnimInstance->Montage_IsPlaying(nullptr))
	//{
		//UseSkill();
	//}

}

float ABossMonster::CheckPower()
{
	return MonsterDMG;
}

void ABossMonster::UseSkill()
{

	//int64 SkillNum = MonsterData->SkillDataRow.Num();
	int32 SkillNum = MonsterSkillData->SkillArray.Num();
	if(SkillNum<=0)
		UE_LOG(LogTemp, Warning, TEXT("SkillNum is 0"));

	TArray<FSkillDataRow> SkillData = MonsterSkillData->SkillArray;
	/*Use Skill 기능 구현*/
	//MonsterSkillComponent에서 스킬쿨이랑 비교하고 쿨타임이 안되어 있으면 
	// 다시 randomrage를 돌리기 MonsterSkillComponent는 
	// 애니메이션 차례대로 재생하고 기능구현이 필요한 스킬의 
	// 이름 받아서 TP같은거 해당 이름일 경우에 구현한 기능을 실행하도록 ㄱㄱ
	//MonsterSkillComponent


	int32 UseRandomSkill = FMath::RandRange(0, SkillNum);
	



	IntervalSkillTime = FMath::RandRange(8,16);
	
}

void ABossMonster::AddBuff(float BuffValue)
{
	StatusComponent->AddHP(BuffValue);
}

void ABossMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (PatrolPathRef)
	{
		if (AMonsterAIController* EnemyAIController = Cast<AMonsterAIController>(Controller))
		{
			EnemyAIController->SetPatrolPath(PatrolPathRef->GetPath());
			if (MonsterData)
			{
				EnemyAIController->BossAgro(MonsterData->BossVision);

			}
		}
	}
	

}

void ABossMonster::BeginPlay()
{
	Super::BeginPlay();

	SetData(MonsterDataTableRowHandle);
	SetSkillData(MonsterData->MonsterSkillDataTableRowHandle);
	//widget 만드는중
	 
	UUserWidgetBase* UserWidget = Cast<UUserWidgetBase>(HPWidgetComponent->GetWidget());
	check(UserWidget);
	UserWidget->SetOwningPawn(this);
}

void ABossMonster::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetData(MonsterDataTableRowHandle);
	SetActorTransform(Transform);
}

float ABossMonster::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (StatusComponent->IsDie()) { return 0.f; }

	float DamageResult = StatusComponent->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (FMath::IsNearlyZero(DamageResult)) { return 0.0; }

	//TODO:: Stun 가지고 오고 stun bound Max HP에 따라서 설정하기
	//StatusComponent->IsStun();
	//AMonsterAIController* EnemyAIController;
	//bool bOriginalUseControllerRotationYaw = EnemyAIController->bUseControllerRotationYaw;
	//bool bOriginalOrientRotationToMovement = EnemyAIController->GetCharacterMovement()->bOrientRotationToMovement;
	//bool checkUseControllerRotationYaw = bUseControllerRotationYaw;
	//DefaultRotator = Controller->GetControlRotation();
	//맞은 대상 바라보기
	/*if (!checkUseControllerRotationYaw)
	{
		bUseControllerRotationYaw = true;
		ChangeYawTime = 0.3f;
		APawn* MonsterController = Controller->GetPawn();
		FVector TargetLocation = DamageCauser->GetActorLocation();
		FVector SelfLocation = MonsterController->GetActorLocation();
		FVector LookTargetDirection = (TargetLocation - SelfLocation).GetSafeNormal();
		FRotator LookTargetRotation = UKismetMathLibrary::FindLookAtRotation(SelfLocation, TargetLocation);
		Controller->SetControlRotation(LookTargetRotation);
	}*/


	if (Controller)
	{
		Controller->StopMovement();
	}

	if (StatusComponent->IsDie() && !MonsterData->DieMontage.IsEmpty())
	{
		if (Controller)
		{
			Controller->Destroy();
		}
		SetActorEnableCollision(false);

		CurrentDieMontage = MonsterData->DieMontage[0];

		//UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
		AnimInstance->Montage_Play(CurrentDieMontage);
		UKismetSystemLibrary::K2_SetTimer(this, TEXT("OnDie"),
			MonsterData->DieMontage[0]->GetPlayLength() - 0.1f, false);
	}
	else if ((!StatusComponent->IsDie() && !MonsterData->HitReactMontage.IsEmpty()) && DamageResult&& StatusComponent->GetStun())
	{
		const int64 HitReactIndex = FMath::RandRange(0, MonsterData->HitReactMontage.Num() - 1);
		AnimInstance->Montage_Play(MonsterData->HitReactMontage[HitReactIndex]);
		UKismetSystemLibrary::K2_SetTimer(this, TEXT("OnStun"),
			MonsterData->HitReactMontage[HitReactIndex]->GetPlayLength() - 0.1f, false);
		StatusComponent->SetStun(false);
	}


	return 0.0f;
}

void ABossMonster::OnPaperBurnEffect(float InDissolve)
{
	const int32 MaterialNum = MaterialInstanceDynamics.Num();
	for (int32 i = 0; i < MaterialNum; ++i)
	{
		float CheckValue = 0;
		if (MaterialInstanceDynamics[i]->GetScalarParameterValue(TEXT("FadeMask"), CheckValue)&&
			MaterialInstanceDynamics[i]->GetScalarParameterValue(TEXT("FadeOut"), CheckValue))
		{
			MaterialInstanceDynamics[i]->SetScalarParameterValue(TEXT("FadeMask"),true);
			MaterialInstanceDynamics[i]->SetScalarParameterValue(TEXT("FadeOut"), 1-InDissolve);
		}
		else if (MaterialInstanceDynamics[i]->GetScalarParameterValue(MF_PostEffect::PaperBurnDissolve, CheckValue))
		{
			MaterialInstanceDynamics[i]->SetScalarParameterValue(MF_PostEffect::PaperBurnDissolve, InDissolve);
		}
		else 
		{
			//FadeOut Effects
			UMaterialInterface* Material = SkeletalMeshComponent->GetMaterial(i);
			UMaterialInstanceDynamic* DynMaterial = Cast<UMaterialInstanceDynamic>(Material);
			if (!DynMaterial)
			{
				UE_LOG(LogTemp, Warning, TEXT("Material is not a dynamic material instance!"));
				//return;
			}
			else 
			{
				if (DynMaterial->GetScalarParameterValue(TEXT("FadeOut"), CheckValue))
				{
					DynMaterial->SetScalarParameterValue(TEXT("FadeOut"), 1 - InDissolve);
				}

			}
		}
	}
}

void ABossMonster::OnPaperBurnEffectEnd()
{

	//Spawn item
	if (!MonsterData->DropItem.IsNull())
	{
		const FItemPackTableRow* DropItemTableRow = MonsterData->DropItem.GetRow<FItemPackTableRow>(TEXT("DropItem"));
		check(DropItemTableRow);

		APawn* OwningPawn = Cast<APawn>(SkeletalMeshComponent->GetOwner());
		/* 주석 풀고 data 넣기*/
		UWorld* World = OwningPawn->GetWorld();
		AItemPack* DropItem = World->SpawnActorDeferred<AItemPack>(DropItemTableRow->ItemClass,
			FTransform::Identity, OwningPawn, OwningPawn, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		DropItem->SetData(MonsterData->DropItem);
		FVector OwnerLocation = SkeletalMeshComponent->GetOwner()->GetActorLocation();
		FRotator OwnerRotation = SkeletalMeshComponent->GetOwner()->GetActorRotation();
		FTransform OwnerTransform;
		OwnerTransform.SetLocation(OwnerLocation);
		OwnerTransform.SetRotation(OwnerRotation.Quaternion());
		DropItem->FinishSpawning(OwnerTransform);
	}
	Destroy();
}
