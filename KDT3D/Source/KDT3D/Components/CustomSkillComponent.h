// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CustomSkillComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KDT3D_API UCustomSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCustomSkillComponent();

	void UseDashSkill();

	void SkillTimer_01();
	void SkillTimer_02();
	void SkillTimer_03();
	void SkillTimer_04();
	void SkillTimer_Z();
	void SkillTimer_X();
	void SkillTimer_C();
	void SkillTimer_V();
	void SkillTimer_Tab();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:
	bool bATK_LB = false;
	bool bATK_RB = false;
	bool bSk_01 = false;
	bool bSk_02 = false;
	bool bSk_03 = false;
	bool bSk_04 = false;
	bool bSk_Tab = false;
	bool bSk_Z = false;
	bool bSk_X = false;
	bool bSk_C = false;
	bool bSk_V = false;

	float fCool_Sk_01 = 0.f;
	float fCool_Sk_02 = 0.f;
	float fCool_Sk_03 = 0.f;
	float fCool_Sk_04 = 0.f;
	float fCool_Sk_Tab = 0.f;
	float fCool_Sk_Z = 0.f;
	float fCool_Sk_X = 0.f;
	float fCool_Sk_C = 0.f;
	float fCool_Sk_V = 0.f;


		
};
