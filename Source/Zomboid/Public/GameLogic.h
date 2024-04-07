// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Board.h"
#include "Kismet/GameplayStatics.h"
#include "Delegates/MulticastDelegateBase.h"
#include "GameLogic.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiDispatcher, int, WhoWon);

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

	UPROPERTY(BlueprintAssignable, Category = "Multi GameLogic Dispatcher")
	FMultiDispatcher MultiDispatcher;

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

	UFUNCTION(BlueprintCallable)
	void ResetBoardForNewGame();


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HardMode;

private:

	// Private Attributes
	bool Running = true;
	// Private 
};
