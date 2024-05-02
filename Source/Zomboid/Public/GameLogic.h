// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Board.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TextRenderActor.h"
#include "Components/TextRenderComponent.h"
#include "Delegates/MulticastDelegateBase.h"
#include "GameLogic.generated.h"


// Template to Parse any Inputs to a Valid Field
template <typename T>
class FieldParser;

template<>
class FieldParser<TArray<int>>
{
private:
	TArray<int> Coord;

public:
	FieldParser(const TArray<int>& i) : Coord(i) {}
	
	FString ReturnValidField()
	{
		return FString::Printf(TEXT("%d,%d"), Coord[0], Coord[1]);
	}
};

template<>
class FieldParser<int>
{
private:
	int Value;
	int Value2;

public:
	FieldParser(const int& i, const int& j) : Value(i), Value2(j) {}

	FString ReturnValidField()
	{
		return FString::Printf(TEXT("%d,%d"), Value, Value2);
	}
};

template<>
class FieldParser<FVector2D>
{
private:
	FVector2D Coord;

public:
	FieldParser(const FVector2D i) : Coord(i){}

	FString ReturnValidField()
	{
		return FString::Printf(TEXT("%d,%d"),Coord.X, Coord.Y);
	}
};

// Enum for GameModes
UENUM(BlueprintType)
enum class EChooseGameMode : uint8
{
	Normal UMETA(DisplayName = "Normal Tic Tac Toe"),
	Crawler UMETA(DisplayName = "Dungeon Crawler")
};

//Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiDispatcher, int, WhoWon);

// Forward Declaration
class APawnPlayer;
class ABoardTarget;

UCLASS()
class ZOMBOID_API AGameLogic : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameLogic();

	UFUNCTION()
	void ReceiveMove(FString& Field);

	UFUNCTION()
	void ProcessInputNormalTicTacToe(FString Field);

	UFUNCTION()
	void ProcessInputCrawlTicTacToe(FString Field);

	UPROPERTY(BlueprintAssignable, Category = "Multi GameLogic Dispatcher")
	FMultiDispatcher MultiDispatcher;

	UFUNCTION()
	void RefreshTextActor();

	UPROPERTY(EditAnywhere)
	EChooseGameMode ActiveGamemode;


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

	UPROPERTY()
	int CurrentTurn;

	UPROPERTY()
	TMap<FString, FVector2D> DirectionMap;


	// References to other Relevant Actors
	UPROPERTY(BlueprintReadOnly)
	ABoard* BoardReference;

	UPROPERTY(BlueprintReadOnly)
	ABoardTarget* BoardTargetReference;

	UPROPERTY(BlueprintReadOnly)
	APawnPlayer* PlayerReference;

	UPROPERTY(EditAnywhere)
	APawnPlayer* EnemyReference;

	UPROPERTY(EditAnywhere)
	ATextRenderActor* ScoreCounter;

	UPROPERTY(EditAnywhere)
	ATextRenderActor* LifeCounterPlayer;

	UPROPERTY(EditAnywhere)
	ATextRenderActor* LifeCounterEnemy;

	UPROPERTY(EditAnywhere)
	ATextRenderActor* TurnCounter;

	UFUNCTION()
	bool CheckForPossiblePlaces();

	UFUNCTION()
	bool CheckForCorrectPattern();




public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HardMode;

	UFUNCTION()
	ABoard* SendOtherBoardRefMethod();

	UFUNCTION()
	FString ParseIntToValidField(int RowValue, int ColValue);

	UFUNCTION()
	TArray<int> ParseFieldToInt(FString Field);

	UPROPERTY(VisibleAnywhere)
	TArray<FString> InstructionsToExecute;

	UFUNCTION(CallInEditor, BlueprintCallable)
	TArray<FString> CalculateMovesOnInstructions(FString Startfield);

private:

	// Private Attributes

	// Private 
};
