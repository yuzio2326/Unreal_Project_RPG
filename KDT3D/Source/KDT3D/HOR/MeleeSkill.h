// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HOR/SkillBase.h"
#include "MeleeSkill.generated.h"

/**
 * 
 */
USTRUCT()
struct KDT3D_API FMeleeSkillTableRow : public FSkillDataRow
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/KDT3D.MeleeATKTableRow"))
	FDataTableRowHandle MeleeATKRowHandle;
};

UCLASS()
class KDT3D_API AMeleeSkill : public ASkillBase
{
	GENERATED_BODY()
	

public:
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle) override;
	const FMeleeSkillTableRow* GetMeleeSkillTableRow() const { return MeleeSkillTableRow; }

protected:
	virtual void Tick(float DeltaTime) override;

	void UpdateSkills(const float DeltaTime);

	virtual void ActivateSkill() override;	//아직 작동 안함

protected:
	FRotator DesiredAimRotation;
	const FMeleeSkillTableRow* MeleeSkillTableRow = nullptr;

protected:
	bool bCanUseSkill = false;
	float MaxSkillCoolTime = 0;
	float MeleeCurrentSkillCoolTime = 0;

};
