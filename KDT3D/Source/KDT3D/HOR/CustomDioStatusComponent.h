// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CustomDioStatusComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerHPChanged, float, CurrentHP, float, MaxHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerMPChanged, float, CurrentMP, float, MaxMP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerSPChanged, float, CurrentSP, float, MaxSP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUseSkill, int, SkillNum, bool, bUseSkill);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDMGChanged, float, CurrentDMG);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDead, bool, PlayerDead);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KDT3D_API UCustomDioStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCustomDioStatusComponent();

	void SetAttack(const bool bFlag) { bAttack = bFlag; }
	bool GetAttacking() const {return bAttack; }
	void SetSPAttack(const bool bFlag) { bSPAttack = bFlag; }
	bool GetSPAttacking() const {return bSPAttack; }
	bool CanMove() const { return !bAttack && !bDead && !bBound && !bStun; }
	void SetChangeDir(const bool bAimFlag) { bAim = bAimFlag; }
	bool CanChangeDir() const { return bAim; }
	bool IsDie() const { return bDead; }
	void SetBound(const bool ChangeBound) { bBound = ChangeBound; }
	bool IsBound() const { return bBound; }
	float GetHP() const { return HP; }
	float GetMP() const { return MP; }		//마나 역할
	float GetSP() const { return SP; }		//스테미나	1dash -20 씩
	float GetDMG() const { return DMG; }		//Rush 아덴
	void  AddHP(float HPValue);
	void  AddMaxHP(float MaxHPValue) { MaxHP += MaxHPValue; }
	void  AddMP(float MPValue);
	void  AddSP(float SPValue);
	void  AddDMG(float DMGValue);
	void StatusSetting(float SetMaxHP, float SetATKPower) { MaxHP = SetMaxHP, HP = SetMaxHP, DMG = SetATKPower; }
	bool IsBounding(UAnimMontage* MontageToPlay);//Bound에만 쓰세요 (MontagePlaying으로 쓰기 ㄱㅊ은데 이미 이렇게 만듦..
	//void WinGame(bool WinGame) { WinThisGame = WinGame; }

	AController* GetLastInstigator() const { return LastInstigator; }



	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);

//public:
	


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	//virtual float UseMovingSkill();	//Montage
	//virtual void UseSkill(int SkillNum, bool bUseSkill);


public:
	bool DashCheck() { return bDashing; }
	bool UseMovingSkill(bool UseDash);
	void MovingSkill();
	void AddMaxStatus();
	void Revival();

protected:
	bool bAttack = false;
	bool bSPAttack = false;	//특수공격
	bool bUseSkill = false; //Player에서 바꿀거임
	bool bAim = false;
	UPROPERTY(BlueprintReadOnly)
	bool bDead = false;
	bool bStun = false;
	bool bBound = false;
	float BoundValue = 0;
	UPROPERTY(BlueprintReadOnly)
	float MaxHP = 200.f;
	UPROPERTY(BlueprintReadOnly)
	float HP = 200.f;
	float MaxMP = 10;	//마나 역할
	float MP = 10;
	UPROPERTY(BlueprintReadOnly)
	float MaxSP = 100;	//스테미나	1dash -20 씩
	UPROPERTY(BlueprintReadOnly)
	float SP = 100;
	//UPROPERTY(VisibleAnywhere)
	UPROPERTY(BlueprintReadOnly)
	float DMG = 30;
	//UPROPERTY(BlueprintReadOnly)
	//bool WinThisGame = false;

protected:
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/KDT3D.PawnTableRow"))
	FDataTableRowHandle DataTableRowHandle;

protected:
	bool bDashing = false;
	bool bDashed = false;

	float Dashtime = 0;

public:
	AController* LastInstigator = nullptr;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerHPChanged OnPlayerHPChanged;
	UPROPERTY(BlueprintAssignable)
	FOnPlayerSPChanged OnPlayerSPChanged;
	UPROPERTY(BlueprintAssignable)
	FOnPlayerMPChanged OnPlayerMPChanged;
	UPROPERTY(BlueprintAssignable)
	FOnPlayerDMGChanged OnPlayerDMGChanged;

	FOnDead OnDead;

	FOnUseSkill OnUseSkill;

	FTimerHandle TimerHandle;

};
