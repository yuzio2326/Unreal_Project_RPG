// Fill out your copyright notice in the Description page of Project Settings.


#include "HOR/PlayerDioController.h"
#include "Actors/PlayerCameraManager/BasicPlayerCameraManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Components/SoftWheelSpringArmComponent.h"
#include "CustomDioStatusComponent.h"		
//#include "DIOCharacter.h"
//#include "Components/CoolTimeComponent.h"			//Chage To DioStatus


APlayerDioController::APlayerDioController()
{
	{
		static ConstructorHelpers::FObjectFinder<UInputMappingContext> Asset
		{ TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Blueprint/01_PersonalProjects/Input/IMC_DIO.IMC_DIO'") };
		check(Asset.Object);

		IMC_DIO = Asset.Object;
	}

	PlayerCameraManagerClass = ABasicPlayerCameraManager::StaticClass();
	//bZoomWheel = false;
}

void APlayerDioController::BeginPlay()
{
	Super::BeginPlay();
	SpringArm = GetPawn()->GetComponentByClass<USoftWheelSpringArmComponent>();
	check(SpringArm);
	SpringArm->SetMinMaxTargetArmLength(160.f, 600.f);
	SpringArm->TargetArmLength = 450;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	Subsystem->AddMappingContext(IMC_DIO, 0);
}

void APlayerDioController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	ensure(EnhancedInputComponent);

	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_DIO, TEXT("IA_Move")))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::OnMove);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_Move is disabled"));
	}

	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_DIO, TEXT("IA_LookMouse")))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::OnLook);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_LookMouse is disabled"));
	}

	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_DIO, TEXT("IA_Jump")))
	{
		EnhancedInputComponent->BindAction(InputAction,	ETriggerEvent::Started, this, &ThisClass::OnDash);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_Jump is disabled"));
	}
	//Add Action	

	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_DIO, TEXT("IA_ATK_LB")))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &ThisClass::OffATK);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::OnATK);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_ATK_LB is disabled"));
	}
	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_DIO, TEXT("IA_ATK_RB")))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &ThisClass::OffSPATK);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::OnSPATK);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_ATK_RB is disabled"));
	}

	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_DIO, TEXT("IA_sk_Ty_01")))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::Use_Sk_01);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_sk_Ty_01 is disabled"));
	}

	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_DIO, TEXT("IA_sk_Ty_02")))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::Use_Sk_02);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_sk_Ty_02 is disabled"));
	}

	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_DIO, TEXT("IA_sk_Ty_03")))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::Use_Sk_03);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_sk_Ty_03 is disabled"));
	}

	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_DIO, TEXT("IA_sk_Ty_04")))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::Use_Sk_04);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_sk_Ty_04 is disabled"));
	}

	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_DIO, TEXT("IA_sk_Ty_Q")))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::Use_Sk_Q);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_sk_Ty_Q is disabled"));
	}

	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_DIO, TEXT("IA_sk_Ty_E")))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::Use_Sk_E);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_sk_Ty_E is disabled"));
	}

	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_DIO, TEXT("IA_sk_Ty_R")))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::Use_Sk_R);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_sk_Ty_R is disabled"));
	}


	if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_DIO, TEXT("IA_Enter")))
	{
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::Use_Revive);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_Enter is disabled"));
	}

}

void APlayerDioController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	StatusComponent = InPawn->GetComponentByClass<UCustomDioStatusComponent>();
	check(StatusComponent);

	SkillComponent = InPawn->GetComponentByClass<USkillComponent>();
	check(SkillComponent);
}

void APlayerDioController::OnMove(const FInputActionValue& InputActionValue)
{
	if (StatusComponent && !StatusComponent->CanMove()) { return; }	//StatusComponent를 custom으로

	const FVector2D ActionValue = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = K2_GetActorRotation();
	const FRotator RotationYaw = FRotator(0.0, Rotation.Yaw, 0.0);
	const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(RotationYaw);
	const FVector RightVector = UKismetMathLibrary::GetRightVector(RotationYaw);

	

	APawn* ControlledPawn = GetPawn();
	ControlledPawn->AddMovementInput(ForwardVector, ActionValue.X);
	ControlledPawn->AddMovementInput(RightVector, ActionValue.Y);
	
	float CheckMove = ActionValue.X + ActionValue.Y;

	if (FMath::IsNearlyZero(CheckMove))
		bMoving = true;

	if (StatusComponent->CanChangeDir())
	{
		StatusComponent->SetChangeDir(false);
		ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn());
		FVector MoveDir = ForwardVector * ActionValue.X + RightVector * ActionValue.Y;
		FRotator ActionRot = MoveDir.Rotation();
		ControlledCharacter->SetActorRotation(ActionRot);

	}
}

void APlayerDioController::OnLook(const FInputActionValue& InputActionValue)
{
	const FVector2D ActionValue = InputActionValue.Get<FVector2D>();

	AddYawInput(ActionValue.X);
	AddPitchInput(ActionValue.Y);
}

void APlayerDioController::OnDash(const FInputActionValue& InputActionValue)
{
	//Dash 입니다/
	if (StatusComponent && !StatusComponent->CanMove()) { return; }//StatusComponent를 custom으로
	//stamina 가 일정량 이하면 return	있으면 마이너스

	//statuscomp에서 바꾸고 dash 하면 player가 받아서 애니메이션 재생 ㄱㄱ
	if (StatusComponent->DashCheck()) { return; }

	if (StatusComponent->GetSP() < 20) { return; }
	APawn* ControlledPawn = GetPawn();

	const FVector2D ActionValue = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = K2_GetActorRotation();
	const FRotator RotationYaw = FRotator(0.0, Rotation.Yaw, 0.0);
	const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(RotationYaw);
	const FVector RightVector = UKismetMathLibrary::GetRightVector(RotationYaw);

	StatusComponent->SetChangeDir(true);
	FVector MoveDir = ForwardVector * ActionValue.X + RightVector * ActionValue.Y;
	FRotator ActionRot = MoveDir.Rotation();

	StatusComponent->UseMovingSkill(true);
	

#pragma region regercy_NotUse


	//if (StatusComponent && !StatusComponent->GetSP()) { return; }//StatusComponent를 custom으로
	//float PlayerSP = StatusComponent->GetSP();
	//if (PlayerSP <= 20) { return; }

	//if (StatusComponent->DashCheck()) { return; }	//대쉬 중이면 리턴 


	//ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn());

	//const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(MoveRotation);
	//const FVector RightVector = UKismetMathLibrary::GetRightVector(MoveRotation);
	//FVector MoveDir = ForwardVector * MoveValue.X + RightVector * MoveValue.Y;
	//FRotator ActionRot = MoveDir.Rotation();

	//ControlledCharacter->SetActorRotation(ActionRot);
	
	//FVector LaunchDirection = ControlledCharacter->GetActorForwardVector() * 3000;	//launch
	//ControlledCharacter->LaunchCharacter(LaunchDirection, true, true);

	//StatusComponent->UseDash();
#pragma endregion
	
}

void APlayerDioController::FastChageDirection(const FInputActionValue& InputActionValue)
{
	const FRotator Rotation = K2_GetActorRotation();
	const FRotator RotationYaw = FRotator(0.0, Rotation.Yaw, 0.0);
	const FVector2D ActionValue = InputActionValue.Get<FVector2D>();


	{
		ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn());
		const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(RotationYaw);
		const FVector RightVector = UKismetMathLibrary::GetRightVector(RotationYaw);
		FVector MoveDir = ForwardVector * ActionValue.X + RightVector * ActionValue.Y;
		FRotator ActionRot = MoveDir.Rotation();
		ControlledCharacter->SetActorRotation(ActionRot);
	}
}

void APlayerDioController::OnATK(const FInputActionValue& InputActionValue)
{
	if (StatusComponent && !StatusComponent->CanMove()) { return; }//StatusComponent를 custom으로
	if (SkillComponent->CheckUsingSkill()) { return; }

	StatusComponent->SetAttack(true);


	//어쨌든 status에서 조종하니까 그냥 값만 넘기고 player에서 montage를 조종하게 하자 나중에 바꾸기
	//bATK = true;
	//ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn());
	//check(ControlledCharacter);
	//USkeletalMeshComponent* SkeletalMeshComponent = ControlledCharacter->GetMesh();
	//check(SkeletalMeshComponent);
	//UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	//check(AnimInstance);


	
}

void APlayerDioController::OffATK(const FInputActionValue& InputActionValue)
{
	//if (StatusComponent && !StatusComponent->CanMove()) { return; }//StatusComponent를 custom으로

	StatusComponent->SetAttack(false);

	//bATK = false;
}

void APlayerDioController::OnSPATK(const FInputActionValue& InputActionValue)
{
	if (StatusComponent && !StatusComponent->CanMove()) { return; }//StatusComponent를 custom으로
	if (SkillComponent->CheckUsingSkill()) { return; }

	StatusComponent->SetSPAttack(true);
}

void APlayerDioController::OffSPATK(const FInputActionValue& InputActionValue)
{
	StatusComponent->SetSPAttack(false);
}

void APlayerDioController::Use_Sk_01(const FInputActionValue& InputActionValue)
{
	if (StatusComponent && !StatusComponent->CanMove()) { return; }//StatusComponent를 custom으로

	//direction Change Fast
	if (SkillComponent->CanUseSk(1))
	{
		FastChageDirection(InputActionValue);
		StatusComponent->AddMP(1);
	
		SkillComponent->UseSK_01();	//잠시 확인
	}



}

void APlayerDioController::Use_Sk_02(const FInputActionValue& InputActionValue)
{
	if (StatusComponent && !StatusComponent->CanMove()) { return; }//StatusComponent를 custom으로

	if (SkillComponent->CanUseSk(2))
	{
		FastChageDirection(InputActionValue);
		StatusComponent->AddMP(3);


		SkillComponent->UseSK_02();	//각 숫자에 맞는 스킬 ㄱㄱ
	}
}

void APlayerDioController::Use_Sk_03(const FInputActionValue& InputActionValue)
{
	if (StatusComponent && !StatusComponent->CanMove()) { return; }//StatusComponent를 custom으로

	if (SkillComponent->CanUseSk(3))
	{
		FastChageDirection(InputActionValue);
		if (StatusComponent->GetMP() < 2) { return; }

		StatusComponent->AddMP(-2);

		SkillComponent->UseSK_03();//각 숫자에 맞는 스킬 ㄱㄱ
	}
}

void APlayerDioController::Use_Sk_04(const FInputActionValue& InputActionValue)
{
	if (StatusComponent && !StatusComponent->CanMove()) { return; }//StatusComponent를 custom으로

	if (SkillComponent->CanUseSk(4))
	{
		FastChageDirection(InputActionValue);
		if (StatusComponent->GetMP() < 1) { return; }
		StatusComponent->AddMP(-1);

		SkillComponent->UseSK_04();	//각 숫자에 맞는 스킬 ㄱㄱ
	}
}

void APlayerDioController::Use_Sk_Q(const FInputActionValue& InputActionValue)
{
	if (StatusComponent && !StatusComponent->CanMove()) { return; }//StatusComponent를 custom으로

	if (SkillComponent->CanUseSk(5))
	{
		FastChageDirection(InputActionValue);

		if (StatusComponent->GetMP() < 2) { return; }
		StatusComponent->AddMP(-2);


		SkillComponent->UseSK_Q();	//각 숫자에 맞는 스킬 ㄱㄱ
	}
}

void APlayerDioController::Use_Sk_E(const FInputActionValue& InputActionValue)
{
	if (StatusComponent && !StatusComponent->CanMove()) { return; }//StatusComponent를 custom으로

	if (SkillComponent->CanUseSk(6))
	{
		FastChageDirection(InputActionValue);

		if (StatusComponent->GetMP() < 1) { return; }
		StatusComponent->AddMP(-1);


		SkillComponent->UseSK_E();	//각 숫자에 맞는 스킬 ㄱㄱ
	}
}

void APlayerDioController::Use_Sk_R(const FInputActionValue& InputActionValue)
{
	if (StatusComponent && !StatusComponent->CanMove()) { return; }//StatusComponent를 custom으로

	if (SkillComponent->CanUseSk(7))
	{
		FastChageDirection(InputActionValue);
		if (StatusComponent->GetMP() < 3) { return; }
		StatusComponent->AddMP(-3);


		SkillComponent->UseSK_R();	//각 숫자에 맞는 스킬 ㄱㄱ
	}
}

//안써도 될지도
void APlayerDioController::Use_Sk(int SkillNum)
{
	if (StatusComponent && !StatusComponent->CanMove()) { return; }//StatusComponent를 custom으로

	switch (SkillNum)
	{
	case 0:
		//if()	status 안에 mana 가 충분하고 skillcmpo 안에 cooltime이 돌았는지 확인
		//StatusComponent->UseSkill()	++ anim에 전달
	case 1:

	default:
		break;
	}
}

void APlayerDioController::Use_Revive(const FInputActionValue& InputActionValue)
{
	if (StatusComponent && !StatusComponent->IsDie()) { return; }
	StatusComponent->Revival();

}


