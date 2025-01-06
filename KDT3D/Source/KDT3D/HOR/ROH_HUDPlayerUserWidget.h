// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "ROH_HUDPlayerUserWidget.generated.h"

/**
 * 
 */
class AItemPack;

UCLASS()
class KDT3D_API UROH_HUDPlayerUserWidget : public UUserWidget
{
	GENERATED_BODY()

	friend class AROH_HUD;

protected:
	void AddItemPack(AItemPack* ItemPack);
	void RemoveItemPack(AItemPack* ItemPack);
	void RepeatItemPack(AItemPack* ItemPack);


protected:
				//TODO:: 얘 주석 해제하고 관련된거 만들기
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "AddItemPack"))
	void ReceiveAddItemPack();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "RemoveItemPack"))
	void ReceiveRemoveItemPack();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "RepeatItemPack"))
	void ReceiveRepeatItemPack();
	

	class AItemPack* CheckItempack;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* DroppedItemDescVerticalBox = nullptr;
};
