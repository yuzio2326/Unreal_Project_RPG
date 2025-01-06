// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CustomSkillComponent.h"

// Sets default values for this component's properties
UCustomSkillComponent::UCustomSkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCustomSkillComponent::UseDashSkill()
{
	int a = 0;
}

void UCustomSkillComponent::SkillTimer_01()
{
}

void UCustomSkillComponent::SkillTimer_02()
{
}

void UCustomSkillComponent::SkillTimer_03()
{
}

void UCustomSkillComponent::SkillTimer_04()
{
}

void UCustomSkillComponent::SkillTimer_Z()
{
}

void UCustomSkillComponent::SkillTimer_X()
{
}

void UCustomSkillComponent::SkillTimer_C()
{
}

void UCustomSkillComponent::SkillTimer_V()
{
}

void UCustomSkillComponent::SkillTimer_Tab()
{
}



// Called when the game starts
void UCustomSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCustomSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

