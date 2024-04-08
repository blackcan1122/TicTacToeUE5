// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnAndParent.h"

void USpawnAndParent::OnExecution_Implementation(const FDataprepContext& InContext)
{

	TArray<TObjectPtr<UObject>> InputGeo = InContext.Objects;

	for (int i = 0; i < InputGeo.Num(); i++)
	{
		FName InputGeoName = InputGeo[i].GetFName();

		FText Temp = FText::FromName(InputGeoName);
		//LogWarning(Temp);
		AStaticMeshActor* StaticMesh = Cast<AStaticMeshActor>(InputGeo[i]);

		if(StaticMesh != nullptr)
		{
			LogWarning(Temp);
			LogWarning(FText::FromString("Ist ein StaticMesh"));
		}
	}

	FText Warning = FText::FromString("Hier ist ein Test");
	LogWarning(Warning);

	MyActor = CreateActor(MyClass,FString("Test"));
	
}