// Fill out your copyright notice in the Description page of Project Settings.

#include "HOR/MeleeATK.h"
#include "Misc/Utils.h"
#include "Kismet/KismetMathLibrary.h"
#include "Subsystem/ActorPoolSubsystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMeleeATK::AMeleeATK()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeleeCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeleeCapsuleComponent"));
	RootComponent = MeleeCapsuleComponent;

	InitialLifeSpan = 0.1f;	//CanChange

	//projectile 구성요소 같으니까 그거 쓰면 될듯
	MeleeCapsuleComponent->SetCollisionProfileName(CollisionProfileName::Projectile);
	MeleeCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);

}

void AMeleeATK::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{

	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FMeleeATKTableRow* Data = DataTableRowHandle.GetRow<FMeleeATKTableRow>(TEXT("MeleeATKTableRow"));
	if (!Data) { ensure(false); return; }
	
	MeleeATKData = Data;
	MeleeCapsuleComponent->MoveIgnoreActors.Empty();
	MeleeCapsuleComponent->MoveIgnoreActors.Add(GetOwner());
	MeleeCapsuleComponent->SetRelativeTransform(Data->Transform);
	//StaticMeshComponent->SetStaticMesh(Data->StaticMesh);

	//MoveIgnoreActors

}

// Called when the game starts or when spawned
void AMeleeATK::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMeleeATK::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FVector Location = GetActorLocation();
	if (!IsValid(this)) { return; }


	// BeginPlay 시점에 Overlapped 되면 들어 옴
	if (!bFromSweep)
	{
		Destroy();
		check(false);
		return;
	}

	FTransform NewTransform;
	NewTransform.SetLocation(SweepResult.ImpactPoint);
	FRotator Rotation = UKismetMathLibrary::Conv_VectorToRotator(SweepResult.ImpactNormal);
	NewTransform.SetRotation(Rotation.Quaternion());

	//decal로 맞은 대상에게 충격 관련된거 추가하면 ㄱㅊ을듯함.. 나중에 추가
	GetWorld()->GetSubsystem<UActorPoolSubsystem>()->SpawnHitEffectWithDecal(NewTransform, MeleeATKData->HitEffectTableRowHandle);
	Destroy();


	float fDamage = MeleeATKData->DMG;
	UGameplayStatics::ApplyDamage(OtherActor, fDamage, GetInstigator()->GetController(), this, nullptr);
}

// Called every frame
void AMeleeATK::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FMeleeATKTableRow::FMeleeATKTableRow()
	: MeleeATKClass(AMeleeATK::StaticClass())
{
	int a = 0;
}
