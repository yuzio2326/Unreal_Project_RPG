// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomDioAnimInstance.h"
#include "CustomDioStatusComponent.h"
#include "Engine/DataAsset.h"
#include "Components/StaticMeshComponent.h"
#include "SkillBase.generated.h"


USTRUCT()		//나중에
struct KDT3D_API  FSkillDataRow : public FTableRowBase
{
	GENERATED_BODY()
	FSkillDataRow();

public:
	UPROPERTY(EditAnywhere, Category = "Skill")
	int UseSPNum;		//CanChange
	UPROPERTY(EditAnywhere, Category = "Skill")
	FTransform Transform = FTransform::Identity;
	//Delete 
	UPROPERTY(EditAnywhere, Category = "Skill")
	float Damage;		//Projectile같은 melee만들고 거기서 데미지랑 이펙트 넣고 딜 ㄱㄱ
	UPROPERTY(EditAnywhere, Category = "Skill")
	float SkillCoolTime;
	UPROPERTY(EditAnywhere, Category = "Skill")
	float CurrentSkillCoolTime;
	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<ASkillBase> SkillBaseClass;

	//SkillAnim
	//UPROPERTY(EditAnywhere, Category = "Weapon|Animation")
	//TSubclassOf<UCustomDioAnimInstance> AnimClass;
	UPROPERTY(EditAnywhere, Category = "SkillAnimation")
	TArray<UAnimMontage*> SkillAnimation;
	//UPROPERTY(EditAnywhere, Category = "Enemy|Animation")
	//TArray<UAnimMontage*> HitReactMontage;




};

UCLASS()
class KDT3D_API ASkillBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkillBase();
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);

protected:
	//UFUNCTION()
	//virtual void Attack();	//기본 공격

	//UFUNCTION()
	//virtual void OnMontageEnd(UAnimMontage* Montage, bool bInterrupted);

protected:
	//virtual void OnConstruction(const FTransform& Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 스킬 발동 (자식 클래스에서 재정의)
	virtual void ActivateSkill();
	// 쿨타임 관리
	void StartCooldown();

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* DefaultSceneRoot;

protected:
	UPROPERTY()
	UStaticMeshComponent* StaticMeshComponent;
	// 쿨타임 종료 처리
	virtual void OnCooldownEnd();

protected:
	APawn* OwningPawn = nullptr;
	USkeletalMeshComponent* OwningPawnBodyMesh = nullptr;
	UCustomDioAnimInstance* AnimInstance = nullptr;
	UPROPERTY()
	UCustomDioStatusComponent* OwnerStatusComponent = nullptr;


protected:	//나중에
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/KDT3D.SkillDataRow"))
	FDataTableRowHandle DataTableRowHandle;

	const FSkillDataRow* SkillDataRow = nullptr;
	
protected: // Input
	class UInputMappingContext* IMC_Skill = nullptr;

};
