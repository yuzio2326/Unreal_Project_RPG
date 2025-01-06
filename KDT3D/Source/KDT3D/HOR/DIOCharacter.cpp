// Fill out your copyright notice in the Description page of Project Settings.


#include "HOR/DIOCharacter.h"
#include "Components/NoFallCharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "PaperSprite.h"

// Sets default values
ADIOCharacter::ADIOCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UNoFallCharacterMovementComponent>(Super::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetCollisionProfileName(CollisionProfileName::Player);
	SpringArm = CreateDefaultSubobject<USoftWheelSpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	{
		SpringArm->SetupAttachment(GetMesh());
		SpringArm->ProbeSize = 5.0;
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritRoll = false;
		SpringArm->TargetArmLength = 250;
		//Controller하고 같이 사용중
		SpringArm->SetMinMaxTargetArmLength(300.f, SpringArm->GetMaxTargetArmLength());
	}

	Camera->SetupAttachment(SpringArm);
	bUseControllerRotationYaw = false;

	{	//Camera
		const FRotator Rotation = FRotator(0., 0., 0.);
		const FVector Translation = FVector(61.538462, 0., 120.0);
		FTransform SpringArmTransform = FTransform(Rotation, Translation, FVector::OneVector);
		SpringArm->SetRelativeTransform(SpringArmTransform);
	}
	StatusComponent = CreateDefaultSubobject<UCustomDioStatusComponent>(TEXT("DioStatusComponent"));
	//StatusComponent2 = CreateDefaultSubobject<UCustomDioStatusComponent>(TEXT("StatusComponent"));
	check(StatusComponent);
	SkillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("DioSkillComponent"));
	check(SkillComponent);

	//if (SkillComponent)
	//{
	//	UDataTable* SkillDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Blueprint/01_PersonalProjects/Data/DT_Skill.DT_Skill'"));
	//}
	//SkillComponent->SetData(DataTableRowHandle);//체크용
	//int a = 0;
	//TODO:: 몬스터 ai만들기
	// 보스 ai만들기 
	// 거리가 일정 거리 이상 떨어져 있으면 순보 or 원거리 공격 
	// 일정 거리 이하 공격	완료
	// 공격은 보스 스킬 쿨타임 만들고 쿨타임이 아니면 기본 공격
	// 공격 근접 공격	완료
	// 플레이어 추격하는 느린 투사체 여러발 발사 일정시간 or 바닥에 박히면 사라짐
	


	
}

void ADIOCharacter::OnDie()
{
	//AnimInstance 만들고
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	CurrentDieMontage = CharacterData->DieMontage[1];
	AnimInstance->Montage_Play(CurrentDieMontage);	//use loop
	AnimInstance->Montage_Pause(CurrentDieMontage);
}

void ADIOCharacter::OffDash()
{
	StatusComponent->UseMovingSkill(false);
}


// Called when the game starts or when spawned
void ADIOCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADIOCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetData(DataTableRowHandle);

	//SkillComponent->SetData(DataTableRowHandle);//체크용
}

void ADIOCharacter::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FPawnTableRow* Data = DataTableRowHandle.GetRow<FPawnTableRow>(TEXT("Character"));
	if (!Data) { ensure(false); return; }

	StatusComponent->SetData(DataTableRowHandle);//체크용

	CharacterData = Data;
	{
		UCharacterMovementComponent* Movement = GetCharacterMovement();
		Movement->bOrientRotationToMovement = true;
		Movement->MaxWalkSpeed = CharacterData->MovementMaxSpeed;
		
	

		const float NewCapsuleHalfHeight = CharacterData->CollisionCapsuleHalfHeight * 0.5f;
		Movement->SetCrouchedHalfHeight(NewCapsuleHalfHeight);
	}
	{
		UCapsuleComponent* Capsule = GetCapsuleComponent();
		if (!FMath::IsNearlyEqual(Capsule->GetUnscaledCapsuleHalfHeight(), CharacterData->CollisionCapsuleHalfHeight))
		{
			Capsule->SetCapsuleHalfHeight(CharacterData->CollisionCapsuleHalfHeight, false);
		}
	}
	{
		USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();
		SkeletalMeshComponent->SetSkeletalMesh(CharacterData->SkeletalMesh);
		SkeletalMeshComponent->SetRelativeTransform(CharacterData->MeshTransform);
		SkeletalMeshComponent->SetAnimClass(CharacterData->CustomAnimClass);
	}
}

// Called every frame
void ADIOCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UseSkill(DeltaTime);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (CurrentDieMontage == CharacterData->DieMontage[1])
	{
		AnimInstance->Montage_Play(CurrentDieMontage);
	}
	
}

// Called to bind functionality to input
void ADIOCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ADIOCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (StatusComponent->IsDie()) { return 0.f; }

	float DamageResult = StatusComponent->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (FMath::IsNearlyZero(DamageResult)) { return 0.0; }



	//사망처리
	if (StatusComponent->IsDie() && !CharacterData->DieMontage.IsEmpty())
	{
		GetMovementComponent()->DestroyComponent();
		/*if (Controller)
		{
			Controller->Destroy();
		}*/
		SetActorEnableCollision(false);

		//const int64 Index = FMath::RandRange(0, CharacterData->DieMontage.Num() - 1);
		CurrentDieMontage = CharacterData->DieMontage[0];

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(CurrentDieMontage);
		UKismetSystemLibrary::K2_SetTimer(this, TEXT("OnDie"),
			CharacterData->DieMontage[0]->GetPlayLength() - 0.1f, false);
	}
	else if (!StatusComponent->IsDie() && !CharacterData->BoundMontage.IsEmpty() && DamageResult && StatusComponent->IsBound())
	{
		//맞은 대상 바라보기
		FVector TargetLocation = DamageCauser->GetOwner()->GetActorLocation();
		FVector OwnerLocation = GetMesh()->GetOwner()->GetActorLocation();
		FRotator OwnerRotation = GetMesh()->GetOwner()->GetActorRotation();
		FVector OwnerForwardVector = GetMesh()->GetOwner()->GetActorForwardVector();
		FRotator LookRotator = UKismetMathLibrary::FindLookAtRotation(OwnerLocation, TargetLocation);
		GetMesh()->GetOwner()->SetActorRotation(LookRotator);
	


		const int64 HitReactIndex = FMath::RandRange(0, CharacterData->BoundMontage.Num() - 1);
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(CharacterData->BoundMontage[HitReactIndex]);
		//bad Function
		UKismetSystemLibrary::K2_SetTimer(this, TEXT("OnBound"),
			CharacterData->BoundMontage[HitReactIndex]->GetPlayLength() - 0.1f, false);
		if (StatusComponent->IsBounding(CharacterData->BoundMontage[HitReactIndex]))
		{
			StatusComponent->SetBound(true);
		}
		//StatusComponent->SetBound(false);
	}
	

	return 0.0f;
}



float ADIOCharacter::UseSkill(float DeltaTime)
{
	//Data잘 가져오고 있고 Skill도 만들던거 마저 다 만들고 montage적용이랑 IMC_Input으로 
	//다시 하나 받고 그걸로 skill넣어서 하자
	// Skill에서 status Component로 조건 처리해서 사용하는 MP 있는 MP모두 여기서 처리
	//Moving Skill만 여기서 처리하자 그냥 나머지 montage는 skill에서 ㄱㄱ

	bool bIsBound = StatusComponent->IsBound();

	bool bUseDash = StatusComponent->DashCheck();
	float CurrentSP = StatusComponent->GetSP();
	StatusComponent->AddSP(DeltaTime*2);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	float MontageTime = AnimInstance->Montage_GetPlayRate(CurrentMontage);
	float PlayingMontageTime = AnimInstance->Montage_GetPosition(CurrentMontage);

	//stun bound 종류별로 넣고 아닐때 else로 ㄱㄱ
	if (bIsBound)
	{
		OnStun();
		if (PlayingMontageTime <= 0.5f) { StatusComponent->SetBound(false); }

	}
	else
	{
		if (bUseDash)
		{
			if (!Dashed && CurrentSP >= 20)
			{
				ActivatingTime = UseMovingSkill() - 0.5f;
				Dashed = true;
			}
			else
			{
				ActivatingTime -= DeltaTime;
				if (ActivatingTime <= 0)
				{
					StatusComponent->UseMovingSkill(false);
					Dashed = false;
				}
			}

		}

		bool bATK = StatusComponent->GetAttacking();
		Attacked = StatusComponent->GetSPAttacking();
		//AttackingTime -= DeltaTime;
		AttackingTime = MontageTime;
		if (bATK)
		{
			Attack();
		}
		if (Attacked)
		{
			SPAttack();
		}
	}
	

	//AttackingTime -= DeltaTime;

	


	return 0.0f;
}

void ADIOCharacter::Attack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	float PlayingMontageTime = AnimInstance->Montage_GetPosition(CurrentMontage);
	bool bPlayMontage = AnimInstance->Montage_IsPlaying(CurrentMontage);
	bool bCheckBound = StatusComponent->IsBound();
	bool bCheckUsingSkill = SkillComponent->CheckUsingSkill();

	if (AttackingTime <= 0.5f && !bCheckUsingSkill && !bCheckBound)
	{		
		//Charge 돌입시 밑의 코드 활성화 시키고 Index로 play하게 ㄱㄱ 
		// PlayingMontageTime || bPlayMontage  로 조절
		//const int64 Index = FMath::RandRange(2, CharacterData->AttackMontage.Num() - 1);
		
		//Charge상태가 아닐때 라는 조건 추가하고 Charge면 2 3 play하게 ㄱㄱ
		if (StatusComponent->GetAttacking() && !CharacterData->AttackMontage.IsEmpty())
		{
			if (AnimNum >= 2)
				AnimNum = 0;
			CurrentMontage = CharacterData->AttackMontage[AnimNum];
			StatusComponent->SetChangeDir(true);
			//AttackingTime = CharacterData->AttackMontage[AnimNum]->GetPlayLength();
			AnimInstance->Montage_Play(CurrentMontage);
			StatusComponent->AddMP(1);
			++AnimNum;
		}
	}


}

void ADIOCharacter::SPAttack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	float PlayingMontageTime = AnimInstance->Montage_GetPosition(CurrentMontage);
	bool bPlayMontage = AnimInstance->Montage_IsPlaying(CurrentMontage);
	bool bCheckUsingSkill = SkillComponent->CheckUsingSkill();
	bool bCheckBound = StatusComponent->IsBound();
	if (StatusComponent->GetMP() < 2) { return; }

	if (AttackingTime <= 0.5f && !bCheckUsingSkill && !bCheckBound)
	{
		const int64 Index = FMath::RandRange(0, CharacterData->DieMontage.Num() - 1);
		if (StatusComponent->GetSPAttacking() && !CharacterData->SkillMontage.IsEmpty())
		{
			CurrentMontage = CharacterData->SkillMontage[Index];
			StatusComponent->SetChangeDir(true);
			//AttackingTime = CharacterData->SkillMontage[Index]->GetPlayLength();
			AnimInstance->Montage_Play(CurrentMontage);
			StatusComponent->AddMP(-2);
		}
	}
}

void ADIOCharacter::OnStun()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	float MontageTime = AnimInstance->Montage_GetPlayRate(CurrentMontage);
	if (StatusComponent->IsBound() && !CharacterData->BoundMontage.IsEmpty())
	{
		CurrentMontage = CharacterData->BoundMontage[0];
		UAnimMontage* PlayingMontage =AnimInstance->GetCurrentActiveMontage();
		if (PlayingMontage == CurrentMontage && MontageTime > 0)
		{
			return;
		}
		AnimInstance->Montage_Play(CurrentMontage);

	}

}

void ADIOCharacter::OnBound()
{
	StatusComponent->SetBound(false);
}

void ADIOCharacter::Use_Sk_01()
{
	//if (SkillComponent->CanUseSk01());
	int a = 0;
}

float ADIOCharacter::UseMovingSkill()
{
	if (StatusComponent->IsDie()) { return 0.f; }
	
	//cc중 사용 불가	status에서 사용 불가 처리는 하긴 했지만 그래도 넣음
	if (StatusComponent->IsDie()|| StatusComponent->IsBound()) { return 0.f; }

	if (StatusComponent->DashCheck()&& !CharacterData->DashMontage.IsEmpty())
	{
		//const int64 MovingSkillIndex = FMath::RandRange(0, CharacterData->DashMontage.Num() - 1);
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(CharacterData->DashMontage[0]);
		CharacterData->DashMontage[0]->GetPlayLength();
		StatusComponent->AddSP(-20);
		return CharacterData->DashMontage[0]->GetPlayLength();
	}

	return 0.0f;
}


