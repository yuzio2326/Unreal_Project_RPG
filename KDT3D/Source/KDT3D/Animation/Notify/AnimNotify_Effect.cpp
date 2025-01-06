// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/AnimNotify_Effect.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/Effect/Effect.h"
#include "Components/WeaponChildActorComponent.h"
#include "Subsystem/ActorPoolSubsystem.h"

UAnimNotify_Effect::UAnimNotify_Effect()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(255, 0, 0, 255);
#endif // WITH_EDITORONLY_DATA
}

void UAnimNotify_Effect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (EffectTableRowHandle.IsNull()) { return; }
	FEffectTableRow* Data = EffectTableRowHandle.GetRow<FEffectTableRow>(TEXT("Effect"));
	if (!Data) { ensure(false); return; }

	AActor* OwningActor = MeshComp->GetOwner();

#if WITH_EDITOR
	if (GIsEditor && OwningActor && OwningActor->GetWorld() != GWorld) { return; } // 에디터 프리뷰
#endif

	UWeaponChildActorComponent* WeaponChildActorComponent = OwningActor->GetComponentByClass<UWeaponChildActorComponent>();
	check(WeaponChildActorComponent);
	AActor* Weapon = WeaponChildActorComponent->GetChildActor();
	check(Weapon);
	USkeletalMeshComponent* WeaponSkeletalMeshComponent = Weapon->GetComponentByClass<USkeletalMeshComponent>();
	check(WeaponSkeletalMeshComponent);

	const FTransform MuzzleTransform = WeaponSkeletalMeshComponent->GetSocketTransform(SocketName);
	FTransform MuzzleNoScaleTransform;
	MuzzleNoScaleTransform.SetLocation(MuzzleTransform.GetLocation());
	MuzzleNoScaleTransform.SetRotation(MuzzleTransform.GetRotation());

	const FTransform EffectTransform = MuzzleNoScaleTransform;
	MeshComp->GetWorld()->GetSubsystem<UActorPoolSubsystem>()->SpawnEffect(EffectTransform, EffectTableRowHandle);
	//AEffect* Effect = MeshComp->GetWorld()->SpawnActorDeferred<AEffect>(Data->EffectClass,
	//	FTransform::Identity, Weapon);
	//Effect->SetData(EffectTableRowHandle);
	//Effect->FinishSpawning(EffectTransform);
	//Effect->Play();

}
