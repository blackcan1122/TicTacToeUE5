// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnPlayer.h"
#include "UObject/UObjectIterator.h"

// Sets default values
APawnPlayer::APawnPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MySpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    MySpringArm->SetupAttachment(RootComponent);
	MySpringArm->TargetArmLength = MySpringArmLength;

	MyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCam"));
    MyCamera->SetupAttachment(MySpringArm);


}

// Called when the game starts or when spawned


void APawnPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APawnPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ChangeSpringArmLength();

}

// Called to bind functionality to input
void APawnPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APawnPlayer::ChangeSpringArmLength()
{
	MySpringArm->TargetArmLength = MySpringArmLength;
}

void APawnPlayer::SetIsTurn(bool SetIsTurn)
{
	IsTurn = SetIsTurn;
}

bool APawnPlayer::GetIsTurn()
{

	return IsTurn;
}

void APawnPlayer::MakeMove(FString ComponentHit)
{
	if (GetIsTurn() == true)
	{
		GameLogicReference->ProcessInput(ComponentHit);
	}
	
}

void APawnPlayer::ReceiveGameLogicRef(AGameLogic* GameLogic)
{
	GameLogicReference = GameLogic;
}

FString APawnPlayer::MakeEasyAIMove()
{
	
	int ColNum = BoardReference->AmountOfColumns;
	int RowNum = BoardReference->AmountOfRows;
	int TakenSpace = BoardReference->GetTakenList().Num();

	bool isValid = false;
	FString ResultString = "0,0";
	while (isValid == false)
	{
		int RandomRowNumber = FMath::RandRange(0, RowNum - 1);
		int RandomColNumber = FMath::RandRange(0, ColNum - 1);
		//UE_LOG(LogTemp, Warning, TEXT("I AM IN A WHILE LOOP WOOOHOOO"));

		ResultString = FString::Printf(TEXT("%d,%d"), RandomRowNumber, RandomColNumber);

		isValid = BoardReference->validateMove(ResultString);
	}

	return ResultString;
}

FString APawnPlayer::MakeExtremeAIMove(TArray<FRows> Currentboard)
{
    Loopcount = 0;  // Debug for Counting iterations

    int BestScore = INT_MIN;
    int CurrentScore;

    FString BestMove;
    FString CurrentMove;

    // Variables for the Alpha - Beta Pruning optimization
    int alpha = INT_MIN;
    int beta = INT_MAX;

    //For First AI Move making a Random Decision to keep iterations small and keeping the Game Random
    if (BoardReference->GetTakenList().Num() <= 1)
    {
        UE_LOG(LogTemp, Warning, TEXT("Making Easy Move for First Move hahahaha"));
        return MakeEasyAIMove();
    }

    // Iterating through the 2D Vector (Playboard)
    for (int i = 0; i < Currentboard.Num(); i++)
    {
        for (int j = 0; j < Currentboard.Num(); j++)
        {
            Loopcount++;
            CurrentMove.Empty();
            CurrentMove.AppendInt(i);
            CurrentMove.Append(",");
            CurrentMove.AppendInt(j);
            //Validation if the field is already occupied
            if (BoardReference ->validateMove(CurrentMove) == true)
            {
                BoardReference->CalcMove(CurrentMove, false, true);  
                CurrentScore = minimax(BoardReference->getBoard(), 0, alpha, beta, false);    // Minimax Algorithm with alpha beta Pruning
                BoardReference->ResetLastMadeMove(CurrentMove);           // Reset the Playboard in a way to function with the recursive behavior

                if (CurrentScore > BestScore)
                {
                    BestScore = CurrentScore;
                    BestMove = CurrentMove;
                }

            }
        }

    }
    UE_LOG(LogTemp, Warning, TEXT("Amount of Loops are: %i"), Loopcount);
    return  BestMove;

}

int APawnPlayer::minimax(TArray<FRows> Board, int depth, int alpha, int beta, bool isMaximazing)
{

     //Win Condition
    TMap<int, int> score{ {-1,1},{0,0},{1,-1} };
    int result = BoardReference->CheckWin();
    if (result == score[-1] || result == score[1])
    {
        return score[result];

    }
    else if (result == 0)
    {
        return result;
    }

    //
    // Maximizing Player
    //
    else if (isMaximazing)
    {
        Loopcount++;
        int BestScore = INT_MIN;
        int CurrentScore;
        FString CurrentMove;

        // Iterating through the 2D Vector (Playboard)
        for (int i = 0; i < Board.Num(); i++)
        {
            for (int j = 0; j < Board.Num(); j++)
            {
                CurrentMove.Empty();
                CurrentMove.AppendInt(i);
                CurrentMove.Append(",");
                CurrentMove.AppendInt(j);

                //Validation if the field is already occupied
                if (BoardReference->validateMove(CurrentMove) == true)
                {
                    BoardReference->CalcMove(CurrentMove, false, true);
                    CurrentScore = minimax(BoardReference->getBoard(), depth + 1, alpha, beta, false);
                    BoardReference->ResetLastMadeMove(CurrentMove);

                    BestScore = FMath::Max(CurrentScore, BestScore);
                    //alpha = FMath::Min(alpha, BestScore);
                    //if (beta <= alpha)
                    //    break; // Beta cutoff
                }
            }
        }
        return BestScore;
    }

    //
    // Minimizing Player
    //
    else
    {
        Loopcount++;
        int BestScore = 2;
        int CurrentScore;
        FString CurrentMove;

        // Iterating through the 2D Vector (Playboard)
        for (int i = 0; i < Board.Num(); i++)
        {
            for (int j = 0; j < Board.Num(); j++)
            {
                CurrentMove.Empty();
                CurrentMove.AppendInt(i);
                CurrentMove.Append(",");
                CurrentMove.AppendInt(j);

                //Validation if the field is already occupied
                if (BoardReference->validateMove(CurrentMove) == true)
                {
                    BoardReference->CalcMove(CurrentMove, true, true);
                    CurrentScore = minimax(BoardReference->getBoard(), depth + 1, alpha, beta, true);
                    BoardReference->ResetLastMadeMove(CurrentMove);

                    BestScore = FMath::Min(CurrentScore, BestScore);
                    //beta = FMath::Max(beta, BestScore);
                    //if (beta <= alpha)
                    //    break; // Alpha cutoff
                }
            }
        }
        return BestScore;
    }
}


void APawnPlayer::ReceiveBoardRef(ABoard* Board)
{
	BoardReference = Board;
}


void APawnPlayer::AddToLife(int NewLife)
{
    Life = Life + NewLife;
}

void APawnPlayer::SubtractFromLife(int NewLife)
{
    Life = Life - NewLife;
    UE_LOG(LogTemp, Warning, TEXT("New Life is: %i"), Life);
}

int APawnPlayer::GetLife()
{
    return Life;
}

void APawnPlayer::AddToScore(int NewScore)
{
    Score = Score + NewScore;
}

int APawnPlayer::GetScore()
{
    return Score;
}