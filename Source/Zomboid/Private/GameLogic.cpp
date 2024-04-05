// Fill out your copyright notice in the Description page of Project Settings.

#include "GameLogic.h"
#include "PawnPlayer.h"
#include "UObject/UObjectGlobals.h"


// Sets default values
AGameLogic::AGameLogic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameLogic::BeginPlay()
{
	Super::BeginPlay();
	SetupActorReferences();
	SendSelfReference();
	SendBoardReference();
	setActivePlayer(true);
	setActiveEnemy(false);

}

// Called every frame
void AGameLogic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameLogic::SetupActorReferences()
{


	// Setup Board Reference
	for (TObjectIterator<ABoard> i; i; ++i)
	{
		BoardReference = *i;
		UE_LOG(LogTemp, Warning, TEXT("Board Reference was set"));
	}
	if (BoardReference == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Board Reference is Null"));
	}

	// Setup Player Reference
	PlayerReference = Cast<APawnPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (PlayerReference == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Reference is Null"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Reference was Set"));
	}

	// Setup Enemy Reference
	// Not Implemented Yet;

}

void AGameLogic::SendSelfReference()
{
	PlayerReference->ReceiveGameLogicRef(this);
}


void AGameLogic::ProcessInput(FString Field)
{
	for (int i = 0; i <= 1; i++)
	{
		// Check if Player or Enemys Turn
		if (PlayerReference->GetIsTurn() == true)
		{
			bool IsSucess;
			UE_LOG(LogTemp, Warning, TEXT("Players Turn"));
			// Will be Executed when its the Players Turn
			if (BoardReference)
			{
				IsSucess = BoardReference->CalcMove(Field, true); // Move Ausgeführt
				int CurrentStateOfBoard = BoardReference->CheckWin();
				if (CurrentStateOfBoard == -1 || CurrentStateOfBoard == 1 || CurrentStateOfBoard == 0) // Spiel ist beendet
				{
					setActivePlayer(false);
					setActiveEnemy(false);
					MultiDispatcher.Broadcast(CurrentStateOfBoard);
					return;				
				}
				if (IsSucess)
				{
					setActivePlayer(false);
					setActiveEnemy(true);
				}

			}

			//Or Calling the Enemy Function inside the Players Block?
			//ProcessInput(EnemyReference->MakeAIMove());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Enemys Turn"));
			// Will be Executed when its the Enemy Turn
			if (BoardReference)
			{
				BoardReference->CalcMove(EnemyReference->MakeEasyAIMove(), false);
			}
			int CurrentStateOfBoard = BoardReference->CheckWin();
			if (CurrentStateOfBoard == -1 || CurrentStateOfBoard == 1 || CurrentStateOfBoard == 0)
			{
				setActivePlayer(false);
				setActiveEnemy(false);
				MultiDispatcher.Broadcast(CurrentStateOfBoard);
				return;
			}
			setActiveEnemy(false);
			setActivePlayer(true);
		}
	}
}

void AGameLogic::setActivePlayer(bool SetIsTurn)
{
	PlayerReference->SetIsTurn(SetIsTurn);
}

void AGameLogic::setActiveEnemy(bool SetIsTurn)
{
	EnemyReference->SetIsTurn(SetIsTurn);
}


void AGameLogic::SendBoardReference()
{
	EnemyReference->ReceiveBoardRef(BoardReference);
	PlayerReference->ReceiveBoardRef(BoardReference);
}

void AGameLogic::ResetBoardForNewGame()
{
	BoardReference->InitPlayboard();
	setActivePlayer(true);
	setActiveEnemy(false);
}