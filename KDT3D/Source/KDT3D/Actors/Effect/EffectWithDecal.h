// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Effect/Effect.h"
#include "Components/DecalComponent.h"
#include "EffectWithDecal.generated.h"

USTRUCT()
struct KDT3D_API FEffectWithDecalTableRow : public FEffectTableRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Effect|Decal")
	FTransform DecalTransform;
	UPROPERTY(EditAnywhere, Category = "Effect|Decal")
	UMaterial* DecalMaterial;
};

UCLASS()
class KDT3D_API AEffectWithDecal : public AEffect
{
	GENERATED_BODY()
public:
	AEffectWithDecal();
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle) override;

protected:
	UPROPERTY(VisibleAnywhere)
	UDecalComponent* HitDecalComponent;

protected:
	FEffectWithDecalTableRow* EffectWithDecalData;
};
