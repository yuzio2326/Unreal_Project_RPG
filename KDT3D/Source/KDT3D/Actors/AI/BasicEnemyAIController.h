// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SplineComponent.h"
#include "Components/StatusComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BasicEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class KDT3D_API ABasicEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;
	
protected:
	UFUNCTION()
	void OnDamaged(float CurrentHP, float MaxHP);

	UFUNCTION()
	void ResetOnDamaged();

	void FindPlayerByPerception();

public:
	void SetPatrolPath(TObjectPtr<USplineComponent> NewPatrolPath) { PatrolPath = NewPatrolPath; }

protected:
	UPROPERTY()
	TObjectPtr<USplineComponent> PatrolPath;

protected:
	bool bDamaged = false;

	UPROPERTY()
	UStatusComponent* StatusComponentRef;
};
