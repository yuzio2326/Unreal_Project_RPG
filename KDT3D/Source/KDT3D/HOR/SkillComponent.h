// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include"SkillBase.h"
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"


USTRUCT()	//Skill관리용
struct KDT3D_API FSkillTableRow : public FTableRowBase
{
	GENERATED_BODY()
public: // Skill
	//UPROPERTY(EditAnywhere, Category = "Skill", meta = (RowType = "/Script/KDT3D.SkillDataRow"))
	//FDataTableRowHandle Skills;

	//Melee
	UPROPERTY(EditAnywhere, Category = "Skill", meta = (RowType = "/Script/KDT3D.SkillDataRow"))
	TArray<FSkillDataRow> SkillArray;

	UPROPERTY(EditAnywhere, Category = "Skill", meta = (RowType = "/Script/KDT3D.SkillDataRow"))
	TArray<FSkillDataRow> RangedSkillArray;//원거리

	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/KDT3D.ProjectileTableRow"))
	FDataTableRowHandle ProjectileRowHandle; // 원거리 공격 투사체

	UPROPERTY(EditAnywhere, Category = "MonsterBasicAttack")
	TArray<UAnimMontage*> AttackAnimation;
	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data")
	FName SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data")
	float UseMPNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data")
	float SkillCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data")
	UAnimMontage* SkillAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data")
	TSubclassOf<AActor> SkillActorClass;

	*/

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerUseSkill01, float, CurrentCooltime01, float, MaxCooltime01);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerUseSkill02, float, CurrentCooltime02, float, MaxCooltime02);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerUseSkill03, float, CurrentCooltime03, float, MaxCooltime03);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerUseSkill04, float, CurrentCooltime04, float, MaxCooltime04);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerUseSkillQ, float, CurrentCooltimeQ, float, MaxCooltimeQ);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerUseSkillR, float, CurrentCooltimeE, float, MaxCooltimeE);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerUseSkillE, float, CurrentCooltimeR, float, MaxCooltimeR);
/*
* 눌렀을때 알파값 높여서 눌렀다는걸 확인 시켜주기
* 쿨타임이면 배경색 보이게 ㄱㄱ
* 
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KDT3D_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void CooltimeSet(float DeltaTime);
public:
	void InitializeSkillData(UDataTable* SkillDataTable);
	void SetData(FDataTableRowHandle InDataTableRowHandle);

	void UseSkill(int Skill_Index);


public:
	bool CanUseSk(int SkillNum);
	void UseSK_01();
	void UseSK_02();
	void UseSK_03();
	void UseSK_04();
	void UseSK_Q();
	void UseSK_E();
	void UseSK_R();
	bool CheckUsingSkill() { return UsingSkill; }
	int CurrentAnim() { return CurrentAnimIndex; }
private:
	float CurrentPlaytime = 0;
	float MaxSk01Cooltime = 0;		//data로 안들어감
	float MaxSk02Cooltime = 0;
	float MaxSk03Cooltime = 0;
	float MaxSk04Cooltime = 0;
	float MaxSkQCooltime = 0;
	float MaxSkECooltime = 0;
	float MaxSkRCooltime = 0;
	float Sk01Cooltime = 0;		//data로 안들어감
	float Sk02Cooltime = 0;
	float Sk03Cooltime = 0;
	float Sk04Cooltime = 0;
	float SkQCooltime = 0;
	float SkECooltime = 0;
	float SkRCooltime = 0;
	

	bool Findingbool = false;
	bool bCanUseskill01 = false;
	bool bCanUseskill02 = false;
	bool bCanUseskill03 = false;
	bool bCanUseskill04 = false;
	bool bCanUseskillQ = false;
	bool bCanUseskillE = false;
	bool bCanUseskillR = false;

protected:
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/KDT3D.SkillTableRow"))
	FDataTableRowHandle DataTableRowHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Data")
	FSkillTableRow SkillTableRow;

	FSkillDataRow* SkillDataRow;

	UPROPERTY(BlueprintReadOnly)
	int CurrentAnimIndex = 0;
	UPROPERTY(BlueprintReadOnly)
	bool UsingSkill = false;

	FOnPlayerMPChanged OnPlayerMPChanged;
	UCustomDioStatusComponent* StatusComponent;
protected:
	UPROPERTY(BlueprintAssignable)
	FOnPlayerUseSkill01 OnPlayerUseSkill01;
	UPROPERTY(BlueprintAssignable)
	FOnPlayerUseSkill02 OnPlayerUseSkill02;
	UPROPERTY(BlueprintAssignable)
	FOnPlayerUseSkill03 OnPlayerUseSkill03;
	UPROPERTY(BlueprintAssignable)
	FOnPlayerUseSkill04 OnPlayerUseSkill04;
	UPROPERTY(BlueprintAssignable)
	FOnPlayerUseSkillQ OnPlayerUseSkillQ;
	UPROPERTY(BlueprintAssignable)
	FOnPlayerUseSkillE OnPlayerUseSkillE;
	UPROPERTY(BlueprintAssignable)
	FOnPlayerUseSkillR OnPlayerUseSkillR;



private:		//SkillCooltime이 안돌아감 이상하게 꼬임 일단 하드코딩 ㄱㄱ
	int iSkillNum = 0;




public:
	//UPROPERTY(EditAnywhere, Category = "Skills")
	//TArray<TSubclassOf<ASkillBase>> SkillClasses;
		
};
