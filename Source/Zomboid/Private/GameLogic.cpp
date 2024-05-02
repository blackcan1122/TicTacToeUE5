// Fill out your copyright notice in the Description page of Project Settings.

#include "GameLogic.h"
#include "PawnPlayer.h"
#include "BoardTarget.h"
#include "Board.h"
#include "MyStructs.h"
#include "UObject/UObjectGlobals.h"


// Sets default values
AGameLogic::AGameLogic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetupActorReferences();

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

	for (TObjectIterator<ABoardTarget> i; i; ++i)
	{
		BoardTargetReference = *i;
		UE_LOG(LogTemp, Warning, TEXT("Board Reference was set"));
	}
	if (BoardReference == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Board Reference is Null"));
	}

	// Setup Enemy Reference
	// Not Implemented Yet;

}

void AGameLogic::SendSelfReference()
{
	PlayerReference->ReceiveGameLogicRef(this);

	BoardTargetReference->ReceiveGameLogicRef(this);
}

void AGameLogic::ReceiveMove(FString& Field)
{
	if (ActiveGamemode == EChooseGameMode::Normal)
	{
		ProcessInputNormalTicTacToe(Field);
	}

	if (ActiveGamemode == EChooseGameMode::Crawler)
	{
		ProcessInputCrawlTicTacToe(Field);
	}
}

void AGameLogic::ProcessInputNormalTicTacToe(FString Field)
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

void AGameLogic::ProcessInputCrawlTicTacToe(FString Field)
{
	UE_LOG(LogTemp, Warning, TEXT("Crawl was Started"));
	//Make Sure this Function will be evaluated two times
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
				int CurrentStateOfBoard = (BoardReference->CheckWinFive()); // (Player Wins:1; Computer Wins: -1; Tie:0 and keep Running is 2)
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
					BoardReference->CalcMove(EnemyReference->MakeEasyAIMove(), false, false);
			}
			int CurrentStateOfBoard = BoardReference->CheckWinFive();
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

ABoard* AGameLogic::SendOtherBoardRefMethod()
{
	return BoardReference;
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

FString AGameLogic::ParseIntToValidField(int RowValue, int ColValue)
{
	return FString::Printf(TEXT("%d,%d"), RowValue, ColValue);
}

TArray<int> AGameLogic::ParseFieldToInt(FString Field)
{
	int Value1;
	int Value2;
	bool ValueWasSet = false;

	// Split the input string into two substrings based on the comma !IMPORTANT! Still misbehavior mit fields above 9 columns or rows
	FString SubstringFirst = Field.Left(Field.Find(TEXT(",")));
	FString SubstringSecond = Field.Right(Field.Find(TEXT(",")));

	// Check if both substrings contain numbers only and convert them to integers
	if (SubstringFirst.IsNumeric() && SubstringSecond.IsNumeric())
	{
		Value1 = FCString::Atoi(*SubstringFirst);
		Value2 = FCString::Atoi(*SubstringSecond);
		ValueWasSet = true;
	}

	// Debug printing of the parsed values
	//if (GEngine && ValueWasSet)
	//{
	//	FString Value1Str = FString::Printf(TEXT("Value 1: %i"), Value1);
	//	FString Value2Str = FString::Printf(TEXT("Value 2: %i"), Value2);
	//	GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Cyan, Value1Str);
	//	GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Magenta, Value2Str);
	//}

	// Return the parsed values as an array

	if (ValueWasSet)
	{
		TArray<int> returnArray;
		returnArray.Add(Value1);
		returnArray.Add(Value2);
		return returnArray;
	}
	else
	{
		TArray<int> ReturnArray = { 99,99 };
		return ReturnArray;
	}



}


bool AGameLogic::CheckForPossiblePlaces()
{
	TArray<FRows> BoardCopy = BoardReference->getBoard();

	for (int i = 0; i < BoardCopy.Num(); i++)
	{

	}
	return false;
}

// Defining Function for iterating through the board and checking if all spots are valid spots
TArray<FString> AGameLogic::CalculateMovesOnInstructions(FString Startfield)
{
	InstructionsToExecute.Empty();
	DirectionMap = BoardTargetReference->ReturnDirectionMap();
	TArray<FString> Instructions = BoardTargetReference->GetInstructions();

	TArray<int> StartMove = ParseFieldToInt(Startfield);
	FVector2D StartMove2D = FVector2D(StartMove[0], StartMove[1]);

	//InstructionsToExecute.Add(ParseIntToValidField(StartMove[0], StartMove[1]));

	for (auto& Instruction : Instructions)
	{
		FVector2D NextMove = *DirectionMap.Find(Instruction);
		StartMove2D = StartMove2D + NextMove;
		InstructionsToExecute.Add(ParseIntToValidField(StartMove2D.X, StartMove2D.Y));
	}

	return InstructionsToExecute;
}

bool AGameLogic::CheckForCorrectPattern()
{
	return false;
}