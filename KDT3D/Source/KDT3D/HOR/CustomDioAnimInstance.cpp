// Fill out your copyright notice in the Description page of Project Settings.


#include "HOR/CustomDioAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"
#include "CustomDioStatusComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCustomDioAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* Pawn = TryGetPawnOwner();
	if (GIsEditor && FApp::IsGame() && !Pawn)
	{
		checkf(false, TEXT("UCustomDioAnimInstance를 사용하려면 소유권자가 Pawn이여야 합니다."));
		return;
	}
	else if (!Pawn) { return; }

	PlayerMovementComponent = Pawn->GetMovementComponent();
	check(PlayerMovementComponent);

	StatusComponent = Pawn->GetComponentByClass<UCustomDioStatusComponent>();
	check(StatusComponent);

	//Skill정보 가져오기 나중에 
	//SkillComponent = Pawn->GetComponentByClass<USkillComponent>();
	//check(SkillComponent);

}

void UCustomDioAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!PlayerMovementComponent) { return; }

	Speed = UKismetMathLibrary::VSizeXY(PlayerMovementComponent->Velocity);

	bPlayerMove = !FMath::IsNearlyZero(Speed);


	if (!StatusComponent) { return; }
	
	bPlayerDash = StatusComponent->DashCheck();

	Dead = StatusComponent->IsDie();
	//if()

}
