// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/BaisicCharacterAnimInstance.h"
#include "CustomDioAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class KDT3D_API UCustomDioAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void SetAimRotation(const FRotator& NewAimRotation) { PlayerAimRotation = NewAimRotation; }
	const FRotator& GetAimRotation() { return PlayerAimRotation; }

protected:
	UPROPERTY()
	class UPawnMovementComponent* PlayerMovementComponent;

	UPROPERTY(BlueprintReadOnly)
	double Speed = 0.0;

	UPROPERTY(BlueprintReadOnly)
	bool bPlayerMove = false;

	UPROPERTY(BlueprintReadOnly)
	FRotator PlayerAimRotation;	//For Target Skills

protected:
	UPROPERTY()
	class UCustomDioStatusComponent* StatusComponent;
	UPROPERTY()
	class USkillComponent* SkillComponent;

	UPROPERTY(BlueprintReadOnly)
	bool bPlayerDash = false;

	UPROPERTY(BlueprintReadOnly)
	bool Dead = false;
protected:
	UPROPERTY(BlueprintReadOnly)
	bool bUseSkill = false;	//control or player 에서 AnimInstance 변수 바꿀수 있게 ㄱㄱ



};
