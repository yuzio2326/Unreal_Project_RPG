// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Misc/Utils.h"
#include "AnimNotify_Effect.generated.h"

class USoundBase;
class UParticleSystem;

UCLASS()
class KDT3D_API UAnimNotify_Effect : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_Effect();

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/KDT3D.EffectTableRow"))
	FDataTableRowHandle EffectTableRowHandle;

	// SocketName to attach to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	FName SocketName = SocketName::Muzzle;
};
