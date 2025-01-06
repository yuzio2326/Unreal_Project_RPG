// Fill out your copyright notice in the Description page of Project Settings.


#include "HOR/ROH_HUDPlayerUserWidget.h"

void UROH_HUDPlayerUserWidget::AddItemPack(AItemPack* ItemPack)
{
	ReceiveAddItemPack();
}

void UROH_HUDPlayerUserWidget::RemoveItemPack(AItemPack* ItemPack)
{
	ReceiveRemoveItemPack();
}

void UROH_HUDPlayerUserWidget::RepeatItemPack(AItemPack* ItemPack)
{
	ReceiveRepeatItemPack();
}
