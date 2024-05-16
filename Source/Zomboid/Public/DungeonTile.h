// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileMarks.h"
#include "DungeonGenerator.h"
#include "DungeonTile.generated.h"

UCLASS()
class ZOMBOID_API ADungeonTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonTile();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void PostInitializeComponents() override;

	UPROPERTY(EditAnywhere)
	int NumberOfExits;

	UPROPERTY(VisibleAnywhere)
	TArray<UTileMarks*> FreeExists;

	UPROPERTY(VisibleAnywhere)
	TArray<UTileMarks*> InUseExists;

	UFUNCTION()
	void SetNumberOfExits();


	UFUNCTION(BlueprintCallable)
	TArray<UTileMarks*> GetFreeExits();

	UFUNCTION(BlueprintCallable)
	TArray<UTileMarks*> GetInUseExits();

};
