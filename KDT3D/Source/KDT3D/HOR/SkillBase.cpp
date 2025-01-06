// Fill out your copyright notice in the Description page of Project Settings.


#include "HOR/SkillBase.h"



FSkillDataRow::FSkillDataRow()
	: SkillBaseClass(ASkillBase::StaticClass())
{
}

// Sets default values
ASkillBase::ASkillBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	//bCanUseSkill = true;
	//Cooldown = 3.0f;
	//CurrentCooldown = Cooldown;

}

void ASkillBase::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FSkillDataRow* Data = DataTableRowHandle.GetRow<FSkillDataRow>(TEXT("Skill"));
	if (!Data) { ensure(false); return; }
	SkillDataRow = Data;

	//Capsulecom
	USkeletalMeshComponent* MeshComponent = GetOwner()->GetComponentByClass<USkeletalMeshComponent>();
	//CharacterData안에 있는 Data를 사용해야 할거 같은데???
	//UAnimInstance* AnimInstance = GetOwner()->GetComponentByClass<UAnimInstance>();
	//AnimInstance->Montage_Play(SkillDataRow->DashMontage[0]);
	//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
}

// Called when the game starts or when spawned
void ASkillBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASkillBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASkillBase::ActivateSkill()
{

	
	//if (bCanUseSkill)
	//{


	//	StartCooldown();
	//}

}

void ASkillBase::StartCooldown()
{
	//bCanUseSkill = false;
	//GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &ABaseSkill::OnCooldownEnd, Cooldown, false);
}

void ASkillBase::OnCooldownEnd()
{
	//bCanUseSkill = true;
}

