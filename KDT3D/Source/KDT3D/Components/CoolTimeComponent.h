// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoolTimeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KDT3D_API UCoolTimeComponent : public UActorComponent
{
	GENERATED_BODY()


public:	
	// Sets default values for this component's properties
	UCoolTimeComponent();

public:
	float SkillTimer(int SkillType, float SkillCooltime);
	bool CheckUseSkill(int SkillType, float SkillCooltime);
	void AddSkill(bool CanUseSkill, int SkillNum, float SkillCooltime);
	//void SetSkill(int SkillType, float skillCooltime);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



protected:
	//int skillType = 0;	//0=tab, 1~4 =1~4, 5~8 = z~v


	


};
