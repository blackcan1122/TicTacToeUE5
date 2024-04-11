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

	PlayerReference->AddToLife(3);
	EnemyReference->AddToLife(3);
	RefreshTextActor();
	CurrentTurn = 0;

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
				IsSucess = BoardReference->CalcMove(Field, true, false); // Move Ausgeführt
				int CurrentStateOfBoard = (BoardReference->CheckWin()); // (Player Wins:1; Computer Wins: -1; Tie:0 and keep Running is 2)
				if (CurrentStateOfBoard == -1 || CurrentStateOfBoard == 1 || CurrentStateOfBoard == 0) // Spiel ist beendet
				{
					setActivePlayer(false);
					setActiveEnemy(false);
					CurrentTurn++;
					MultiDispatcher.Broadcast(CurrentStateOfBoard);

					// Player Looses
					if (CurrentStateOfBoard == -1)
					{
						EnemyReference->AddToLife(1);
						PlayerReference->SubtractFromLife(1);
						RefreshTextActor();
					}
					// Player Wins
					else if (CurrentStateOfBoard == 1)
					{
						PlayerReference->AddToLife(1);
						EnemyReference->SubtractFromLife(1);
						RefreshTextActor();
					}

					return;				
				}
				if (IsSucess)
				{
					PlayerReference->AddToScore(1);
					RefreshTextActor();
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
				if (HardMode == true)
				{
					FString NextMove;
					BoardReference->SaveBoard();
					NextMove = EnemyReference->MakeExtremeAIMove(BoardReference->getBoard());
					BoardReference->RestoreBoard();
					BoardReference->CalcMove(NextMove, false, false);
					UE_LOG(LogTemp, Warning, TEXT("AI MADE: %s"), *NextMove);
				}
				else
				{
					BoardReference->CalcMove(EnemyReference->MakeEasyAIMove(), false,false);
				}
			}
			int CurrentStateOfBoard = BoardReference->CheckWin();
			if (CurrentStateOfBoard == -1 || CurrentStateOfBoard == 1 || CurrentStateOfBoard == 0)
			{
				setActivePlayer(false);
				setActiveEnemy(false);
				CurrentTurn++;
				MultiDispatcher.Broadcast(CurrentStateOfBoard);
				// Player Looses
				if (CurrentStateOfBoard == -1)
				{
					EnemyReference->AddToLife(1);
					PlayerReference->SubtractFromLife(1);
					RefreshTextActor();
				}
				// Player Wins
				else if (CurrentStateOfBoard == 1)
				{
					PlayerReference->AddToLife(1);
					EnemyReference->SubtractFromLife(1);
					RefreshTextActor();
				}
				return;
			}
			setActiveEnemy(false);
			setActivePlayer(true);
			RefreshTextActor();
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


void AGameLogic::RefreshTextActor()
{
	if (PlayerReference != nullptr && ScoreCounter != nullptr)
	{
		UTextRenderComponent* ScoreCountTemp = ScoreCounter->GetTextRender();
		ScoreCountTemp->SetText(FText::Format(NSLOCTEXT("Namespace", "Key", "Your Current Score: {0}"), FText::AsNumber(PlayerReference->GetScore())));
	}

	if (EnemyReference != nullptr && LifeCounterEnemy != nullptr)
	{
		UTextRenderComponent* LifeCounterEnemyTemp = LifeCounterEnemy->GetTextRender();
		LifeCounterEnemyTemp->SetText(FText::Format(NSLOCTEXT("Namespace", "Key", "Enemy Life: {0}"), FText::AsNumber(EnemyReference->GetLife())));
	}

	if (PlayerReference != nullptr && LifeCounterPlayer != nullptr)
	{
		UTextRenderComponent* LifeCounterPlayerTemp = LifeCounterPlayer->GetTextRender();
		LifeCounterPlayerTemp->SetText(FText::Format(NSLOCTEXT("Namespace", "Key", "Player Life: {0}"), FText::AsNumber(PlayerReference->GetLife())));
	}

	if (TurnCounter != nullptr)
	{
		UTextRenderComponent* TurnCounterTemp = TurnCounter->GetTextRender();
		TurnCounterTemp->SetText(FText::Format(NSLOCTEXT("Namespace", "Key", "Current Turn: {0} / 5"), FText::AsNumber(CurrentTurn)));
	}
}