// Fill out your copyright notice in the Description page of Project Settings.

#include "HOR/CustomDioStatusComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Data/PawnData.h"

// Sets default values for this component's properties
UCustomDioStatusComponent::UCustomDioStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}



void UCustomDioStatusComponent::AddHP(float HPValue)
{
	HP += HPValue; 
	OnPlayerHPChanged.Broadcast(HP, MaxHP);
}

void UCustomDioStatusComponent::AddMP(float MPValue)
{
	MP += MPValue;
	OnPlayerMPChanged.Broadcast(MP, MaxMP);
}

void UCustomDioStatusComponent::AddSP(float SPValue)
{
	SP += SPValue;
	OnPlayerSPChanged.Broadcast(SP, MaxSP);
}

void UCustomDioStatusComponent::AddDMG(float DMGValue)
{
	DMG += DMGValue;
	OnPlayerDMGChanged.Broadcast(DMG);
}

bool UCustomDioStatusComponent::IsBounding(UAnimMontage* MontageToPlay)
{
	USkeletalMeshComponent* OwnerMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	UAnimInstance* AnimInstance = OwnerMesh->GetAnimInstance();
	bool IsBounding = AnimInstance->Montage_IsActive(MontageToPlay);

	return IsBounding;
}

// Called when the game starts
void UCustomDioStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	//For UI
	OnPlayerDMGChanged.Broadcast(DMG);
	OnPlayerHPChanged.Broadcast(HP, MaxHP);
	OnPlayerMPChanged.Broadcast(MP, MaxMP);
	OnPlayerSPChanged.Broadcast(SP, MaxSP);
	// ...
	
}

void UCustomDioStatusComponent::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;

	FPawnTableRow* CharacterData = DataTableRowHandle.GetRow<FPawnTableRow>(TEXT("Character"));

	//DMG, HP 세팅
	if (CharacterData->DMG) { DMG = CharacterData->DMG; }
	if (CharacterData->HP) { MaxHP = CharacterData->HP; }
	HP = MaxHP;
	//if(InDataTableRowHandle)

}


// Called every frame
void UCustomDioStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//AddSP(DeltaTime*2);	//tick당 sp회복 10초에 1번 대쉬 가능한 정도 Player에서 처리 ㄱㄱ

	AddMaxStatus();

	// ...
}

float UCustomDioStatusComponent::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bDead) { return 0; }
	if (FMath::IsNearlyZero(Damage)) { return 0; }
	float NewDamage = Damage;

	NewDamage = FMath::Clamp(NewDamage, 0.f, NewDamage);

	HP -= NewDamage;
	HP = FMath::Clamp(HP, 0.f, HP);

	LastInstigator = EventInstigator;
	OnPlayerHPChanged.Broadcast(HP, MaxHP);

	//bStun = true;
	BoundValue += NewDamage;
	float MaxBound = MaxHP * 0.25f;
	if (BoundValue >= MaxBound)
	{
		bBound = true;
		BoundValue = 0;
	}

	if (HP <= 0.f)
	{
		bDead = true;
		OnDead.Broadcast(true);
	}

	return NewDamage;
}

//void UCustomDioStatusComponent::UseSkill(int SkillNum, bool bUseSkill)
//{
//
//}

bool UCustomDioStatusComponent::UseMovingSkill(bool UseDash)
{
	if (bDead) { return bDashing; }
	if (bStun|| bBound) { return bDashing; }	//cc면 사용 불가



	return bDashing = UseDash;;
}

void UCustomDioStatusComponent::MovingSkill()
{
}

void UCustomDioStatusComponent::AddMaxStatus()
{
	if (HP > MaxHP) 
	{ 
		HP = MaxHP; 
		OnPlayerHPChanged.Broadcast(HP, MaxHP);
	}
	if (MP > MaxMP) 
	{ 
		MP = MaxMP; 
		OnPlayerMPChanged.Broadcast(MP, MaxMP);
	}
	if (SP > MaxSP) 
	{ 
		SP = MaxSP; 
		OnPlayerSPChanged.Broadcast(SP, MaxSP);
	}
}

void UCustomDioStatusComponent::Revival()
{
	if (bDead)
	{
		bDead = false;
		HP = MaxHP;
		MP = MaxMP;
		SP = MaxSP;
		OnDead.Broadcast(false);
	}

}



