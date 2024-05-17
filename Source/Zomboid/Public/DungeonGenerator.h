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
	Wall UMETA(DisplayName = "Blocked Wall"),
	InUse UMETA(DisplayName = "IsConnected")
};

USTRUCT(BlueprintType)
struct ZOMBOID_API FMarkConnection
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EMarkType CurrentMarkType;

	UPROPERTY(VisibleAnywhere)
	ADungeonTile* ConnectedTo = nullptr;


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

	UFUNCTION(BlueprintCallable)
	void SpawnDungeonTiles(int Amount);

	UFUNCTION(CallInEditor)
	void ClearDungeonTiles();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
	void CheckForNeighbors(const ADungeonTile* StartPos);

	UFUNCTION()
	bool SetPositionAndRotation(ADungeonTile* NewTile);

	UFUNCTION()
	int CheckForAmountOfExits(ADungeonTile* TileToCheck);

	UFUNCTION()
	bool CheckValidPosition(FVector PossibleCoords);

	UFUNCTION()
	bool CheckCorrectOrientation(ADungeonTile* NewTile);

	UFUNCTION(CallInEditor)
	void PrintAllErrorTiles();

	UPROPERTY(VisibleAnywhere)
	TArray<ADungeonTile*> ProblematicTiles;

	UFUNCTION()
	void ChooseANewContinue();

	UFUNCTION()
	bool SpawnDeadEnds();

	UFUNCTION()
	bool SpawnNextTile();

	UFUNCTION()
	ADungeonTile* ReturnTileAboveExits(UWorld* CurrentWorld, int AboveAmount);

	UFUNCTION()
	bool ProcessNewTile(ADungeonTile* NewTile, int iteration);

	UFUNCTION()
	bool CheckIfAllMarksConnected(ADungeonTile* StartPos);

	UFUNCTION()
	float IsWithinDistance(const FVector& Pos1, const FVector& Pos2);

	UFUNCTION()
	void GatherActorToFix(const TArray<ADungeonTile*>& ActorsToCheck);

	UFUNCTION(BlueprintCallable)
	bool FixAllEncaplsuledActors();

	UFUNCTION()
	TArray<UTileMarks*> ReturnAdjacentMarksOfNeighbors(const ADungeonTile* StartTile);

	UPROPERTY(EditAnywhere)
	TSubclassOf<ADungeonTile> FallBackSolution;

	UPROPERTY(VisibleAnywhere)
	TArray<ADungeonTile*> ActorsToFix;

	UPROPERTY(VisibleAnywhere)
	TArray<ADungeonTile*> ActorsToContinueBranching;

	UFUNCTION()
	ADungeonTile* ReturnTileWithAmountOfExits(UWorld* CurrentWorld, int Amount);

	UFUNCTION(CallInEditor)
	void DEBUGUpdateFixActor();

};
