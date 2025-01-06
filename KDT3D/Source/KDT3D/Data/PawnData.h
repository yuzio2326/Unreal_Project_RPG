// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Misc/Utils.h"
#include "Animation/BaisicCharacterAnimInstance.h"
#include "HOR/CustomDioAnimInstance.h"
#include "AIController.h"
#include "PawnData.generated.h"

USTRUCT()
struct KDT3D_API FPawnTableRow : public FTableRowBase
{
	GENERATED_BODY()
	// 나중에 따로 만들어야 할거 같음.... 
public:
	UPROPERTY(EditAnywhere, Category = "Pawn")
	USkeletalMesh* SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Pawn")
	FTransform MeshTransform = FTransform::Identity;

	UPROPERTY(EditAnywhere, Category = "Pawn|Animation")
	TSubclassOf<UBaisicCharacterAnimInstance> AnimClass;

public:
	UPROPERTY(EditAnywhere, Category = "Pawn|Animation")
	TSubclassOf<UCustomDioAnimInstance> CustomAnimClass;

public:
	UPROPERTY(EditAnywhere, Category = "Pawn|Collision")
	TSubclassOf<UShapeComponent> CollisionClass = USphereComponent::StaticClass();
	UPROPERTY(EditAnywhere, Category = "Pawn|Collision")
	float CollisionSphereRadius = 32.f;
	UPROPERTY(EditAnywhere, Category = "Pawn|Collision")
	FVector CollisionBoxExtent = FVector(32.0, 32.0, 32.0);
	UPROPERTY(EditAnywhere, Category = "Pawn|Collision")
	float CollisionCapsuleRadius = 22.f;
	UPROPERTY(EditAnywhere, Category = "Pawn|Collision")
	float CollisionCapsuleHalfHeight = 44.f;

public: // Animation
	UPROPERTY(EditAnywhere, Category = "Pawn|Animation")
	TArray<UAnimMontage*> HitReactMontage;
	UPROPERTY(EditAnywhere, Category = "Pawn|Animation")
	TArray<UAnimMontage*> DieMontage;
	UPROPERTY(EditAnywhere, Category = "Pawn|Animation")
	TArray<UAnimMontage*> StunMontage;
	UPROPERTY(EditAnywhere, Category = "Pawn|Animation")
	TArray<UAnimMontage*> BoundMontage;

	//Can Control Anim
	UPROPERTY(EditAnywhere, Category = "Pawn|Animation")
	TArray<UAnimMontage*> AttackMontage;
	UPROPERTY(EditAnywhere, Category = "Pawn|Animation")
	TArray<UAnimMontage*> DashMontage;
	UPROPERTY(EditAnywhere, Category = "Pawn|Animation")
	TArray<UAnimMontage*> SkillMontage;

	UPROPERTY(EditAnywhere, Category = "Skill", meta = (RowType = "/Script/KDT3D.SkillDataRow"))
	FDataTableRowHandle SkillData;


public: // Movement
	UPROPERTY(EditAnywhere, Category = "Pawn|Movement")
	float MovementMaxSpeed = 400.f;

public: // AI (Enemy만 적용)
	UPROPERTY(EditAnywhere, Category = "Pawn|AI")
	TSubclassOf<AAIController> AIControllerClass;

	UPROPERTY(EditAnywhere, Category = "Pawn|Status")
	float HP = 0;
	UPROPERTY(EditAnywhere, Category = "Pawn|Status")
	float DMG = 0;
	UPROPERTY(EditAnywhere, Category = "Pawn|Status")
	TArray<float> SkillDMGArray;


public: // Weapon
	UPROPERTY(EditAnywhere, Category = "Pawn|Weapon", meta = (RowType="/Script/KDT3D.WeaponTableRow"))
	FDataTableRowHandle Weapon;
};