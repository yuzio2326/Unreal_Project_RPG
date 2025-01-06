// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicCharacter.h"
#include "Components/NoFallCharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PaperSprite.h"

ABasicCharacter::ABasicCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UNoFallCharacterMovementComponent>(Super::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetCollisionProfileName(CollisionProfileName::Player);
	SpringArm = CreateDefaultSubobject<USoftWheelSpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Weapon = CreateDefaultSubobject<UWeaponChildActorComponent>(TEXT("Weapon"));
	{
		SpringArm->SetupAttachment(GetMesh());
		SpringArm->ProbeSize = 5.0;
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritRoll = false;
		SpringArm->SetMinMaxTargetArmLength(200.f, SpringArm->GetMaxTargetArmLength());
	}
	Camera->SetupAttachment(SpringArm);
	Weapon->SetupAttachment(GetMesh(), SocketName::Weapon);

	bUseControllerRotationYaw = false;

	//UCharacterMovementComponent* Movement = GetCharacterMovement();
	//Movement->bCanWalkOffLedges = false;
	const FRotator Rotation = FRotator(0., 90.0, 0.);
	const FVector Translation = FVector(0., 0., 90.0);
	FTransform SpringArmTransform = FTransform(Rotation, Translation, FVector::OneVector);
	SpringArm->SetRelativeTransform(SpringArmTransform);

	StatusComponent = CreateDefaultSubobject<UStatusComponent>(TEXT("StatusComponent"));

	// Minimap
	{
		MinimapSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MinimapSpriteComponent"));
		MinimapSpriteComponent->SetupAttachment(RootComponent);
		MinimapSpriteComponent->SetVisibleInSceneCaptureOnly(true);
		{
			FTransform Transform;
			Transform.SetLocation(FVector(0.0, 0.0, 300.0));
			Transform.SetRotation(FRotator(0.0, 90.0, -90.0).Quaternion());
			MinimapSpriteComponent->SetRelativeTransform(Transform);
		}
		{
			static ConstructorHelpers::FObjectFinder<UPaperSprite> Asset(TEXT("/Script/Paper2D.PaperSprite'/Game/Blueprint/Minimap/Player_Sprite.Player_Sprite'"));
			ensure(Asset.Object);
			MinimapSpriteComponent->SetSprite(Asset.Object);
		}
		{
			static ConstructorHelpers::FObjectFinder<UMaterial> Asset(TEXT("/Script/Engine.Material'/Game/Blueprint/Minimap/MyDefaultSpriteMaterial.MyDefaultSpriteMaterial'"));
			ensure(Asset.Object);
			MinimapSpriteComponent->SetMaterial(0, Asset.Object);
		}
	}
}

void ABasicCharacter::OnDie()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Pause(CurrentDieMontage);
}

// Called when the game starts or when spawned
void ABasicCharacter::BeginPlay()
{
	Super::BeginPlay();

	TSubclassOf<AActor> MinimapClass = LoadClass<AActor>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/Blueprint/Minimap/BP_Minimap.BP_Minimap_C'"));
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.Owner = this;
	AActor* MinimapActor = GetWorld()->SpawnActor<AActor>(MinimapClass, ActorSpawnParameters);
	//MinimapActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
}

void ABasicCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetData(DataTableRowHandle);
}

bool ABasicCharacter::CanJumpInternal_Implementation() const
{
	bool bJump = JumpIsAllowedInternal();
	if (!bJump && bIsCrouched)
	{
		bJump = true;
		auto& This = ConstCast(TObjectPtr<const ThisClass>(this));
		This->UnCrouch();
	}
	return bJump;
}

void ABasicCharacter::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FPawnTableRow* Data = DataTableRowHandle.GetRow<FPawnTableRow>(TEXT("Character"));
	if (!Data) { ensure(false); return; }

	CharacterData = Data;
	{
		UCharacterMovementComponent* Movement = GetCharacterMovement();
		//Movement->RotationRate = CharacterData->RotationRate;
		Movement->bOrientRotationToMovement = true;
		Movement->GetNavAgentPropertiesRef().bCanCrouch = true;
		Movement->MaxWalkSpeed = CharacterData->MovementMaxSpeed;

		const float NewCapsuleHalfHeight = CharacterData->CollisionCapsuleHalfHeight * 0.5f;
		Movement->SetCrouchedHalfHeight(NewCapsuleHalfHeight);
	}
	{
		UCapsuleComponent* Capsule = GetCapsuleComponent();
		if (!FMath::IsNearlyEqual(Capsule->GetUnscaledCapsuleHalfHeight(), CharacterData->CollisionCapsuleHalfHeight))
		{
			Capsule->SetCapsuleHalfHeight(CharacterData->CollisionCapsuleHalfHeight, false);
		}
	}
	{
		USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();
		SkeletalMeshComponent->SetSkeletalMesh(CharacterData->SkeletalMesh);
		SkeletalMeshComponent->SetRelativeTransform(CharacterData->MeshTransform);
		SkeletalMeshComponent->SetAnimClass(CharacterData->AnimClass);
	}
}

void ABasicCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	RecalculateBaseEyeHeight();
	FVector& MeshRelativeLocation = GetMesh()->GetRelativeLocation_DirectMutable();
	MeshRelativeLocation.Z = CharacterData->MeshTransform.GetLocation().Z + HalfHeightAdjust;
	BaseTranslationOffset.Z = MeshRelativeLocation.Z;

	K2_OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

void ABasicCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	RecalculateBaseEyeHeight();

	FVector& MeshRelativeLocation = GetMesh()->GetRelativeLocation_DirectMutable();
	MeshRelativeLocation.Z = CharacterData->MeshTransform.GetLocation().Z;
	BaseTranslationOffset.Z = MeshRelativeLocation.Z;

	K2_OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

// Called every frame
void ABasicCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABasicCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ABasicCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (StatusComponent->IsDie()) { return 0.f; }

	float DamageResult = StatusComponent->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (FMath::IsNearlyZero(DamageResult)) { return 0.0; }

	if (Controller)
	{
		Controller->StopMovement();
	}

	if (StatusComponent->IsDie() && !CharacterData->DieMontage.IsEmpty())
	{
		GetMovementComponent()->DestroyComponent();
		Weapon->DestroyComponent();
		/*if (Controller)
		{
			Controller->Destroy();
		}*/
		SetActorEnableCollision(false);

		const int64 Index = FMath::RandRange(0, CharacterData->DieMontage.Num() - 1);
		CurrentDieMontage = CharacterData->DieMontage[Index];

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(CurrentDieMontage);
		UKismetSystemLibrary::K2_SetTimer(this, TEXT("OnDie"),
			CharacterData->DieMontage[Index]->GetPlayLength() - 0.1f, false);
	}
	else if (!StatusComponent->IsDie() && !CharacterData->HitReactMontage.IsEmpty())
	{
		const int64 HitReactIndex = FMath::RandRange(0, CharacterData->HitReactMontage.Num() - 1);
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(CharacterData->HitReactMontage[HitReactIndex]);
	}

	return 0.0f;
}

