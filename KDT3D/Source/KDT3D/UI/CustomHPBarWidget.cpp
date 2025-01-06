// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/CustomHPBarWidget.h"
#include "HOR/CustomDioStatusComponent.h"
#include "Components/StatusComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void UCustomHPBarWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UCustomHPBarWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UCustomHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	APawn* Pawn = GetOwningPawn();
	UStatusComponent* StatusComponent = Pawn->GetComponentByClass<UStatusComponent>();
	check(StatusComponent);
	UCustomDioStatusComponent* CustomStatusComponent = 
		Pawn->GetComponentByClass<UCustomDioStatusComponent>();
	check(CustomStatusComponent);
	if (CustomStatusComponent != nullptr)
	{
		CustomStatusComponent->OnPlayerHPChanged.AddDynamic(this, &ThisClass::OnHPChanged);
		CustomStatusComponent->OnDead.AddDynamic(this, &ThisClass::OnDeath);

	}
	else if (StatusComponent != nullptr)
	{
		StatusComponent->OnHPChanged.AddDynamic(this, &ThisClass::OnHPChanged);
		StatusComponent->OnDie.AddDynamic(this, &ThisClass::OnDie);
	}
	else 
		return;


	//StatusComponent->OnHPChanged.AddDynamic(this, &ThisClass::OnHPChanged);
	//StatusComponent->OnDie.AddDynamic(this, &ThisClass::OnDie);

	SetVisibility(ESlateVisibility::Visible);	//check 이후 Hidden으로 바꾸기
}

void UCustomHPBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UCustomHPBarWidget::OnHiddenUI()
{
	
}

void UCustomHPBarWidget::OnHPChanged(float CurrentHP, float MaxHP)
{
}

void UCustomHPBarWidget::OnDie()
{
	UKismetSystemLibrary::K2_SetTimer(this, TEXT("OnHiddenUI"), 2.f, false);
}

void UCustomHPBarWidget::OnDeath(bool IsDead)
{
	if (IsDead)
	{
		UKismetSystemLibrary::K2_SetTimer(this, TEXT("OnHiddenUI"), 2.f, false);
	}
}
