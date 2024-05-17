// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonTile.h"

// Sets default values
ADungeonTile::ADungeonTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ADungeonTile::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetNumberOfExits();

}

void ADungeonTile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SetNumberOfExits();
}


void ADungeonTile::SetNumberOfExits()
{
	InUseExists.Empty();
	FreeExists.Empty();
	TArray<UTileMarks*> Components;
	this->GetComponents<UTileMarks>(Components);

	for (auto Mark : Components)
	{
		EMarkType Temp = Mark->CurrentConnectionType.CurrentMarkType;
		if (Temp == EMarkType::Free)
		{
			FreeExists.Add(Mark);
		}
		else if (Temp == EMarkType::InUse)
		{
			InUseExists.Add(Mark);
		}
	}
}

TArray<UTileMarks*> ADungeonTile::GetFreeExits()
{
	SetNumberOfExits();
	return FreeExists;
}

TArray<UTileMarks*> ADungeonTile::GetInUseExits()
{
	SetNumberOfExits();
	return InUseExists;
}

TArray<UTileMarks*> ADungeonTile::GetInUseAndFreeExits()
{
	SetNumberOfExits();
	TArray<UTileMarks*> ReturnArray;
	ReturnArray.Append(FreeExists);
	ReturnArray.Append(InUseExists);
	return ReturnArray;
}
