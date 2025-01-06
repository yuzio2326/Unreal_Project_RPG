// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/AnimNotify_SpawnProjectile.h"
#include "Actors/Projectile/Projectile.h"
#include "Actors/Weapon/Gun.h"
#include "Components/WeaponChildActorComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/Utils.h"

UAnimNotify_SpawnProjectile::UAnimNotify_SpawnProjectile()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(90, 90, 200, 255);
#endif // WITH_EDITORONLY_DATA
}

FRotator UAnimNotify_SpawnProjectile::GetPlayerProjectileRotation(const FVector& InProjectSpawnLocation, UCameraComponent* InCameraComponent)
{
	const FVector CameraForwardVector = InCameraComponent->GetForwardVector();
	const FVector DestinationLocation = InCameraComponent->GetComponentLocation() + CameraForwardVector * 5000.0;

	TArray<AActor*> IgnoreActors; IgnoreActors.Add(InCameraComponent->GetOwner());
	FHitResult HitResult;

	const ETraceTypeQuery TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel4);
	const bool bHit = UKismetSystemLibrary::LineTraceSingle(InCameraComponent->GetWorld(),
		InCameraComponent->GetComponentLocation(), DestinationLocation, TraceTypeQuery,
		false, IgnoreActors, EDrawDebugTrace::None, HitResult, true);

	FRotator Rotator;
	if (bHit)
	{
		Rotator = UKismetMathLibrary::FindLookAtRotation(InProjectSpawnLocation, HitResult.ImpactPoint);
	}
	else
	{
		Rotator = UKismetMathLibrary::FindLookAtRotation(InProjectSpawnLocation, DestinationLocation);
	}
	return Rotator;
}

void UAnimNotify_SpawnProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
#if WITH_EDITOR
	if (GIsEditor && MeshComp->GetWorld() != GWorld) { return; } // 에디터 프리뷰
#endif

	APawn* OwningPawn = Cast<APawn>(MeshComp->GetOwner());
	check(OwningPawn);

	UWeaponChildActorComponent* WeaponChildActorComponent = OwningPawn->GetComponentByClass<UWeaponChildActorComponent>();
	check(WeaponChildActorComponent);
	AActor* WeaponActor = WeaponChildActorComponent->GetChildActor();
	check(WeaponActor);

 	USkeletalMeshComponent* WeaponSkeletalMeshComponent = WeaponActor->GetComponentByClass<USkeletalMeshComponent>();
	check(WeaponSkeletalMeshComponent);
	const FVector MuzzleLocation = WeaponSkeletalMeshComponent->GetSocketLocation(SocketName::Muzzle);
#if WITH_EDITOR
	USkeletalMeshSocket const* SkeletalMeshSocket = WeaponSkeletalMeshComponent->GetSocketByName(SocketName::Muzzle);
	check(SkeletalMeshSocket);
#endif

	FRotator ProjectileRotator = FRotator::ZeroRotator;
	bool bIsPlayer = false;
	if (UCameraComponent* CameraComponent = OwningPawn->GetComponentByClass<UCameraComponent>())
	{
		bIsPlayer = true;
		ProjectileRotator = GetPlayerProjectileRotation(MuzzleLocation, CameraComponent);
	}
	else
	{
		// 구현 필요
		check(false);
	}
	
	AGun* GunActor = Cast<AGun>(WeaponActor);
	check(GunActor);
	const FGunTableRow* GunTableRow = GunActor->GetGunTableRow();
	const FProjectileTableRow* ProjectileTableRow = GunTableRow->ProjectileRowHandle.GetRow<FProjectileTableRow>(TEXT("Projectile"));
	check(ProjectileTableRow);

	UWorld* World = OwningPawn->GetWorld();
	AProjectile* Projectile = World->SpawnActorDeferred<AProjectile>(ProjectileTableRow->ProjectileClass,
		FTransform::Identity, OwningPawn, OwningPawn, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	Projectile->SetData(GunTableRow->ProjectileRowHandle);

	FTransform NewTransform;
	NewTransform.SetLocation(MuzzleLocation);
	NewTransform.SetRotation(ProjectileRotator.Quaternion());
	Projectile->FinishSpawning(NewTransform);
}
