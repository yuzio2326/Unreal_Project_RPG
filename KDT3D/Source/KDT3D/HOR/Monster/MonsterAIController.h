// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SplineComponent.h"
#include "HOR/CustomDioStatusComponent.h"
#include "Components/StatusComponent.h"
#include "HOR/Monster/MonsterSkillComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "MonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class KDT3D_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;

protected:
	//OnStun 이랑 Bound 추가하고 각각 어떻게 가져올건지 정하기 일정 체력 이하 stun 
	//일정 데미지 이상 Bound or applyDmg에서 type 도 넣어야됌
	UFUNCTION()
	void OnDamaged(float CurrentHP, float MaxHP);

	UFUNCTION()
	void ResetOnDamaged();

	void FindPlayerByPerception();
	virtual void SetFocus(AActor* NewFocus, EAIFocusPriority::Type InPriority = EAIFocusPriority::Gameplay);

	UFUNCTION()
	void OnAttack();

	UFUNCTION()
	void OnUseSkill(bool InUsingSkill, bool InCanUseSkill, bool InIsRangeSkill);



public:
	void SetPatrolPath(TObjectPtr<USplineComponent> NewPatrolPath) { PatrolPath = NewPatrolPath; }
	void BossAgro(bool IsBoss) { bBossAgro = IsBoss; }

	void CheckStopAI();

protected:
	UPROPERTY()
	TObjectPtr<USplineComponent> PatrolPath;

protected:
	bool bDamaged = false;
	bool bUsingSkill = false;
	bool bBossAgro = false;

	UPROPERTY()
	UStatusComponent* StatusComponentRef;

	UPROPERTY()
	UMonsterSkillComponent* MonsterSkillComponentRef;

	UPROPERTY()
	UCustomDioStatusComponent* PlayerStatusComponentRef;
};
