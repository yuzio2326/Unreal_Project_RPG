// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Enemy/Enemy.h"		//For PawnTableRow /얘도 그냥 뺄까??
#include "HOR/SkillComponent.h"		//안에 skillbase도 있음
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "HOR/Monster/MonsterSkillComponent.h"
#include "Components/AdvanceFloatingPawnMovement.h"
#include "Components/SplineComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BossMonster.generated.h"

/**
 * 
 */
USTRUCT()
struct KDT3D_API FBossMonsterTableRow : public FPawnTableRow
{
	GENERATED_BODY()
	//Skill 없으면 그냥 Pawn 사용 해도 되지만 그냥 저 테이블 가져다가 붙여 쓰는게 나을듯함

public:
	UPROPERTY(EditAnywhere, Category = "MonsterSkill")
	TArray<FSkillDataRow> SkillDataRow;		//스킬 하나씩 세팅 ㄱㄱ
	UPROPERTY(EditAnywhere, Category = "MonsterSkill")
	float SkillDMG = 0;

	UPROPERTY(EditAnywhere, Category = "MonsterTypeSetting")
	bool BossVision = false;//인식 범위 넓히기용

	UPROPERTY(EditAnywhere, Category = "DropItem", meta = (RowType = "/Script/KDT3D.ItemPackTableRow"))
	FDataTableRowHandle DropItem;

	//여기에 저장해서 사용하게 하고 끄집어 내서 사용 ㄱㄱ
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/KDT3D.SkillTableRow"))
	FDataTableRowHandle MonsterSkillDataTableRowHandle;
	//	이걸로 바꿈
	//	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/KDT3D.SkillTableRow"))
	//	FDataTableRowHandle DataTableRowHandle;

};



UCLASS()
class KDT3D_API ABossMonster : public APawn	
{
	GENERATED_BODY()
	
public:
	ABossMonster(const FObjectInitializer& ObjectInitializer);
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);
	virtual void SetSkillData(const FDataTableRowHandle& InSkillDataTableRowHandle);
protected:
	//Duplacte In Editor
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

	virtual void OnStun();
	virtual void OnAttack();
	virtual void OnSpawnBullet();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual float CheckPower();
	virtual void UseSkill();
	virtual void AddBuff(float BuffValue);

protected:
	//void CanUseSkill();
protected:
	float Cooltime = 0;
	float ChangeYawTime = 0;
	FRotator DefaultRotator = FRotator(0, 0, 0);

	UPROPERTY(BlueprintReadOnly)
	TArray<float> SkillPowerArray;
	UPROPERTY(BlueprintReadOnly)
	TArray<float> SkillCoolTimeArray;
	UPROPERTY(BlueprintReadOnly)
	TArray<float> SkillCurrentCoolTimeArray;
	UPROPERTY(BlueprintReadOnly)
	TArray<bool> CanUseSkillArray;


protected: // 옵션에 따라 생성되고 Root로 지정 됩니다.
	UPROPERTY()
	TObjectPtr<UShapeComponent> CollisionComponent;

	//없으면 제자리에 있게 하기
	UPROPERTY(EditAnywhere)
	TObjectPtr<APatrolPath> PatrolPathRef;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> HPWidgetComponent;

	//UPROPERTY(VisibleAnywhere)
	//TObjectPtr<UWeaponChildActorComponent> Weapon;

	//UPROPERTY(VisibleAnywhere)
	//UPaperSpriteComponent* MinimapSpriteComponent;

protected:
	UPROPERTY(VisibleAnywhere)
	UAdvanceFloatingPawnMovement* MovementComponent;

	UPROPERTY(VisibleAnywhere)
	UStatusComponent* StatusComponent;

	//UPROPERTY(VisibleAnywhere)
	//UCustomDioStatusComponent* StatusComponent;

	UPROPERTY(VisibleAnywhere)
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere)
	UAISenseConfig_Sight* AISenseConfig_Sight;

	UPROPERTY(VisibleAnywhere)
	UAnimInstance* AnimInstance;
#pragma region Replaced
	//UPROPERTY(VisibleAnywhere)
	//USkillComponent* MonsterSkillComponent;
#pragma endregion
	UPROPERTY(VisibleAnywhere)
	UMonsterSkillComponent* MonsterSkillComponent;

	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* PaperBurnEffectTimelineComponent;
	TArray<UMaterialInstanceDynamic*> MaterialInstanceDynamics;




protected:
	float MonsterDMG = 0;
	float MonsterHP = 0;
	float IntervalSkillTime = 0;

protected:
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/KDT3D.BossMonsterTableRow"))
	FDataTableRowHandle MonsterDataTableRowHandle;


	//TODO:: skilldata들어가는지 확인하고 만든 데이터 가져다가 사용하는 기능 ㄱㄱ
	// ++ Ai집어 넣어서 돌아가는지 확인 
	//UPROPERTY(EditAnywhere, meta = (RowType = "/Script/KDT3D.SkillTableRow"))
	//FDataTableRowHandle SkillDataTableRowHandle;

	FBossMonsterTableRow* MonsterData;

	FSkillTableRow* MonsterSkillData;

	UAnimMontage* CurrentDieMontage;
};
