// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemy/Enemy.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/UserWidgetBase.h"
#include "Actors/AI/BasicEnemyAIController.h"
#include "PaperSprite.h"

// Sets default values
AEnemy::AEnemy(const FObjectInitializer& ObjectInitializer)
	//: Super(ObjectInitializer.SetDefaultSubobjectClass<USphereComponent>(TEXT("CollisionComponent")))
	: Super(ObjectInitializer)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));

	SkeletalMeshComponent->SetupAttachment(DefaultSceneRoot);
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// HP Bar
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

	// Weapon
	{
		Weapon = CreateDefaultSubobject<UWeaponChildActorComponent>(TEXT("Weapon"));
		Weapon->SetupAttachment(SkeletalMeshComponent, SocketName::Weapon);
	}

	MovementComponent = CreateDefaultSubobject<UAdvanceFloatingPawnMovement>(TEXT("MovementComponent"));
	StatusComponent = CreateDefaultSubobject<UStatusComponent>(TEXT("StatusComponent"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISenseConfig_Sight"));
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = true;
	AISenseConfig_Sight->SightRadius = 800.f;
	AISenseConfig_Sight->LoseSightRadius = 1000.f;
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 120.f;
	AIPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
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

	// Minimap
	{
		MinimapSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MinimapSpriteComponent"));
		MinimapSpriteComponent->SetupAttachment(RootComponent);
		MinimapSpriteComponent->SetVisibleInSceneCaptureOnly(true);
		{
			FTransform Transform;
			Transform.SetLocation(FVector(0.0, 0.0, 300.0));
			Transform.SetRotation(FRotator(0.0, 90.0, -90.0).Quaternion());
			MinimapSpriteComponent->SetRelativeTransform(Transform);
		}
		{
			static ConstructorHelpers::FObjectFinder<UPaperSprite> Asset(TEXT("/Script/Paper2D.PaperSprite'/Game/Blueprint/Minimap/Enemy_Sprite.Enemy_Sprite'"));
			ensure(Asset.Object);
			MinimapSpriteComponent->SetSprite(Asset.Object);
		}
		{
			static ConstructorHelpers::FObjectFinder<UMaterial> Asset(TEXT("/Script/Engine.Material'/Game/Blueprint/Minimap/MyDefaultSpriteMaterial.MyDefaultSpriteMaterial'"));
			ensure(Asset.Object);
			MinimapSpriteComponent->SetMaterial(0, Asset.Object);
		}
	}
}

void AEnemy::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FPawnTableRow* Data = DataTableRowHandle.GetRow<FPawnTableRow>(TEXT("Enemy"));
	if (!Data) { ensure(false); return; }

	EnemyData = Data;

	AIControllerClass = EnemyData->AIControllerClass;

	MovementComponent->MaxSpeed = EnemyData->MovementMaxSpeed;

	if (!IsValid(CollisionComponent) || CollisionComponent->GetClass() != EnemyData->CollisionClass)
	{
		EObjectFlags SubobjectFlags = GetMaskedFlags(RF_PropagateToSubObjects) | RF_DefaultSubObject;
		CollisionComponent = NewObject<UShapeComponent>(this, EnemyData->CollisionClass, TEXT("CollisionComponent"), SubobjectFlags);
		CollisionComponent->RegisterComponent();
		CollisionComponent->SetCollisionProfileName(CollisionProfileName::Enemy);
		CollisionComponent->SetCanEverAffectNavigation(false);
		SetRootComponent(CollisionComponent);
		DefaultSceneRoot->SetRelativeTransform(FTransform::Identity);
		DefaultSceneRoot->AttachToComponent(CollisionComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}

	if (USphereComponent* SphereComponent = Cast<USphereComponent>(CollisionComponent))
	{
		SphereComponent->SetSphereRadius(EnemyData->CollisionSphereRadius);
	}
	else if(UBoxComponent* BoxComponent = Cast<UBoxComponent>(CollisionComponent))
	{
		BoxComponent->SetBoxExtent(EnemyData->CollisionBoxExtent);
	}
	else if(UCapsuleComponent* CapsuleComponent = Cast<UCapsuleComponent>(CollisionComponent))
	{
		CapsuleComponent->SetCapsuleSize(EnemyData->CollisionCapsuleRadius, EnemyData->CollisionCapsuleHalfHeight);
	}

	SkeletalMeshComponent->SetSkeletalMesh(EnemyData->SkeletalMesh);
	SkeletalMeshComponent->SetAnimClass(EnemyData->AnimClass);
	SkeletalMeshComponent->SetRelativeTransform(EnemyData->MeshTransform);

	Weapon->SetData(EnemyData->Weapon);

	if (EnemyData->HP != 0 && EnemyData->DMG != 0)
	{
		StatusComponent->StatusSetting(EnemyData->HP, EnemyData->DMG);
		EnemyDMG = EnemyData->DMG;
		EnemyHP = EnemyData->HP;
	}

}

void AEnemy::PostDuplicate(EDuplicateMode::Type DuplicateMode)
{
	Super::PostDuplicate(DuplicateMode);

	if (DuplicateMode == EDuplicateMode::Normal)
	{
		FTransform Backup = GetActorTransform();
		CollisionComponent->DestroyComponent();
		SetData(DataTableRowHandle);
		SetActorTransform(Backup);
	}
}

void AEnemy::PostLoad()
{
	Super::PostLoad();
}

void AEnemy::PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph)
{
	Super::PostLoadSubobjects(OuterInstanceGraph);
}

void AEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (PatrolPathRef)
	{
		if (ABasicEnemyAIController* BasicEnemyAIController = Cast<ABasicEnemyAIController>(Controller))
		{
			BasicEnemyAIController->SetPatrolPath(PatrolPathRef->GetPath());
		}
	}
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	SetData(DataTableRowHandle);

	UUserWidgetBase* UserWidget = Cast<UUserWidgetBase>(HPWidgetComponent->GetWidget());
	check(UserWidget);
	UserWidget->SetOwningPawn(this);
}

void AEnemy::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetData(DataTableRowHandle);
	SetActorTransform(Transform);
}

float AEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (StatusComponent->IsDie()) { return 0.f; }

	float DamageResult = StatusComponent->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (FMath::IsNearlyZero(DamageResult)) { return 0.0; }

	if (Controller)
	{
		Controller->StopMovement();
	}

	if (StatusComponent->IsDie() && !EnemyData->DieMontage.IsEmpty())
	{
		if (Controller)
		{
			Controller->Destroy();
		}
		SetActorEnableCollision(false);

		const int64 Index = FMath::RandRange(0, EnemyData->DieMontage.Num() - 1);
		CurrentDieMontage = EnemyData->DieMontage[Index];

		UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
		AnimInstance->Montage_Play(CurrentDieMontage);
		UKismetSystemLibrary::K2_SetTimer(this, TEXT("OnDie"), 
			EnemyData->DieMontage[Index]->GetPlayLength() - 0.5f, false);
	}
	else if (!StatusComponent->IsDie() && !EnemyData->HitReactMontage.IsEmpty())
	{
		const int64 HitReactIndex = FMath::RandRange(0, EnemyData->HitReactMontage.Num() - 1);
		UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
		AnimInstance->Montage_Play(EnemyData->HitReactMontage[HitReactIndex]);
	}

	return 0.0f;
}

void AEnemy::OnPaperBurnEffect(float InDissolve)
{
	const int32 MaterialNum = MaterialInstanceDynamics.Num();
	for (int32 i = 0; i < MaterialNum; ++i)
	{
		MaterialInstanceDynamics[i]->SetScalarParameterValue(MF_PostEffect::PaperBurnDissolve, InDissolve);
	}
}

void AEnemy::OnPaperBurnEffectEnd()
{
	Destroy();
}

void AEnemy::OnDie()
{
	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	AnimInstance->Montage_Pause(CurrentDieMontage);

	const int32 MaterialNum = SkeletalMeshComponent->GetSkinnedAsset()->GetMaterials().Num();
	MaterialInstanceDynamics.Reserve(MaterialNum);
	for (int32 i = 0; i < MaterialNum; ++i)
	{
		MaterialInstanceDynamics.Add(SkeletalMeshComponent->CreateDynamicMaterialInstance(i));
	}

	PaperBurnEffectTimelineComponent->Play();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AEnemy::CheckPower()
{
	return EnemyDMG;
}
