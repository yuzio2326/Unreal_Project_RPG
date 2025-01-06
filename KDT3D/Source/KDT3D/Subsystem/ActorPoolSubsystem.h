// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Misc/ActorPool.h"
#include "Actors/Effect/Effect.h"
#include "Actors/Effect/EffectWithDecal.h"
#include "ActorPoolSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class KDT3D_API UActorPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void SpawnEffect(const FTransform& SpawnTransform, const FDataTableRowHandle& InDataTableRowHandle);
	void SpawnHitEffectWithDecal(const FTransform& SpawnTransform, const FDataTableRowHandle& InDataTableRowHandle);

protected:
	/** Called when world is ready to start gameplay before the game mode transitions to the correct state and call BeginPlay on all actors */
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

protected:
	FCircularActorPool<AEffect> EffectPool;
	FCircularActorPool<AEffectWithDecal> HitEffectDecalPool;
};
