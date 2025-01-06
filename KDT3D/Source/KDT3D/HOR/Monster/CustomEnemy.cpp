// Fill out your copyright notice in the Description page of Project Settings.


#include "HOR/Monster/CustomEnemy.h"

// Sets default values
ACustomEnemy::ACustomEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACustomEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACustomEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

