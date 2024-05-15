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
	FreeExists.Empty();
	NumberOfExits = 0;
	TArray<UTileMarks*> Components;
	this->GetComponents<UTileMarks>(Components);

	for (auto Mark : Components)
	{
		EMarkType Temp = Mark->CurrentConnectionType.CurrentMarkType;
		if (Temp == EMarkType::Free)
		{
			FreeExists.Add(Mark);
			NumberOfExits++;
		}
	}
}

TArray<UTileMarks*> ADungeonTile::GetFreeExits()
{
	SetNumberOfExits();
	return FreeExists;
}