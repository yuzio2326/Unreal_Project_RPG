// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HPBarWidget.h"
#include "Components/StatusComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void UHPBarWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UHPBarWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APawn* Pawn = GetOwningPawn();
	UStatusComponent* StatusComponent = Pawn->GetComponentByClass<UStatusComponent>();
	check(StatusComponent);
	StatusComponent->OnHPChanged.AddDynamic(this, &ThisClass::OnHPChanged);
	StatusComponent->OnDie.AddDynamic(this, &ThisClass::OnDie);

	SetVisibility(ESlateVisibility::Hidden);	//check 이후 Hidden으로 바꾸기
}

void UHPBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	const float EffectPercent = HPBarEffect->GetPercent();
	const float Percent = HPBar->GetPercent();
	if (!FMath::IsNearlyEqual(EffectPercent, Percent))
	{
		const float NewEffectPercent = FMath::Lerp(EffectPercent, Percent, InDeltaTime * 2.f);
		HPBarEffect->SetPercent(NewEffectPercent);
	}
}

void UHPBarWidget::OnHiddenUI()
{
	SetVisibility(ESlateVisibility::Hidden); //check 이후 Hidden으로 바꾸기
}

void UHPBarWidget::OnHPChanged(float CurrentHP, float MaxHP)
{
	if (FMath::IsNearlyZero(MaxHP))
	{
		ensureMsgf(false, TEXT("MaxHP is nearly zero"));
		return;
	}
	SetVisibility(ESlateVisibility::Visible);
	const float Percent = CurrentHP / MaxHP;
	HPBar->SetPercent(Percent);

	UKismetSystemLibrary::K2_SetTimer(this, TEXT("OnHiddenUI"), 3.f, false);
}

void UHPBarWidget::OnDie()
{
	UKismetSystemLibrary::K2_SetTimer(this, TEXT("OnHiddenUI"), 2.f, false);
}
