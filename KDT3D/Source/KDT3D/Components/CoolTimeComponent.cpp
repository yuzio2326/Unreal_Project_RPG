// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CoolTimeComponent.h"

// Sets default values for this component's properties
UCoolTimeComponent::UCoolTimeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

float UCoolTimeComponent::SkillTimer(int SkillType, float SkillCooltime)
{
	return 0.0f;
}

bool UCoolTimeComponent::CheckUseSkill(int SkillType, float SkillCooltime)
{
	switch (SkillType)
	{
	case 0:

	default:
		break;
	}

	return false;
}

void UCoolTimeComponent::AddSkill(bool CanUseSkill, int SkillNum, float SkillCooltime)
{
	//Skill_Info* Skill_info;

	//Skill_info->bCanUseSkill = CanUseSkill;

}


// Called when the game starts
void UCoolTimeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCoolTimeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

