// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Engine/DataAsset.h"
#include "Misc/Utils.h"
#include "HOR/PlayerDioController.h"
#include "HOR/CustomItemComponent.h"
#include "Actors/Effect/Effect.h"
#include "ItemPack.generated.h"

USTRUCT()
struct KDT3D_API FItemPackTableRow : public FTableRowBase
{
	GENERATED_BODY()
	FItemPackTableRow();
public:
	UPROPERTY(EditAnywhere, Category = "Item")
	UStaticMesh* StaticMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Item")
	FVector Scale = FVector(2.0, 2.0, 2.0);

	UPROPERTY(EditAnywhere, Category = "Item")
	float AddMaxHP = 0;

	UPROPERTY(EditAnywhere, Category = "Item")
	float Heal = 0;
	
	UPROPERTY(EditAnywhere, Category = "Item")
	float AddDMG = 0;

	//UPROPERTY(EditAnywhere, Category = "Item")
	//bool WinItem = false;

	UPROPERTY(EditAnywhere, Category = "Item")
	bool RepeatItem = false;	//0=repeat 1=delete
	
	UPROPERTY(EditAnywhere, Category = "Item")
	float RepeaTime = 0;

	UPROPERTY(EditAnywhere, Category = "Item|Sound")
	TObjectPtr<USoundBase> Sound;
	UPROPERTY(EditAnywhere, Category = "Item|Sound")
	float VolumeMultiplier = 1.f;
	UPROPERTY(EditAnywhere, Category = "Item")
	TSubclassOf<AItemPack> ItemClass;
};


UCLASS()
class KDT3D_API AItemPack : public AActor
{
	GENERATED_BODY()
public:
	struct FItemContext
	{
		// Item을 사용하는 주체
		APlayerController* User;
		FName ItemName;		//이름에 따라서 처리를 다르게 ㄱㄱ
	};
	using FContext = FItemContext;
public:
	void SetContext(TUniquePtr<FContext> NewContext) { check(!Context); Context = MoveTemp(NewContext); }

public:
	//virtual void Use();
	//virtual void Repeat() { Use(); }
	//virtual void Delete() { ReleaseContext(); }
public:	
	// Sets default values for this actor's properties
	AItemPack(const FObjectInitializer& ObjectInitializer);
	virtual void SetData(const FDataTableRowHandle& InDataTableRowHandle);
	virtual void UseItem(APawn* Pawn);
	//TODO:: item 관련 싹 만들고 HUD 등등 만들기

private:
	void ReleaseContext() { check(Context); Context = nullptr; }

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	//ㄴ> BeginOverlap을 사용하는게 더 나을지도 모르겠음... 
	// 솔직히 뭘 하던 ㄱㅊ지 않나 싶기도 하고 일단 바꿔야함. ㅇㅇ
	//TODO: 이거 자기 변수 사용하는거 보다 이거 사용해서 해야 할거 같음.


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	TUniquePtr<FContext> Context;

	float fItemRefillTime = 0;
	bool ItemRefill = false;
	bool CanUseItem = true;

	class APlayerDioController* PlayerController = nullptr;
	//class ABasicPlayerController* BasicController = nullptr;
	class UCustomItemComponent* ItemComponent = nullptr;


protected:
	//UPROPERTY(VisibleAnywhere)
	//AEffect	Effects;

	UPROPERTY(VisibleAnywhere)
	UShapeComponent* Collider;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	//UPROPERTY(VisibleAnywhere)
	//UPaperSpriteComponent* MinimapSpriteComponent;

protected:
	UPROPERTY(EditAnywhere, meta = (RowType = "/Script/KDT3D.ItemPackTableRow"))
	FDataTableRowHandle DataTableRowHandle;

	//TArray<AActor*> OverlappedItemPacks;
};
