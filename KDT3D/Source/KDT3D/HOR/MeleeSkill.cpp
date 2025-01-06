// Fill out your copyright notice in the Description page of Project Settings.


#include "HOR/MeleeSkill.h"

void AMeleeSkill::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	Super::SetData(InDataTableRowHandle);

	MeleeSkillTableRow = static_cast<const FMeleeSkillTableRow*>(SkillDataRow);
}

void AMeleeSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateSkills(DeltaTime);
}

void AMeleeSkill::UpdateSkills(const float DeltaTime)
{

	MaxSkillCoolTime = MeleeSkillTableRow->SkillCoolTime;

	if (MeleeCurrentSkillCoolTime <= 0)	{bCanUseSkill = true;}
	else	
		MeleeCurrentSkillCoolTime -= DeltaTime;

	if (!bCanUseSkill)//skillcomp에서 false로 만들면 canUse를 false로 ㄱㄱ
	{
		//skillcomp에서 false가 되면 얘 호출 순서 위로 올리기
		MeleeCurrentSkillCoolTime = MaxSkillCoolTime;
	}


}

void AMeleeSkill::ActivateSkill()
{
	//StatusComponent->GetSP();

	StartCooldown();
}

