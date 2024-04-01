// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Board.h"
#include "Kismet/GameplayStatics.h"
#include "GameLogic.generated.h"

class APawnPlayer;

UCLASS()
class ZOMBOID_API AGameLogic : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameLogic();

	UFUNCTION()
	void ProcessInput(FString Field);

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void SetupActorReferences();

	UFUNCTION()
	void SendSelfReference();

	UFUNCTION()
	void SendBoardReference();

	UFUNCTION()
	void setActivePlayer(bool SetIsTurn);

	UFUNCTION()
	void setActiveEnemy(bool SetIsTurn);
	// Protected Member Attributes


	// References to other Relevant Actors
	UPROPERTY(BlueprintReadOnly)
	ABoard* BoardReference;
	UPROPERTY(BlueprintReadOnly)
	APawnPlayer* PlayerReference;
	UPROPERTY(EditAnywhere)
	APawnPlayer* EnemyReference;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	// Private Attributes
	bool Running = true;
	// Private 
};
