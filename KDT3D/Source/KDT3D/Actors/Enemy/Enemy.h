// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StatusComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/WeaponChildActorComponent.h"
#include "Components/SplineComponent.h"
#include "Components/AdvanceFloatingPawnMovement.h"
#include "PaperSpriteComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Actors/AI/PatrolPath.h"
#include "Misc/Utils.h"
#include "Animation/BaisicCharacterAnimInstance.h"
#include "Data/PawnData.h"
#include "Enemy.generated.h"

UCLASS()
class KDT3D_API AEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy(const FObjectInitializer& ObjectInitializer);
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);

protected:
	virtual void PostDuplicate(EDuplicateMode::Type DuplicateMode) override;
	virtual void PostLoad() override;
	virtual void PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph) override;
	
	/** Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay */
	virtual void PostInitializeComponents() override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform);

protected:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	virtual void OnPaperBurnEffect(float InDissolve);
	UFUNCTION()
	virtual void OnPaperBurnEffectEnd();

protected:
	UFUNCTION()
	virtual void OnDie();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float CheckPower();

protected:
	float EnemyHP;
	float EnemyDMG;


protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<APatrolPath> PatrolPathRef;

protected: // 옵션에 따라 생성되고 Root로 지정 됩니다.
	UPROPERTY()
	TObjectPtr<UShapeComponent> CollisionComponent;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> HPWidgetComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWeaponChildActorComponent> Weapon;

	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* MinimapSpriteComponent;

protected:
	UPROPERTY(VisibleAnywhere)
	UAdvanceFloatingPawnMovement* MovementComponent;

	UPROPERTY(VisibleAnywhere)
	UStatusComponent* StatusComponent;

	UPROPERTY(VisibleAnywhere)
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere)
	UAISenseConfig_Sight* AISenseConfig_Sight;

	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* PaperBurnEffectTimelineComponent;
	TArray<UMaterialInstanceDynamic*> MaterialInstanceDynamics;

protected:
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/KDT3D.PawnTableRow"))
	FDataTableRowHandle DataTableRowHandle;

	FPawnTableRow* EnemyData;

	UAnimMontage* CurrentDieMontage;
};
