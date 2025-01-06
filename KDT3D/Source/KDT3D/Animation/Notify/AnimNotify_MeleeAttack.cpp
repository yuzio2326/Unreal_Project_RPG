// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/AnimNotify_MeleeAttack.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "HOR/MeleeATK.h"
#include "Misc/Utils.h"
#include "Actors/Enemy/Enemy.h"
#include "HOR/Monster/BossMonster.h"

UAnimNotify_MeleeAttack::UAnimNotify_MeleeAttack()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(255, 255, 0, 255);
#endif // WITH_EDITORONLY_DATA
}

void UAnimNotify_MeleeAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	const FVector WeaponSocketLocation = MeshComp->GetSocketLocation(SocketName::Weapon);
	if (!DataTableRowHandle.DataTable) 
	{
		bool bCheckData = false;
	}



	AActor* OwningActor = MeshComp->GetOwner();
#if WITH_EDITOR
	if (GIsEditor && OwningActor && OwningActor->GetWorld() != GWorld) { return; } // 에디터 프리뷰
#endif
	APawn* OwningPawn = Cast<APawn>(OwningActor);
	check(OwningPawn);
	AEnemy* Enemy = Cast<AEnemy>(OwningPawn);
	
	ABossMonster* Monster = Cast<ABossMonster>(OwningPawn);
	
	const FName ProfileName = Enemy ? CollisionProfileName::Enemy : CollisionProfileName::Player;
	const FName ProfileNameMonster = Monster ? CollisionProfileName::Enemy : CollisionProfileName::Player;

	TArray<AActor*> IgnoreActors;
	FHitResult HitResult;


	if (Enemy->CheckPower() > 1) { Damage = Enemy->CheckPower(); }
	else if (Monster->CheckPower() > 1) { Damage = Enemy->CheckPower(); }
	else { Damage = 1; }


	if (UKismetSystemLibrary::SphereTraceSingleByProfile(MeshComp,
		WeaponSocketLocation, WeaponSocketLocation, 100.f,
		ProfileName, false, IgnoreActors, EDrawDebugTrace::None,
		HitResult, true, FLinearColor::Red, FLinearColor::Green, 1.f))
	{
		AActor* DamagedActor = HitResult.GetActor();
		UGameplayStatics::ApplyDamage(DamagedActor, Damage, OwningPawn->GetController(),
			OwningPawn, nullptr);
	}


}
