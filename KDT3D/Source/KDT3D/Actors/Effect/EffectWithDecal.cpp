// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Effect/EffectWithDecal.h"
#include "Subsystem/ActorPoolSubsystem.h"

AEffectWithDecal::AEffectWithDecal()
{
	HitDecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("Hit Decal"));
	HitDecalComponent->SetupAttachment(RootComponent);
}

void AEffectWithDecal::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	Super::SetData(InDataTableRowHandle);

	EffectWithDecalData = static_cast<FEffectWithDecalTableRow*>(EffectData);
	HitDecalComponent->SetMaterial(0, EffectWithDecalData->DecalMaterial);
	HitDecalComponent->SetRelativeTransform(EffectWithDecalData->DecalTransform);
}