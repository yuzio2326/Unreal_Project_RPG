// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Weapon/WeaponBase.h"
#include "Gun.generated.h"

USTRUCT()
struct KDT3D_API FGunTableRow : public FWeaponTableRow
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/KDT3D.ProjectileTableRow"))
	FDataTableRowHandle ProjectileRowHandle;
};

UCLASS()
class KDT3D_API AGun : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle) override;
	const FGunTableRow* GetGunTableRow() const { return GunTableRow; }

protected:
	virtual void Tick(float DeltaTime) override;

	void UpdateDesiredAimRotation(const float DeltaTime);

protected:
	FRotator DesiredAimRotation;
	const FGunTableRow* GunTableRow = nullptr;
};
