// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HOR/SkillComponent.h"
#include "Components/AdvanceFloatingPawnMovement.h"
#include "MonsterSkillComponent.generated.h"




USTRUCT()
struct KDT3D_API FMonsterSkillTableRow : public FTableRowBase
{
	GENERATED_BODY()
	//Skill 없으면 그냥 Pawn 사용 해도 되지만 그냥 저 테이블 가져다가 붙여 쓰는게 나을듯함

public:
	UPROPERTY(EditAnywhere, Category = "MonsterSkill")
	TArray<FSkillDataRow> SkillDataRow;		//스킬 하나씩 세팅 ㄱㄱ



};

//	스킬 사용중이면 true 하고 해당 값이 모두 사용 했으면 true ㄱㄱ 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUsingSkill, bool, UsingSkill, bool, CanUseSkill, bool, IsRangeSkill);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KDT3D_API UMonsterSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMonsterSkillComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void InitializeSkillData(UDataTable* SkillDataTable);
	void SetData(FDataTableRowHandle InDataTableRowHandle);


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual void UseRangedSkill();
	UFUNCTION(BlueprintCallable)
	virtual void UseSkill();

	virtual void ActivateSkill(int ActivateCurrentAnim);

	virtual void AddMovement(float DeltaTime, int MovementType, float Delay);
	
	//Skill을 사용할수 없을때 사용 all SkillCoolTime 있거나 내부 쿨일 경우에
	//음... boss Monster에 만들어야 하나?
	UFUNCTION(BlueprintCallable)	
	virtual void Attack();



protected:
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/KDT3D.SkillTableRow"))
	FDataTableRowHandle DataTableRowHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Data")
	FSkillTableRow SkillTableRow;

	FMonsterSkillTableRow MonsterSkillTableRow;


protected:
	USkeletalMeshComponent* SkeletalMeshComponent;
	UAnimInstance* AnimInstance;
	UAdvanceFloatingPawnMovement* MovementComponent;

protected:
	TArray<int> SkillAnimNum;
	int CurrentAnimCount;
	int AttackCount = 0;
	int SkillCount = 0;
	int RangeSkillCount = 0;
	bool UsingSkill = false;
	TArray<float> SkillCooldowns;
	TArray<float> RangedSkillCooldowns;
	bool ISAllSkillCooltime = false;
	bool ISRangedSkillCooltime = false;
	bool ISMeleeSkillCooltime = false;

	float fMoveTime = 0;
	int iMovementType = 0;
	float fDelay = 0;

public:
	UPROPERTY(BlueprintAssignable)
	FOnUsingSkill OnUsingSkill;

};
