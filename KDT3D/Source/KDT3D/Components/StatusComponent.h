// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHPChanged, float, CurrentHP, float, MaxHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDie);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KDT3D_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusComponent();

	void SetAttack(const bool bFlag) { bAttack = bFlag; }
	bool CanMove() const { return !bAttack && !bDie; }
	bool IsDie() const { return bDie; }
	float GetHP() const { return HP; }
	float GetMaxHP() const { return MaxHP; }
	void StatusSetting(float SetMaxHP, float SetATKPower) { MaxHP = SetMaxHP, HP = MaxHP, ATKPower = SetATKPower; }
	void SetSkillDMG(int SKillNum, float SkillDMG) { SkillPowerArray[SKillNum] = SkillDMG; }
	void SetMonsterSkillCooltime(int SkillNum, float SkillCooltime) { SkillCoolTimeArray[SkillNum] = SkillCooltime; }
	void AddHP(float HPValue);
	bool GetStun() const { return bStun; }
	void SetStun(bool InStun) { bStun = InStun, StunValue = 0; }//오류시  = , StunValue = 0; 이거 없애기
	AController* GetLastInstigator() const { return LastInstigator; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

protected:
	bool bAttack = false;
	bool bDie = false;
	bool bStun = false;
	float MaxHP = 4.f;
	float HP = 4.f;
	float StunValue = 0;
	

protected:
	UPROPERTY(BlueprintReadOnly)
	float ATKPower = 1;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<float> SkillPowerArray;
	UPROPERTY(BlueprintReadOnly)
	TArray<float> SkillCoolTimeArray;
	UPROPERTY(BlueprintReadOnly)
	TArray<float> SkillCurrentCoolTimeArray;


public:
	AController* LastInstigator = nullptr;

	UPROPERTY(BlueprintAssignable)
	FOnHPChanged OnHPChanged;
	FOnDie OnDie;
};
