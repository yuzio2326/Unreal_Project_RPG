// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/ItemSubsystem.h"

UItemSubsystem::UItemSubsystem()
{
	{
		static ConstructorHelpers::FObjectFinder<UDataTable> Asset(TEXT("/Script/Engine.DataTable'/Game/Blueprint/Weapon/DT_Weapon.DT_Weapon'"));
		check(Asset.Object);
		WeaponDataTable = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UDataTable> Asset(TEXT("/Script/Engine.DataTable'/Game/Blueprint/Weapon/DT_Gun.DT_Gun'"));
		check(Asset.Object);
		GunDataTable = Asset.Object;
	}
	//{
	//	static ConstructorHelpers::FObjectFinder<UDataTable> Asset(TEXT("/Script/Engine.DataTable'/Game/Blueprint/01_PersonalProjects/03_Item/ItemPack.ItemPack'"));
	//	check(Asset.Object);
	//	ItemPackDataTable = Asset.Object;
	//}
}

FDataTableRowHandle UItemSubsystem::FindItem(const FName& InKey)
{
	FDataTableRowHandle DataTableRowHandle;

#if WITH_EDITOR
	bool bFind = false;
	if (WeaponDataTable->GetRowMap().Find(InKey))
	{
		check(!bFind);
		bFind = true;
		DataTableRowHandle.DataTable = WeaponDataTable;
		DataTableRowHandle.RowName = InKey;
	}
	if (GunDataTable->GetRowMap().Find(InKey))
	{
		check(!bFind);
		bFind = true;
		DataTableRowHandle.DataTable = GunDataTable;
		DataTableRowHandle.RowName = InKey;
	}	
	//if (ItemPackDataTable->GetRowMap().Find(InKey))
	//{
	//	check(!bFind);
	//	bFind = true;
	//	DataTableRowHandle.DataTable = ItemPackDataTable;
	//	DataTableRowHandle.RowName = InKey;
	//}
#else
	if (WeaponDataTable->GetRowMap().Find(InKey))
	{
		DataTableRowHandle.DataTable = WeaponDataTable;
		DataTableRowHandle.RowName = InKey;
	}
	else if (GunDataTable->GetRowMap().Find(InKey))
	{
		DataTableRowHandle.DataTable = GunDataTable;
		DataTableRowHandle.RowName = InKey;
	}	
	//else if (ItemPackDataTable->GetRowMap().Find(InKey))
	//{
	//	DataTableRowHandle.DataTable = ItemPackDataTable;
	//	DataTableRowHandle.RowName = InKey;
	//}
#endif

	return DataTableRowHandle;
}
