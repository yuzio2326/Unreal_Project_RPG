// Fill out your copyright notice in the Description page of Project Settings.

#include "HOR/Monster/MonsterAIController.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

void AMonsterAIController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(PatrolPath))
	{
		checkf(false, TEXT("PatrolPath not valid"));
		return;
	}

	if (!IsValid(BrainComponent))
	{
#pragma region BlackBoard Ref
		//UBehaviorTree* BehaviorTree = LoadObject<UBehaviorTree>(nullptr, TEXT("/Script/AIModule.BehaviorTree'/Game/Blueprint/AI/BT_BasicEnemy.BT_BasicEnemy'"));
#pragma endregion
		UBehaviorTree* BehaviorTree = LoadObject<UBehaviorTree>(nullptr, TEXT("/Script/AIModule.BehaviorTree'/Game/Blueprint/01_PersonalProjects/99_Enemy/AI/BT_Basic_Monster.BT_Basic_Monster'"));
		check(BehaviorTree);
		RunBehaviorTree(BehaviorTree);
	}

	if (IsValid(PatrolPath))
	{
		Blackboard->SetValueAsObject(TEXT("SplineComponent"), PatrolPath);
	}
	Blackboard->SetValueAsBool(TEXT("CanUseSkill"), true);
}

void AMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	StatusComponentRef = InPawn->GetComponentByClass<UStatusComponent>();
	StatusComponentRef->OnHPChanged.AddDynamic(this, &ThisClass::OnDamaged);

	MonsterSkillComponentRef = InPawn->GetComponentByClass<UMonsterSkillComponent>();
	MonsterSkillComponentRef->OnUsingSkill.AddDynamic(this, &ThisClass::OnUseSkill);

}

void AMonsterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckStopAI();

	if (!bDamaged)
	{
		FindPlayerByPerception();
	}
}

void AMonsterAIController::OnDamaged(float CurrentHP, float MaxHP)
{
	bDamaged = true;
	AController* Instigator_ = StatusComponentRef->GetLastInstigator();
	APawn* InstigatorPawn = Instigator_->GetPawn();
	check(InstigatorPawn);
	Blackboard->SetValueAsObject(TEXT("DetectedPlayer"), Cast<UObject>(InstigatorPawn));
	SetFocus(InstigatorPawn, 2);
	if (!bBossAgro)
	{
		UKismetSystemLibrary::K2_SetTimer(this, TEXT("ResetOnDamaged"), 10.f, false);
	}
	else{ UKismetSystemLibrary::K2_SetTimer(this, TEXT("ResetOnDamaged"), 20.f, false); }
}

void AMonsterAIController::ResetOnDamaged()
{
	bDamaged = false;
}

void AMonsterAIController::FindPlayerByPerception()
{
	APawn* OwningPawn = GetPawn();
	if (UAIPerceptionComponent* AIPerceptionComponent = OwningPawn->GetComponentByClass<UAIPerceptionComponent>())
	{
		TArray<AActor*> OutActors;
		AIPerceptionComponent->GetCurrentlyPerceivedActors(UAISenseConfig_Sight::StaticClass(), OutActors);

		bool bFound = false;
		for (AActor* It : OutActors)
		{
			if (ACharacter* DetectedPlayer = Cast<ACharacter>(It))
			{
				bFound = true;
				Blackboard->SetValueAsObject(TEXT("DetectedPlayer"), Cast<UObject>(DetectedPlayer));
				SetFocus(DetectedPlayer,2);
				break;
			}
		}
		if (!bFound && !bBossAgro)
		{
			Blackboard->ClearValue(TEXT("DetectedPlayer"));
		}
	}
}

void AMonsterAIController::SetFocus(AActor* NewFocus, EAIFocusPriority::Type InPriority)
{
	// clear out existing
	ClearFocus(InPriority);

	// now set new
	if (NewFocus)
	{
		if (InPriority >= FocusInformation.Priorities.Num())
		{
			FocusInformation.Priorities.SetNum(InPriority + 1);
		}
		FocusInformation.Priorities[InPriority].Actor = NewFocus;
	}

}

void AMonsterAIController::OnAttack()
{
}

void AMonsterAIController::OnUseSkill(bool InUsingSkill, bool InCanUseSkill, bool InIsRangeSkill)
{
	Blackboard->SetValueAsBool(TEXT("UsingSkill"), InUsingSkill);
	Blackboard->SetValueAsBool(TEXT("CanUseSkill"), InCanUseSkill);
	Blackboard->SetValueAsBool(TEXT("IsRangeSkill"), InIsRangeSkill);
}


void AMonsterAIController::CheckStopAI()
{

	APawn* OwningPawn = GetPawn();

	// Montage가 Play 중이라면 BT 내부에서 AI 진행을 멈춘다
	const bool bMontagePlaying = OwningPawn->GetComponentByClass<USkeletalMeshComponent>()->GetAnimInstance()->IsAnyMontagePlaying();


	const UAnimMontage* CurrentMontage = OwningPawn->GetComponentByClass<USkeletalMeshComponent>()->GetAnimInstance()->GetCurrentActiveMontage();
	const float CurrentPosition = OwningPawn->GetComponentByClass<USkeletalMeshComponent>()->GetAnimInstance()->Montage_GetPosition(CurrentMontage);
	const float MontageLength = OwningPawn->GetComponentByClass<USkeletalMeshComponent>()->GetAnimInstance()->Montage_GetPlayRate(CurrentMontage);
	bool StopAI = false;
	//A가 사용중이면 Stop, A가 사용중인데 0.2보다 낮으면 Play가 되어야함
	if (MontageLength > 0.2f && bMontagePlaying) { StopAI = true; }
	else { StopAI = false; }

	Blackboard->SetValueAsBool(TEXT("MontagePlaying"), StopAI);

}
