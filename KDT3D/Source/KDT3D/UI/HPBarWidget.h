// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWidgetBase.h"
#include "Components/ProgressBar.h"
#include "HPBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class KDT3D_API UHPBarWidget : public UUserWidgetBase
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized();
	virtual void NativePreConstruct();
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	UFUNCTION()
	void OnHiddenUI();
	UFUNCTION()
	void OnHPChanged(float CurrentHP, float MaxHP);
	UFUNCTION()
	void OnDie();

protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UProgressBar* HPBarEffect;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UProgressBar* HPBar;
};
