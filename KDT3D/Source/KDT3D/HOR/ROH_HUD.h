// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HOR/ItemPack.h"
#include "Misc/Utils.h"
#include "DIOCharacter.h"
#include "ROH_HUD.generated.h"

/**
 * 
 */
UCLASS()
class KDT3D_API AROH_HUD : public AHUD
{
	GENERATED_BODY()

public:
	AROH_HUD();
protected:
	//void AddDroppedItem(ADroppedItem* NewItem);		//HP회복 같은거
	//void RemoveDroppedItem(ADroppedItem* NewItem);
	//TODO:: 이거 완성 시키고 BossMonster하고 Player피격 관련 만들어서 적용 시키기
	//
	void AddItemPack(AItemPack* ItemPack);
	void RemoveItemPack(AItemPack* ItemPack);
	void RepeatItemPack(AItemPack* ItemPack);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	class APlayerDioController* PlayerController = nullptr;
	class UROH_HUDPlayerUserWidget* Widget = nullptr;

	//class UInputMappingContext* IMC_Skill = nullptr;	//안쓰고 닿으면 회복 ㄱㄱ
	//ㄴ> skill_imc 받아서 데이터 입력값 확인 하고 skillcomponent의 skillcooltime을 보여줍시다

	TArray<AActor*> OverlappedDroppedItems;
	
};
