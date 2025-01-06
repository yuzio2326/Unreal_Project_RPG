// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapon/Sword.h"

void ASword::OnMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (WeaponTableRow->WeaponAttackMontage[0] == Montage)
	{
		OwnerStatusComponent->SetAttack(false);
	}
}

void ASword::Attack()
{
	Super::Attack();

	OwnerStatusComponent->SetAttack(true);
}
