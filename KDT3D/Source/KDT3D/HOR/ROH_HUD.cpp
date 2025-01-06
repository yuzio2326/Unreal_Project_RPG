// Fill out your copyright notice in the Description page of Project Settings.

#include "HOR/ROH_HUD.h"
#include "HOR/ROH_HUDPlayerUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "HOR/PlayerDioController.h"

AROH_HUD::AROH_HUD()
{

}

void AROH_HUD::AddItemPack(AItemPack* ItemPack)
{
	Widget->AddItemPack(ItemPack);
}

void AROH_HUD::RemoveItemPack(AItemPack* ItemPack)
{
	Widget->RemoveItemPack(ItemPack);
}

void AROH_HUD::RepeatItemPack(AItemPack* ItemPack)
{
	Widget->RepeatItemPack(ItemPack);
}

void AROH_HUD::BeginPlay()
{
	Super::BeginPlay();
	
	UClass* WidgetClass = LoadClass<UROH_HUDPlayerUserWidget>(nullptr,
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/01_PersonalProjects/98_UIs/UI_ROH.UI_ROH_C'"));
	check(WidgetClass);
	Widget = CreateWidget<UROH_HUDPlayerUserWidget>(GetWorld(), WidgetClass);
	Widget->AddToViewport();

	PlayerController = Cast<APlayerDioController>(UGameplayStatics::GetPlayerController(this, 0));
	check(PlayerController);

	/*
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		//Subsystem->AddMappingContext(IMC_Item, 0);
		EnableInput(PlayerController);
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
		ensure(EnhancedInputComponent);
		//if (const UInputAction* InputAction = FUtils::GetInputActionFromName(IMC_Item, TEXT("IA_PickupItem")))
		//{
		//	EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::OnPickupItem);
		//}
	}
	*/
	

}

void AROH_HUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (APawn* Pawn = PlayerController->GetPawn())
	{
		TArray<AActor*> Temp;
		Pawn->GetOverlappingActors(Temp, AItemPack::StaticClass());

		if (Temp != OverlappedDroppedItems)
		{
			for (AActor* It : OverlappedDroppedItems)
			{
				RemoveItemPack(static_cast<AItemPack*>(It));
			}
			OverlappedDroppedItems = MoveTemp(Temp);

			for (AActor* It : OverlappedDroppedItems)
			{
				AddItemPack(static_cast<AItemPack*>(It));
			}
		}
	}

}
