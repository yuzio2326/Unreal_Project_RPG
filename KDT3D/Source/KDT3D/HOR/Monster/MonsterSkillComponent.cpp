// Fill out your copyright notice in the Description page of Project Settings.

#include "HOR/Monster/MonsterSkillComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "HOR/Monster/MonsterAIController.h"	//Changed
#include "Misc/Utils.h"
#include "Components/AdvanceFloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h" // GameplayStatics 포함 (PlayerPawn 찾기)

// Sets default values for this component's properties
UMonsterSkillComponent::UMonsterSkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMonsterSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	

	//SetData(DataTableRowHandle);
	// ...
	
}


// Called every frame
void UMonsterSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//get Skill Num 가지고 오기
	int32 RangedSkillNum = SkillTableRow.RangedSkillArray.Num();

	//get Skill Num 랜덤하게 가지고 오기
	int32 MeleeSkillNum = SkillTableRow.SkillArray.Num();

	//MeleeSkill이 있으면
	if (MeleeSkillNum > 0)
	{
		//TODO:: BroadCast로 ISAllSkillCooltime얘가 모두 쿨이면 사용 가능 여부를 false로 ㄱㄱ
		for (int32 i = 0; i < MeleeSkillNum; i++)
		{
			//단 하나라도 쿨타임이 다 돌았으면 false
			if (SkillCooldowns[i] <= 0)
			{	ISMeleeSkillCooltime = false;
				break;	}
			else { ISMeleeSkillCooltime = true; }
		}
		for (int32 i = 0; i < MeleeSkillNum; i++) {
			if (SkillCooldowns[i] > 0.0f) {
				SkillCooldowns[i] -= DeltaTime;
				SkillCooldowns[i] = FMath::Max(SkillCooldowns[i], 0.0f);  // 0보다 작아지지 않도록
			}
		}

	}
	else { ISMeleeSkillCooltime = true; }

	//Ranged Skill 이 있으면
	if (RangedSkillNum > 0)
	{
		//모든 스킬 쿨타임인지 아는 bool의 기본값을 true로
		for (int32 i = 0; i < RangedSkillNum; i++)
		{
			//단 하나라도 쿨타임이 다 돌았으면 false
			if (RangedSkillCooldowns[i] <= 0) 
			{	ISRangedSkillCooltime = false;
				break;	}
			else { ISRangedSkillCooltime = true; }
		}
		for (int32 i = 0; i < RangedSkillNum; i++) {
			if (RangedSkillCooldowns[i] > 0.0f) {
				RangedSkillCooldowns[i] -= DeltaTime;
				RangedSkillCooldowns[i] = FMath::Max(RangedSkillCooldowns[i], 0.0f);  // 0보다 작아지지 않도록
			}
		}

	}
	else{ ISRangedSkillCooltime = true; }
	
	//melee나 range둘중에 하나라도 쿨타임이 돌면
	if (!ISRangedSkillCooltime )
		OnUsingSkill.Broadcast(UsingSkill, true, true);
	else if (!ISMeleeSkillCooltime)
		OnUsingSkill.Broadcast(UsingSkill, true, false);


	//AddMovement(DeltaTime, iMovementType, fDelay);

	//Skill관련 체크용
	//UKismetSystemLibrary::K2_SetTimer(this, TEXT("UseSkill"),3,false);
	//check
	//if (!AnimInstance->Montage_IsPlaying(nullptr) ||
	//	AnimInstance->Montage_GetPlayRate(SkillTableRow.AttackAnimation[0]) < 0.5f)
	//{
	//	AnimInstance->Montage_Play(SkillTableRow.AttackAnimation[0]);
	//	
	//}

	// ...
}


void UMonsterSkillComponent::UseRangedSkill()
{
	bool CanUseSkill = true;

	if (!UsingSkill)
	{
		//get Skill Num 가지고 오기 1개면 0번 하나 자기 숫자 1
		int32 SkillNum = SkillTableRow.RangedSkillArray.Num();
		//RangedSkill 이 없으면 Broadcast 시키고 return
		if (SkillNum <= 0) 
		{ 
			OnUsingSkill.Broadcast(UsingSkill, CanUseSkill, false);
			return; 
		}

		//스킬중에 아무거나 하나 이상 사용 가능하면
		while (!ISRangedSkillCooltime)
		{
			int64 Index = 0;
			//Random 뽑고
			if (SkillNum > 0)
			{
				Index = FMath::RandRange(0, SkillNum-1);
			}
			else {	Index = 0;	}

			//쿨타임이면 다시 뽑고
			if (RangedSkillCooldowns[Index] > 0) { continue; }
			//아니면 해당 스킬 사용
			else
			{
				SkillCount = Index;
				break;
			}
		}
		//스킬 모두 쿨타임이면
		if (ISMeleeSkillCooltime && ISRangedSkillCooltime)
		{
			UsingSkill = false;
			CanUseSkill = false;
		}
		else
		{
			CurrentAnimCount = 0;
			UsingSkill = true;
			CanUseSkill = false;
		}
		OnUsingSkill.Broadcast(UsingSkill, CanUseSkill, false);

	}
	else //skill 사용중이면 남은 몽타주 다 재생 하고 마지막 몽타주일 경우 UsingSkill얘를 false로 ㄱㄱ
	{
		//재생하려는 몽타주 Array 가져오기
		FSkillDataRow CurrentSkillMontage = SkillTableRow.RangedSkillArray[SkillCount];
		//재생하려는 애니메이션 갯수
		int32 SkillMontageNum = CurrentSkillMontage.SkillAnimation.Num();
		//사용하려는 애니메이션이 재생하려는 애니메이션 갯수보다 크면 초기화
		//다음 애니메이션이 없으면
		if (CurrentAnimCount >= SkillMontageNum)
		{
			UsingSkill = false;
			CurrentAnimCount = 0;
			//skill 쿨타임 전부 돌려서 확인 하고 사용 가능한놈이 하나라도 있으면 true로 하기
			CanUseSkill = true;
			OnUsingSkill.Broadcast(UsingSkill, CanUseSkill, true);
			RangedSkillCooldowns[SkillCount] = SkillTableRow.RangedSkillArray[SkillCount].SkillCoolTime;
			SkillCount = 0;// 초기화
		}
		//다음에 재생할 몽타주가 있으면
		else
		{
			//일반공격 초기화
			AttackCount = 0;
			TArray<UAnimMontage*> CurrentSkillMontageArray = SkillTableRow.RangedSkillArray[SkillCount].SkillAnimation;
			bool FastChangeMontage = false;

			//다음에 재생할 몽타주 선택
			if (CurrentAnimCount > 0)
			{
				//스킬 작동 구현하기
				ActivateSkill(CurrentAnimCount);
			}
			//몽타주 재생
			if (!AnimInstance->Montage_IsPlaying(nullptr) ||
				FastChangeMontage)
			{
				AnimInstance->Montage_Play(CurrentSkillMontageArray[CurrentAnimCount]);
				CurrentAnimCount++;
				CanUseSkill = false;
				//스킬 사용중이고 스킬 애니메이션 다 안돌았음
				OnUsingSkill.Broadcast(UsingSkill, CanUseSkill, true);
			}

		}
	}

}


void UMonsterSkillComponent::UseSkill()
{
	// Use Skill 에서 Random 한 int값 가져와서 넣고 CanUseSkill에서 해당 스킬의 쿨타임을 체크
	// 이후 CanUseSkill에서 사용 가능 여부를 물어보고 사용 가능하면 true로 
	// 반환된 값이 true면 사용하고 아니면 다시 Random 돌리고
	// 기능 구현은 UseSkill에서 넣고 UseSkill 내부에 내부쿨 넣자
	// 해당 스킬 사용했으면 UsedSkill 함수를 만들던지 해서 쿨타임 넣는 방식 ㄱㄱ


	//TODO:: 이거 완성 시켜서 스킬 쫙 사용하게 하고 DATATable에 원거리 캐릭이나 다른 캐릭터들 추가



	bool CanUseSkill = true;

	//skill 이 사용중이 아니면 skill 뽑아서 쿨타임 비교하고 랜덤하게 사용하기
	if (!UsingSkill)
	{
		//get Skill Num 랜덤하게 가지고 오기
		int32 SkillNum = SkillTableRow.SkillArray.Num();	//1개 있으면 0
		//MeleeSkill 이 없으면 Broadcast 시키고 return
		if (SkillNum <= 0)
		{
			OnUsingSkill.Broadcast(UsingSkill, CanUseSkill, true);
			return;
		}

		//스킬중에 아무거나 하나 이상 사용 가능하면
		while(!ISMeleeSkillCooltime)
		{
			//Random 뽑고
			int64 Index = FMath::RandRange(0, SkillNum-1);
			//쿨타임이면 다시 뽑고
			if (SkillCooldowns[Index] > 0) { continue; }
			//아니면 해당 스킬 사용
			else 
			{
				SkillCount = Index;
				break;
			}
			
		}
		//스킬 모두 쿨타임이면
		if (ISMeleeSkillCooltime && ISRangedSkillCooltime)
		{
			UsingSkill = false;
			CanUseSkill = false;
		}
		else 
		{

			CurrentAnimCount = 0;
			UsingSkill = true;
			CanUseSkill = false;

		}
		OnUsingSkill.Broadcast(UsingSkill, CanUseSkill, false);

	}
	else //skill 사용중이면 남은 몽타주 다 재생 하고 마지막 몽타주일 경우 UsingSkill얘를 false로 ㄱㄱ
	{
		//재생하려는 몽타주 Array 가져오기
		FSkillDataRow CurrentSkillMontage = SkillTableRow.SkillArray[SkillCount];

		//재생하려는 애니메이션 갯수
		int32 SkillMontageNum = SkillTableRow.SkillArray[SkillCount].SkillAnimation.Num();
		
		//사용하려는 애니메이션이 재생하려는 애니메이션 갯수보다 크면 초기화
		//다음 애니메이션이 없으면
		if (CurrentAnimCount >= SkillMontageNum)
		{
			UsingSkill = false;
			CurrentAnimCount = 0;
			//skill 쿨타임 전부 돌려서 확인 하고 사용 가능한놈이 하나라도 있으면 true로 하기
			CanUseSkill = true;		
			OnUsingSkill.Broadcast(UsingSkill, CanUseSkill,false);
			SkillCooldowns[SkillCount] = SkillTableRow.SkillArray[SkillCount].SkillCoolTime;
			SkillCount = 0;//초기화
		}
		//다음에 재생할 몽타주가 있으면
		else 
		{
			//일반공격 초기화
			AttackCount = 0;
			TArray<UAnimMontage*> CurrentSkillMontageArray = SkillTableRow.SkillArray[SkillCount].SkillAnimation;
			bool FastChangeMontage = false;
		
			//다음에 재생할 몽타주 선택
			if (CurrentAnimCount > 0)
			{
				//스킬 작동 구현하기
				ActivateSkill(CurrentAnimCount);
			}
			//몽타주 재생
			if (!AnimInstance->Montage_IsPlaying(nullptr) || 
				FastChangeMontage)
			{
				AnimInstance->Montage_Play(CurrentSkillMontageArray[CurrentAnimCount]);
				CurrentAnimCount++;
				CanUseSkill = false;
				//스킬 사용중이고 스킬 애니메이션 다 안돌았음
				OnUsingSkill.Broadcast(UsingSkill, CanUseSkill, false);
			}
		}
	}

	//해당 array의 몽타주 갯수
	


	//CanUseSkill(SkillNum);

}

void UMonsterSkillComponent::ActivateSkill(int ActivateCurrentAnim)
{
	//스킬 기능 구현 부분입니다.
	TArray<UAnimMontage*> CurrentSkillMontage = SkillTableRow.RangedSkillArray[SkillCount].SkillAnimation;
	if (!CurrentSkillMontage.IsValidIndex(ActivateCurrentAnim)) { return; }

	FString CurrentMontageName = CurrentSkillMontage[ActivateCurrentAnim]->GetName();
	
	FString TP_Ult = TEXT("Boss_Ult_02");
	FString TP = TEXT("Boss_TP_02");
	FString Aurora_BackStep = TEXT("Boss_Aurora_SK01_TP");
	FString Aurora_Dash = TEXT("Boss_Aurora_SK02");

	//UKismetSystemLibrary::SphereTraceSingleByProfile(MeshComp,
	//	WeaponSocketLocation, WeaponSocketLocation, 100.f,
	//	ProfileName, false, IgnoreActors, EDrawDebugTrace::None,
	//	HitResult, true, FLinearColor::Red, FLinearColor::Green, 1.f))

	FRotator Rotation = GetOwner()->GetActorRotation();
	FRotator RotationYaw = FRotator(0.0, Rotation.Yaw, 0.0);
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(RotationYaw);
	FVector ActorLocation = GetOwner()->GetActorLocation();
	USkeletalMeshComponent* OwnerMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();

	//Skill 종류
	if (CurrentMontageName == TP_Ult || CurrentMontageName == TP || CurrentMontageName == Aurora_BackStep)
	{
		FHitResult HitResult;
		float TPDistance = 800.f;
		TArray<AActor*> IgnoreActors;
		if (GetOwner()->GetWorld() != GWorld) { return; }
		bool bHit = GetOwner()->GetWorld()->SweepSingleByChannel(HitResult, ActorLocation,
			ActorLocation,FQuat::Identity, ECC_Pawn,	FCollisionShape::MakeSphere(TPDistance));
		if (bHit && HitResult.GetActor())
		{
			APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
			//TP성공
			if (PlayerPawn == HitResult.GetActor())
			{
				FVector TPArrive = HitResult.GetActor()->GetActorLocation();
				FVector AddArrive = TPArrive + (ForwardVector * -50);
				GetOwner()->SetActorLocation(AddArrive);
				FRotator LookRotator = UKismetMathLibrary::FindLookAtRotation(ActorLocation, AddArrive);
				GetOwner()->SetActorRotation(LookRotator);
			}
			else
			{
				FVector Teleport = ActorLocation + (ForwardVector * TPDistance);
				GetOwner()->SetActorLocation(Teleport);
			}
		}
	}
	else if (CurrentMontageName == Aurora_Dash)
	{
		//일정 범위 안에 대상 찾으면 해당 방향으로 돌진
		FHitResult HitResult;
		float TPDistance = 800.f;
		TArray<AActor*> IgnoreActors;
		if (GetOwner()->GetWorld() != GWorld) { return; }
		bool bHit = GetOwner()->GetWorld()->SweepSingleByChannel(HitResult, ActorLocation,
			ActorLocation, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(TPDistance));
		if (bHit && HitResult.GetActor())
		{
			APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
			if (PlayerPawn == HitResult.GetActor())
			{
				FVector TargetLocation = HitResult.GetActor()->GetActorLocation() + (ForwardVector * -50);
				FRotator LookRotator = UKismetMathLibrary::FindLookAtRotation(ActorLocation, TargetLocation);
				//메쉬 회전값 더하기
				UE_LOG(LogTemp, Warning, TEXT("HitActor is True"));
				AAIController* AIController = Cast<AAIController>(GetOwner()->GetInstigatorController());
				if (AIController) { AIController->SetControlRotation(LookRotator); }
				else { GetOwner()->SetActorRotation(LookRotator); }
			}
		}
		//못찾으면 그냥 돌진
		FVector LaunchDirection = ForwardVector * 1000;
		MovementComponent->LaunchPawn(LaunchDirection, true, true);

		UStatusComponent* MonsterStatus = GetOwner()->FindComponentByClass<UStatusComponent>();
		if (MonsterStatus->GetHP() != MonsterStatus->GetMaxHP())
		{
			MonsterStatus->AddHP(400);
		}
		//if(GetOwner())
	}
	
}

void UMonsterSkillComponent::AddMovement(float DeltaTime, int MovementType, float Delay)
{

	
}

void UMonsterSkillComponent::Attack()
{
	//TODO:: ㄱ 밑에 있는거 실행하기
	//SkillComponet의 Animinstance만 바뀌고 상위의 MeshComp에서 바꾸지는 않아서 안돌아감
	//상위로 보내서 이걸 실행하도록 합시다
	//스킬 쿨 관련만 여기서 만들고 공격 및 스킬 애니메이션 재생은 BossMonster에게 
	//if (SkillTableRow.AttackAnimation.IsEmpty()) { return; }
	//UAnimMontage* AttackMontage = SkillTableRow.AttackAnimation[0];
	//USkeletalMeshComponent* OwnerMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	//UAnimInstance* TestAnimInstance = OwnerMesh->GetAnimInstance();
	////TestAnimInstance->Montage_Play(AttackMontage);
	//if (!AnimInstance->Montage_IsPlaying(AttackMontage))
	//{
	//	AnimInstance->Montage_Play(AttackMontage);
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Montage is playing!"));
	//}
	//else
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Montage already playing or failed to play."));
	//}

	/* Test*/


	int32 AttackAnimNum = SkillTableRow.AttackAnimation.Num() - 1;
	if (AttackCount > AttackAnimNum) { AttackCount = 0; }
	if (AttackAnimNum > 0)
	{
		// 아닌데.. Montage가 Play 중이라면 BT 내부에서 AI 진행을 멈추게 했는데
		// Montage가 play되지는 않고 
		// 모든 몽타주가 재생 중이지 않을 때  
		if (!AnimInstance->Montage_IsPlaying(nullptr))
		{
			AnimInstance->Montage_Play(SkillTableRow.AttackAnimation[AttackCount]);
			AttackCount++;
		}
	}
}

void UMonsterSkillComponent::InitializeSkillData(UDataTable* SkillDataTable)
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

void UMonsterSkillComponent::SetData(FDataTableRowHandle InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FSkillTableRow* Data = DataTableRowHandle.GetRow<FSkillTableRow>(TEXT("SkillTableRow"));
	if (!Data) { ensure(false); return; }
	SkillTableRow = *Data;

	//Owner 정보 빼오고
	SkeletalMeshComponent = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	MovementComponent = GetOwner()->FindComponentByClass<UAdvanceFloatingPawnMovement>();

	SkillCooldowns.SetNum(SkillTableRow.SkillArray.Num());
	for (int32 i = 0; i < SkillTableRow.SkillArray.Num()-1; i++) 
	{
		SkillCooldowns[i] = 0.0f; 
	}
	RangedSkillCooldowns.SetNum(SkillTableRow.RangedSkillArray.Num());
	for (int32 i = 0; i < SkillTableRow.RangedSkillArray.Num()-1; i++)
	{
		RangedSkillCooldowns[i] = 0.0f;
	}
	if (SkillTableRow.RangedSkillArray.Num() > 0) 
		OnUsingSkill.Broadcast(false, true, true);
	else
		OnUsingSkill.Broadcast(false, true, false);

}

