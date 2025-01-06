// Fill out your copyright notice in the Description page of Project Settings.


#include "HOR/SkillComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HOR/SkillBase.h"

// Sets default values for this component's properties
USkillComponent::USkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	SetData(DataTableRowHandle);

	// ...
	
}


// Called every frame
void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CooltimeSet(DeltaTime);
	// ...
}

void USkillComponent::CooltimeSet(float DeltaTime)
{
	//FSkillTableRow* SkillTablePoint = &SkillTableRow;
	if (SkillTableRow.SkillArray.IsEmpty()) { return; }

	int MaxSkillNum = SkillTableRow.SkillArray.Num()-1;

	if (Sk01Cooltime > 0) { Sk01Cooltime -= DeltaTime; }
	OnPlayerUseSkill01.Broadcast(Sk01Cooltime, MaxSk01Cooltime);
	if (Sk02Cooltime > 0) { Sk02Cooltime -= DeltaTime; }
	OnPlayerUseSkill02.Broadcast(Sk02Cooltime, MaxSk02Cooltime);
	if (Sk03Cooltime > 0) { Sk03Cooltime -= DeltaTime; }
	OnPlayerUseSkill03.Broadcast(Sk03Cooltime, MaxSk03Cooltime);
	if (Sk04Cooltime > 0) { Sk04Cooltime -= DeltaTime; }
	OnPlayerUseSkill04.Broadcast(Sk04Cooltime, MaxSk04Cooltime);
	if (SkQCooltime > 0) { SkQCooltime -= DeltaTime; }
	OnPlayerUseSkillQ.Broadcast(SkQCooltime, MaxSkQCooltime);
	if (SkECooltime > 0) { SkECooltime -= DeltaTime; }
	OnPlayerUseSkillE.Broadcast(SkECooltime, MaxSkECooltime);
	if (SkRCooltime > 0) { SkRCooltime -= DeltaTime; }
	OnPlayerUseSkillR.Broadcast(SkRCooltime, MaxSkRCooltime);
	CurrentPlaytime -= DeltaTime;
	if (CurrentPlaytime < 0.8f) 
	{ UsingSkill = false; }

	//skilldata뽑고 1번 anim가져오기
	//FSkillDataRow SkillData = SkillTableRow.SkillArray[1];//empty 입니다 ->setData가 호출이 안됌
	//setdata호출 시키고 다시 체크ㄱㄱ
	//쿨타임 확인
	//float CurrentSkillCoolTime = SkillData.CurrentSkillCoolTime;


	//if (Sk01Cooltime > 0)
	
	//if (Sk01Cooltime > 0)

}

void USkillComponent::InitializeSkillData(UDataTable* SkillDataTable)
{
	if (!SkillDataTable) return;

	static const FString ContextString(TEXT("Skill Data Context"));
	TArray<FSkillDataRow*> AllSkillData;
	SkillDataTable->GetAllRows<FSkillDataRow>(ContextString, AllSkillData);

	for (FSkillDataRow* SkillData : AllSkillData)
	{
		if (SkillData)
		{
			// 필요한 데이터만 저장하거나, 초기화합니다.
			//SkillDataArray.Add(*SkillData);
		}
	}

}

void USkillComponent::SetData(FDataTableRowHandle InDataTableRowHandle)
{

	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FSkillTableRow* Data = DataTableRowHandle.GetRow<FSkillTableRow>(TEXT("SkillTableRow"));
	
	if (!Data) { ensure(false); return; }
	//값 가져오는중 포인터가 안되서 값으로 받고
	SkillTableRow = *Data;

	USkeletalMeshComponent* OwnerMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	StatusComponent = OwnerMesh->GetOwner()->FindComponentByClass<UCustomDioStatusComponent>();
	
	//TArray<FSkillDataRow> SkillData = SkillTableRow.SkillArray;

	//for (int i = 0; i < SkillNum; i++)
	//{
	//	SkillCoolTimeArray[SkillNum] = SkillTableRow.SkillArray[SkillNum].SkillCoolTime;
	//}


	//TArray<FSkillDataRow*> SkillDataArray = SkillTableRow.SkillArray;

}

void USkillComponent::UseSkill(int Skill_Index)
{

	//UCustomDioStatusComponent* StatusComponent = GetOwner()->FindComponentByClass<UCustomDioStatusComponent>();

	//int PlayerMP = StatusComponent->GetMP();
	//int RequireMP = SkillDataRow->UseSPNum;	//UseSPNum를 MP로 이름 바꾸기
	//if (SkillDataRow->CurrentSkillCoolTime <= 0) 
	//{
		//if ((PlayerMP + RequireMP) < 0) { return; }	//얻을 경우 + 사용하면 음수임
	//}

	//controller 에서 use skill 01 하면 skill 01 실행



}

bool USkillComponent::CanUseSk(int SkillNum)
{

	CurrentAnimIndex = SkillNum;

	if (Sk01Cooltime <= 0) { bCanUseskill01 = true; }
	if (Sk01Cooltime > 0) { bCanUseskill01 = false; }
	if (Sk02Cooltime <= 0) { bCanUseskill02 = true; }
	if (Sk02Cooltime > 0) { bCanUseskill02 = false; }
	if (Sk03Cooltime <= 0) { bCanUseskill03 = true; }
	if (Sk03Cooltime > 0) { bCanUseskill03 = false; }
	if (Sk04Cooltime <= 0) { bCanUseskill04 = true; }
	if (Sk04Cooltime > 0) { bCanUseskill04 = false; }
	if (SkQCooltime <= 0) { bCanUseskillQ = true; }
	if (SkQCooltime > 0) { bCanUseskillQ = false; }
	if (SkECooltime <= 0) { bCanUseskillE = true; }
	if (SkECooltime > 0) { bCanUseskillE = false; }
	if (SkRCooltime <= 0) { bCanUseskillR = true; }
	if (SkRCooltime > 0) { bCanUseskillR = false; }
	bool BoundCheck = StatusComponent->IsBound();
	bool CanUseSkill = false;
	if (!BoundCheck)
	{
		switch (SkillNum)
		{
		case 1: Findingbool=bCanUseskill01;
			break;
		case 2: Findingbool=bCanUseskill02;
			break;
		case 3: Findingbool=bCanUseskill03;
			break;
		case 4: Findingbool=bCanUseskill04;
			break;
		case 5: Findingbool= bCanUseskillQ;
			break;
		case 6: Findingbool= bCanUseskillE;
			break;
		case 7: Findingbool= bCanUseskillR;
			break;

		default:
			break;
		}
		CanUseSkill = Findingbool && !CheckUsingSkill();
	}

	return CanUseSkill;
}

void USkillComponent::UseSK_01()
{

	//FSkillTableRow* SkillTablePoint = &SkillTableRow;
	if (SkillTableRow.SkillArray.IsEmpty()) { return; }
	//skilldata 뽑고 Array 1번 anim가져오기
	FSkillDataRow SkillData = SkillTableRow.SkillArray[1];
	//empty 입니다 ->setData가 호출이 안됌

	//setdata호출 시키고 다시 체크ㄱㄱ
	//쿨타임 확인 및 다른 스킬 사용중인지
	bool BoundCheck = StatusComponent->IsBound();
	if (Sk01Cooltime > 0 && !UsingSkill && !BoundCheck) { return; }

	TArray<UAnimMontage*> SkillAnimationMontage = SkillData.SkillAnimation;

	//Owner 정보 빼오고
	USkeletalMeshComponent* OwnerMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	UAnimInstance* AnimInstance = OwnerMesh->GetAnimInstance();

	//skilldata에 있는 애니메이션 갯수 가져오고
	const int64 MaxIndex = SkillAnimationMontage.Num()-1;

	//Montage Play		0번 montageplay 다음 1번 ㄱㄱ하게 하기
	UsingSkill = true;
	AnimInstance->Montage_Play(SkillAnimationMontage[0]);
	if (MaxSk01Cooltime <= 0)
	{
		MaxSk01Cooltime = SkillTableRow.SkillArray[1].SkillCoolTime;
	}

	Sk01Cooltime = SkillTableRow.SkillArray[1].SkillCoolTime;
	
	CurrentPlaytime = SkillAnimationMontage[0]->GetPlayLength();

	OnPlayerMPChanged.Broadcast(2, 10);

}
void USkillComponent::UseSK_02()
{
	//FSkillTableRow* SkillTablePoint = &SkillTableRow;
	if (SkillTableRow.SkillArray.IsEmpty()) { return; }
	//skilldata 뽑고 Array 1번 anim가져오기
	FSkillDataRow SkillData = SkillTableRow.SkillArray[2]; //empty 입니다 ->setData가 호출이 안됌

	//setdata호출 시키고 다시 체크ㄱㄱ
	//쿨타임 확인
	bool BoundCheck = StatusComponent->IsBound();

	if (Sk02Cooltime > 0 && !UsingSkill && !BoundCheck) { return; }
	TArray<UAnimMontage*> SkillAnimationMontage = SkillData.SkillAnimation;
	//Owner 정보 빼오고
	USkeletalMeshComponent* OwnerMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	UAnimInstance* AnimInstance = OwnerMesh->GetAnimInstance();
	//skilldata에 있는 애니메이션 갯수 가져오고
	const int64 MaxIndex = SkillAnimationMontage.Num()-1;
	//Montage Play		0번 montageplay 다음 1번 ㄱㄱ하게 하기
	UsingSkill = true;
	AnimInstance->Montage_Play(SkillAnimationMontage[0]);
	if (MaxSk02Cooltime <= 0)
	{
		MaxSk02Cooltime = SkillTableRow.SkillArray[2].SkillCoolTime;
	}
	Sk02Cooltime = SkillData.SkillCoolTime;
	//Sk02Cooltime = SkillCoolTimeArray[2];
	CurrentPlaytime = SkillAnimationMontage[0]->GetPlayLength();
	OnPlayerMPChanged.Broadcast(3, 10);

}

void USkillComponent::UseSK_03()
{
	//FSkillTableRow* SkillTablePoint = &SkillTableRow;
	if (SkillTableRow.SkillArray.IsEmpty()) { return; }
	//skilldata 뽑고 Array 1번 anim가져오기
	FSkillDataRow SkillData = SkillTableRow.SkillArray[3]; //empty 입니다 ->setData가 호출이 안됌

	//setdata호출 시키고 다시 체크ㄱㄱ
	//쿨타임 확인
	bool BoundCheck = StatusComponent->IsBound();

	if (Sk03Cooltime > 0 && !UsingSkill && BoundCheck) { return; }
	TArray<UAnimMontage*> SkillAnimationMontage = SkillData.SkillAnimation;
	//Owner 정보 빼오고
	USkeletalMeshComponent* OwnerMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	UAnimInstance* AnimInstance = OwnerMesh->GetAnimInstance();
	//skilldata에 있는 애니메이션 갯수 가져오고
	const int64 MaxIndex = SkillAnimationMontage.Num()-1;
	//Montage Play		0번 montageplay 다음 1번 ㄱㄱ하게 하기
	UsingSkill = true;
	AnimInstance->Montage_Play(SkillAnimationMontage[0]);

	if (MaxSk03Cooltime <= 0)
	{
		MaxSk03Cooltime = SkillTableRow.SkillArray[3].SkillCoolTime;
	}

	Sk03Cooltime = SkillData.SkillCoolTime;
	//Sk03Cooltime = SkillCoolTimeArray[3];
	CurrentPlaytime = SkillAnimationMontage[0]->GetPlayLength();
	OnPlayerMPChanged.Broadcast(-2, 10);
}

void USkillComponent::UseSK_04()
{
	//FSkillTableRow* SkillTablePoint = &SkillTableRow;
	if (SkillTableRow.SkillArray.IsEmpty()) { return; }
	//skilldata 뽑고 Array 1번 anim가져오기
	FSkillDataRow SkillData = SkillTableRow.SkillArray[4]; //empty 입니다 ->setData가 호출이 안됌

	//setdata호출 시키고 다시 체크ㄱㄱ
	//쿨타임 확인
	bool BoundCheck = StatusComponent->IsBound();

	if (Sk04Cooltime > 0 && !UsingSkill && BoundCheck) { return; }
	TArray<UAnimMontage*> SkillAnimationMontage = SkillData.SkillAnimation;
	//Owner 정보 빼오고
	USkeletalMeshComponent* OwnerMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	UAnimInstance* AnimInstance = OwnerMesh->GetAnimInstance();
	//skilldata에 있는 애니메이션 갯수 가져오고
	const int64 MaxIndex = SkillAnimationMontage.Num()-1;
	//Montage Play		0번 montageplay 다음 1번 ㄱㄱ하게 하기
	UsingSkill = true;
	AnimInstance->Montage_Play(SkillAnimationMontage[0]);

	if (MaxSk04Cooltime <= 0)
	{
		MaxSk04Cooltime = SkillTableRow.SkillArray[4].SkillCoolTime;
	}

	Sk04Cooltime = SkillData.SkillCoolTime;
	//Sk04Cooltime = SkillCoolTimeArray[4];
	CurrentPlaytime = SkillAnimationMontage[0]->GetPlayLength();
	OnPlayerMPChanged.Broadcast(-1, 10);
}

void USkillComponent::UseSK_Q()
{
	//FSkillTableRow* SkillTablePoint = &SkillTableRow;
	if (SkillTableRow.SkillArray.IsEmpty()) { return; }
	//skilldata 뽑고 Array 1번 anim가져오기
	FSkillDataRow SkillData = SkillTableRow.SkillArray[5]; //empty 입니다 ->setData가 호출이 안됌

	//setdata호출 시키고 다시 체크ㄱㄱ
	//쿨타임 확인
	if (SkQCooltime > 0 && !UsingSkill && !StatusComponent->IsBound()) { return; }
	TArray<UAnimMontage*> SkillAnimationMontage = SkillData.SkillAnimation;
	//Owner 정보 빼오고
	USkeletalMeshComponent* OwnerMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	UAnimInstance* AnimInstance = OwnerMesh->GetAnimInstance();
	//skilldata에 있는 애니메이션 갯수 가져오고
	const int64 MaxIndex = SkillAnimationMontage.Num() - 1;
	//Montage Play		0번 montageplay 다음 1번 ㄱㄱ하게 하기
	UsingSkill = true;
	AnimInstance->Montage_Play(SkillAnimationMontage[0]);

	if (MaxSkQCooltime <= 0)
	{
		MaxSkQCooltime = SkillTableRow.SkillArray[5].SkillCoolTime;
	}

	SkQCooltime = SkillData.SkillCoolTime;
	//SkQCooltime = SkillCoolTimeArray[5];
	CurrentPlaytime = SkillAnimationMontage[0]->GetPlayLength();
	OnPlayerMPChanged.Broadcast(-2, 10);
}

void USkillComponent::UseSK_E()
{
	//FSkillTableRow* SkillTablePoint = &SkillTableRow;
	if (SkillTableRow.SkillArray.IsEmpty()) { return; }
	//skilldata 뽑고 Array 1번 anim가져오기
	FSkillDataRow SkillData = SkillTableRow.SkillArray[6]; //empty 입니다 ->setData가 호출이 안됌
	//setdata호출 시키고 다시 체크ㄱㄱ
	//쿨타임 확인
	if (SkECooltime > 0 && !UsingSkill && !StatusComponent->IsBound()) { return; }
	TArray<UAnimMontage*> SkillAnimationMontage = SkillData.SkillAnimation;
	//Owner 정보 빼오고
	USkeletalMeshComponent* OwnerMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	UAnimInstance* AnimInstance = OwnerMesh->GetAnimInstance();
	//skilldata에 있는 애니메이션 갯수 가져오고
	const int64 MaxIndex = SkillAnimationMontage.Num() - 1;
	//Montage Play		0번 montageplay 다음 1번 ㄱㄱ하게 하기
	UsingSkill = true;
	AnimInstance->Montage_Play(SkillAnimationMontage[0]);

	if (MaxSkECooltime <= 0)
	{
		MaxSkECooltime = SkillTableRow.SkillArray[6].SkillCoolTime;
	}

	SkECooltime = SkillData.SkillCoolTime;
	//SkECooltime = SkillCoolTimeArray[6];
	CurrentPlaytime = SkillAnimationMontage[0]->GetPlayLength();
	OnPlayerMPChanged.Broadcast(-2, 10);
}

void USkillComponent::UseSK_R()
{
	//FSkillTableRow* SkillTablePoint = &SkillTableRow;
	if (SkillTableRow.SkillArray.IsEmpty()) { return; }
	//skilldata 뽑고 Array 1번 anim가져오기
	FSkillDataRow SkillData = SkillTableRow.SkillArray[7]; //empty 입니다 ->setData가 호출이 안됌

	//setdata호출 시키고 다시 체크ㄱㄱ
	//쿨타임 확인
	if (SkRCooltime > 0 && !UsingSkill && !StatusComponent->IsBound()) { return; }
	TArray<UAnimMontage*> SkillAnimationMontage = SkillData.SkillAnimation;
	//Owner 정보 빼오고
	USkeletalMeshComponent* OwnerMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	UAnimInstance* AnimInstance = OwnerMesh->GetAnimInstance();
	//skilldata에 있는 애니메이션 갯수 가져오고
	const int64 MaxIndex = SkillAnimationMontage.Num() - 1;
	//Montage Play		0번 montageplay 다음 1번 ㄱㄱ하게 하기
	UsingSkill = true;
	AnimInstance->Montage_Play(SkillAnimationMontage[0]);

	if (MaxSkRCooltime <= 0)
	{
		MaxSkRCooltime = SkillTableRow.SkillArray[7].SkillCoolTime;
	}

	SkRCooltime = SkillData.SkillCoolTime;
	//SkRCooltime = SkillCoolTimeArray[7];
	CurrentPlaytime = SkillAnimationMontage[0]->GetPlayLength();
	OnPlayerMPChanged.Broadcast(-4, 10);
}




