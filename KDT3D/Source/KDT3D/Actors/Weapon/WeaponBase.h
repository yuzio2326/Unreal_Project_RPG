// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Animation/BaisicCharacterAnimInstance.h"
#include "Components/StatusComponent.h"
#include "Misc/Utils.h"
#include "HOR/SkillBase.h"
#include "WeaponBase.generated.h"

class AWeaponBase;
USTRUCT()
struct KDT3D_API FWeaponTableRow : public FTableRowBase
{
	GENERATED_BODY()
	FWeaponTableRow();

public: // Base
	UPROPERTY(EditAnywhere, Category = "Weapon")
	USkeletalMesh* SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FTransform Transform = FTransform::Identity;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<AWeaponBase> WeaponClass;

public: // Animation
	UPROPERTY(EditAnywhere, Category = "Weapon|Animation")
	TSubclassOf<UBaisicCharacterAnimInstance> AnimClass;

	UPROPERTY(EditAnywhere, Category = "Weapon|Animation")
	TArray<UAnimMontage*> WeaponAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Enemy|Animation")
	TArray<UAnimMontage*> HitReactMontage;
};

UCLASS()
class KDT3D_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);

protected: // InputAction callback
	void OnFire(const FInputActionValue& InputActionValue);
	UFUNCTION(BlueprintCallable)
	virtual void Attack();

	UFUNCTION()
	virtual void OnMontageEnd(UAnimMontage* Montage, bool bInterrupted);

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponent;

protected: // Owner
	// 이 무기의 소유권자 Pawn
	APawn* OwningPawn = nullptr;
	// 이 무기의 소유권자 Pawn의 SkeletalMeshComponent로 캐릭터의 MeshComponent 같은 것을 의미
	USkeletalMeshComponent* OwningPawnBodyMesh = nullptr;

	UBaisicCharacterAnimInstance* BasicAnimInstance = nullptr;

	UPROPERTY()
	UStatusComponent* OwnerStatusComponent = nullptr;

protected:
	friend class UItemAction_Weapon;
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/KDT3D.WeaponTableRow"))
	FDataTableRowHandle DataTableRowHandle;

	const FWeaponTableRow* WeaponTableRow = nullptr;

	UPROPERTY(EditAnywhere, Category = "MonsterSkill")
	TArray<FSkillDataRow> SkillDataRow;		//스킬 하나씩 세팅 ㄱㄱ

	int AttackCount = 0;
protected: // Input
	class UInputMappingContext* IMC_Weapon = nullptr;
};
