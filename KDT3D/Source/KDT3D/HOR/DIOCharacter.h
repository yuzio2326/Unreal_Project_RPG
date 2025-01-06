// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SoftWheelSpringArmComponent.h"
#include "CustomDioStatusComponent.h"
#include "SkillComponent.h"		//player에서 MP소모값 처리할거임 
#include "CustomDioAnimInstance.h"
#include "Data/PawnData.h"
#include "DIOCharacter.generated.h"

UCLASS()
class KDT3D_API ADIOCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADIOCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UFUNCTION()
	virtual void OnDie();
	UFUNCTION()
	virtual void OffDash();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;


protected:
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual float UseSkill(float DeltaTime);
	virtual float UseMovingSkill();
	virtual void Attack();
	virtual void SPAttack();
	virtual void OnStun();
	virtual void OnBound();

protected:
	void Use_Sk_01();


private:
	bool Dashed = false;
	bool Attacked = false;
	float ActivatingTime=0;
	float AttackingTime =0;
	float SPAttackingTime =0;
	int AnimNum = 0;

protected:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USoftWheelSpringArmComponent> SpringArm;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;


	//나중에 만들기
	UPROPERTY(VisibleAnywhere)
	UCustomDioStatusComponent* StatusComponent;

	UPROPERTY(VisibleAnywhere)
	USkillComponent* SkillComponent;

	FOnPlayerMPChanged OnPlayerMPChanged;
	//UPROPERTY(VisibleAnywhere)
	//UPaperSpriteComponent* MinimapSpriteComponent;



protected:
	UAnimMontage* CurrentDieMontage;
	UAnimMontage* CurrentMontage;

	UCustomDioAnimInstance* PlayerAnimInstance;

protected:
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/KDT3D.PawnTableRow"))
	FDataTableRowHandle DataTableRowHandle;

	FPawnTableRow* CharacterData;


};
