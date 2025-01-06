// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/DataAsset.h"
#include "Misc/Utils.h"
#include "DIOCharacter.h"
#include "PlayerDioController.generated.h"

/**
 * 
 */
UCLASS()
class KDT3D_API APlayerDioController : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerDioController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn);

protected:
	void OnMove(const FInputActionValue& InputActionValue);
	void OnLook(const FInputActionValue& InputActionValue);
	void OnDash(const FInputActionValue& InputActionValue);

	//void CanDash();
	void FastChageDirection(const FInputActionValue& InputActionValue);

	void OnATK(const FInputActionValue& InputActionValue);
	void OffATK(const FInputActionValue& InputActionValue);
	void OnSPATK(const FInputActionValue& InputActionValue);
	void OffSPATK(const FInputActionValue& InputActionValue);


	void Use_Sk_01(const FInputActionValue& InputActionValue);
	void Use_Sk_02(const FInputActionValue& InputActionValue);
	void Use_Sk_03(const FInputActionValue& InputActionValue);
	void Use_Sk_04(const FInputActionValue& InputActionValue);
	void Use_Sk_Q(const FInputActionValue& InputActionValue);
	void Use_Sk_E(const FInputActionValue& InputActionValue);
	void Use_Sk_R(const FInputActionValue& InputActionValue);
	void Use_Sk(int SkillNum);

	void Use_Revive(const FInputActionValue& InputActionValue);



protected:
	class USoftWheelSpringArmComponent* SpringArm = nullptr;
	UInputMappingContext* IMC_DIO = nullptr;



protected:
	UPROPERTY(BlueprintReadWrite)
	bool bZoomWheel = true;

	UPROPERTY()
	class UCustomDioStatusComponent* StatusComponent;
	UPROPERTY()
	class USkillComponent* SkillComponent;

	UPROPERTY()
	class ADIOCharacter* PlayerCharacterComponent;

	UPROPERTY()
	class UCoolTimeComponent* CoolTimeComponent;

	bool bMoving = false;
	bool bDash = false;

	//임시 변수들
	bool bATK = false;	//

};
