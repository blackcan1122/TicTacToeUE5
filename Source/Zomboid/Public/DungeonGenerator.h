// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "DungeonGenerator.generated.h"




// Enum for GameModes
UENUM(BlueprintType)
enum class EMarkType : uint8
{
	Free UMETA(DisplayName = "Free Access"),
	Wall UMETA(DisplayName = "Blocked Wall")
};

// Forward Declarations
class ADungeonTile;
class UTileMarks;

UCLASS()
class ZOMBOID_API ADungeonGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonGenerator();

	UFUNCTION(CallInEditor)
	void Prepare();

	UFUNCTION(BlueprintCallable)
	void SpawnDungeonTiles(int Amount);

	UFUNCTION(CallInEditor)
	void ClearDungeonTiles();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	TMap<FString,FVector> PlacementDirection;

	UPROPERTY()
	TMap<FString, FVector> InversedPlacementDirection;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ADungeonTile>> AvaiableTiles;

	UPROPERTY(VisibleAnywhere)
	UClass* PickedClass;

	UPROPERTY(visibleAnywhere)
	ADungeonTile* LastPieceMade;

	UPROPERTY(VisibleAnywhere)
	TArray<ADungeonTile*> Neighbors;

	UPROPERTY(VisibleAnywhere)
	TArray<FString> LM_MarkDirectionPosition;

	UPROPERTY(VisibleAnywhere)
	TArray<FString> New_MarkDirectionPosition;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> LM_MarkWorldPositions;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> New_MarkWorldPositions;

	UPROPERTY()
	float PositionOffsetDungeonTile;

	UPROPERTY(VisibleAnywhere)
	FTransform LastMadeActTransform;

	UPROPERTY(VisibleAnywhere)
	TArray<UTileMarks*> LastMadeFreeExits;

	UPROPERTY(VisibleAnywhere)
	FString LastInstruction;

	UPROPERTY(VisibleAnywhere)
	TArray<UTileMarks*> NewFreeExits;

	UPROPERTY(VisibleAnywhere)
	FTransform NewActTransform;


	UFUNCTION()
	bool CheckPossiblePositions(ADungeonTile* LastMadePiece, ADungeonTile* PotentialNewTile);

	UFUNCTION(BlueprintCallable)
	void CheckForNeighbors(ADungeonTile* StartPos);

	UFUNCTION()
	FVector SetPositionAndRotation(ADungeonTile* NewTile);

	UFUNCTION()
	int CheckForAmountOfExits(ADungeonTile* TileToCheck);

	UFUNCTION()
	bool CheckValidPosition(FVector PossibleCoords);

	UFUNCTION()
	bool CheckCorrectOrientation(ADungeonTile* NewTile);

	//UFUNCTION()
	//void DefineValidDirections(ADungeonTile* StartPos);



};
