// Fill out your copyright notice in the Description page of Project Settings.


#include "HOR/ItemPack.h"
#include "Misc/Utils.h"
#include "Subsystem/ItemSubsystem.h"	//Not Use
#include "HOR/CustomDioStatusComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Data/PawnData.h"



// Sets default values
AItemPack::AItemPack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USphereComponent>(TEXT("Collider")))
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Collider = CreateDefaultSubobject<UShapeComponent>(TEXT("Collider"));
	Collider->SetCollisionProfileName(CollisionProfileName::PawnTrigger);
	RootComponent = Collider;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	PrimaryActorTick.bCanEverTick = true;
	OnActorBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
	//ItemComponent = CreateDefaultSubobject<UCustomItemComponent>(TEXT("CustomItemComponent"));
	// Add Dynamic 해야 하는데 이 액터를 만들때 Add Dynamic    
	//check(ItemComponent);

}

void AItemPack::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetData(DataTableRowHandle);
}

void AItemPack::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FItemPackTableRow* Data = DataTableRowHandle.GetRow<FItemPackTableRow>(TEXT("ItemPack"));
	if (!Data) { ensure(false); return; }

	StaticMeshComponent->SetStaticMesh(Data->StaticMesh);
	StaticMeshComponent->SetRelativeScale3D(Data->Scale);


	if (USphereComponent* SphereComponent = Cast<USphereComponent>(Collider))
	{
		const float ScaledRadius = FMath::Max3(Data->Scale.X, Data->Scale.Y, Data->Scale.Z);

		if (Data->StaticMesh)
		{
			FBoxSphereBounds BoxSphereBounds = Data->StaticMesh->GetBounds();
			SphereComponent->SetSphereRadius(ScaledRadius * BoxSphereBounds.SphereRadius);
		}
	}

}

//HUD의 droped item에서 호출하게 함
void AItemPack::UseItem(APawn* Pawn)
{
	if (DataTableRowHandle.IsNull()) { return; }
	FItemPackTableRow* Data = DataTableRowHandle.GetRow<FItemPackTableRow>(TEXT("ItemPack"));
	if (!Data) { ensure(false); return; }

	UCustomDioStatusComponent* PlayerStatus = Pawn->GetComponentByClass<UCustomDioStatusComponent>();
	FPawnTableRow* PawnData = DataTableRowHandle.GetRow<FPawnTableRow>(TEXT("Character"));
	if (!Data->Sound.IsNull())
	{
		const FVector Location = GetActorLocation();
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Data->Sound, Location, Data->VolumeMultiplier);
	}

	//아이템 재사용 가능하면
	if (Data->RepeatItem)
	{
		PlayerStatus->AddMaxHP(Data->AddMaxHP);
		PlayerStatus->AddHP(Data->Heal);
		fItemRefillTime = Data->RepeaTime;
		CanUseItem = false;
	}
	//일회용 아이템
	else if (!Data->RepeatItem)
	{
		PlayerStatus->AddMaxHP(Data->AddMaxHP);
		PlayerStatus->AddHP(Data->Heal);
		PlayerStatus->AddDMG(Data->AddDMG);
		//PlayerStatus->WinGame(Data->WinItem);
		Destroy();
	}



#pragma region ItemAction 주석

	//UItemAction* ItemAction = Cast<UItemAction>(Data->ItemActionClass->GetDefaultObject());
	//if (!ItemAction)
	//{
	//	ensureMsgf(false, TEXT("ItemAction is nullptr"));
	//	return;
	//}

	//TUniquePtr<UItemAction::FContext> Context = MakeUnique<UItemAction::FContext>();
	//Context->User = PC;
	//Context->ItemName = DataTableRowHandle.RowName;
	//ItemAction->SetContext(MoveTemp(Context));
	//ItemAction->Use();

#pragma endregion
}

// Called when the game starts or when spawned
void AItemPack::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerDioController>(UGameplayStatics::GetPlayerController(this, 0));
	check(PlayerController);

}

void AItemPack::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	APawn* PlayerPawn = PlayerController->GetPawn();//nullptr
	if (OtherActor == PlayerPawn)
	{ 
		UseItem(PlayerPawn); 
		SetActorEnableCollision(false);
	}
	
}

// Called every frame
void AItemPack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Rotating Item
	{
		FRotator Rotator{ 0.0, DeltaTime * 60.0, 0.0 };
		AddActorLocalRotation(Rotator);
	}
	
	if (DataTableRowHandle.IsNull()) { return; }
	FItemPackTableRow* Data = DataTableRowHandle.GetRow<FItemPackTableRow>(TEXT("ItemPack"));


	//보충이 되는 아이템
	//ItemComponent만들었으니 거기에 여기 있는 기능구현을 만들고 여기는 ovelap되었을때 자신의 
	// itemComponent를 통해서 기능 호출을 하도록 하자

	//FPawnTableRow* PawnData = DataTableRowHandle.GetRow<FPawnTableRow>(TEXT("Character"));

	
	//APawn* PlayerPawn = PlayerController->GetPawn();//nullptr

	if (CanUseItem) 
	{
		SetActorEnableCollision(true);

	}
	else 
	{
		float fSizeValue = (Data->RepeaTime - fItemRefillTime) / Data->RepeaTime;
		fSizeValue = FMath::Clamp(fSizeValue, 0.0f, 1.0f);
		StaticMeshComponent->SetRelativeScale3D(Data->Scale*(fSizeValue));

	}

	if (fItemRefillTime>0)
	{
		fItemRefillTime -= DeltaTime;
	}
	else if (fItemRefillTime <= 0)
	{
		CanUseItem = true;
	}



}

FItemPackTableRow::FItemPackTableRow()
	: ItemClass(AItemPack::StaticClass())
{
}

