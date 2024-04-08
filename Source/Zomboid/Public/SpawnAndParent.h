// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataprepOperation.h"
#include "Engine/StaticMeshActor.h"
#include "SpawnAndParent.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBOID_API USpawnAndParent : public UDataprepEditingOperation
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	TArray<FString> Test;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> MyClass = nullptr;

	UPROPERTY()
	AActor* MyActor;


	virtual void OnExecution_Implementation(const FDataprepContext& InContext) override;
};
