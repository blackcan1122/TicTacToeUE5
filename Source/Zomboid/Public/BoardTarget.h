// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoardTarget.generated.h"

class AGameLogic;
class ABoard;

UCLASS()
class ZOMBOID_API ABoardTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoardTarget();

	UFUNCTION()
	void DrawTarget();

	UFUNCTION()
	void ReceiveGameLogicRef(AGameLogic* PGameLogicRef);

	UFUNCTION()
	TMap<FString, FVector2D> ReturnDirectionMap();

	UFUNCTION()
	TArray<FString> GetInstructions();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	AGameLogic* GameLogicRef;

	UPROPERTY(EditAnywhere)
	ABoard* BoardReference;

	UPROPERTY(VisibleAnywhere)
	TMap<FString, FVector2D> DirectionMap;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector2D> TargetBoard;


	UPROPERTY(VisibleAnywhere)
	TArray<FString> InstructionsBoard;

	UFUNCTION(CallInEditor)
	void DebugGeneratedTargets();

	UFUNCTION(CallInEditor)
	void GenerateTarget();

	UFUNCTION(CallInEditor)
	void CleanComponents();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
