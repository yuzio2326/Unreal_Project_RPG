// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Base.h"

UBTTask_Base::UBTTask_Base(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UBTTask_Base::SetOwner(AActor* InActorOwner)
{
	ActorOwner = InActorOwner;
	AIOwner = Cast<AAIController>(InActorOwner);
}