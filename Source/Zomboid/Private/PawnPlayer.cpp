// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnPlayer.h"
#include "UObject/UObjectIterator.h"

// Sets default values
APawnPlayer::APawnPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MySpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	MySpringArm->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	MySpringArm->TargetArmLength = MySpringArmLength;

	MyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCam"));
	MyCamera->AttachToComponent(MySpringArm, FAttachmentTransformRules::SnapToTargetNotIncludingScale);


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
	while (TakenSpace <= ColNum*RowNum && isValid == false)
	{
		int RandomRowNumber = FMath::RandRange(0, RowNum - 1);
		int RandomColNumber = FMath::RandRange(0, ColNum - 1);
		UE_LOG(LogTemp, Warning, TEXT("I AM IN A WHILE LOOP WOOOHOOO"));

		ResultString = FString::Printf(TEXT("%d,%d"), RandomRowNumber, RandomColNumber);

		isValid = BoardReference->validateMove(ResultString);
	}

	return ResultString;
}

FString APawnPlayer::MakeExtremeAIMove(TArray<FRows>& Currentboard)
{
    //loopCount = 0;  // Debug for Counting iterations

    int BestScore = INT_MIN;
    int CurrentScore;

    FString BestMove;
    FString CurrentMove;

    //TArray<FString> strVRows = currentBoard.validRowsStr();    // Dynamic getting all valid Rows (A,B,C...) instead of hardcoding it for a bigger sized playboard
    //TArray<FString> strVCols = currentBoard.validColStr();  // Dynamic getting all valid Rows (A,B,C...) instead of hardcoding it for a bigger sized playboard

    // Variables for the Alpha - Beta Pruning optimization
    int alpha = INT_MIN;
    int beta = INT_MAX;

    // Iterating through the 2D Vector (Playboard)
    for (int i = 0; i < Currentboard.Num(); i++)
    {
        for (int j = 0; j < Currentboard.Num(); j++)
        {
            //loopCount++;
            CurrentMove.Empty();
            CurrentMove.AppendInt(i);
            CurrentMove.Append(",");
            CurrentMove.AppendInt(j);

            //Validation if the field is already occupied
            if (BoardReference -> ValidInputAI(CurrentMove) == true)
            {
                BoardReference->CalcMove(CurrentMove, false);   // Calling a own Function for Updating the Playboard without taking items to the Taken List
                CurrentScore = minimax(Currentboard, 0, alpha, beta, false);    // Minimax Algorithm with alpha beta Pruning
                BoardReference->ResetLastMadeMove(CurrentMove);           // Reset the Playboard in a way to function with the recursive behavior

                if (CurrentScore > BestScore)
                {
                    BestScore = CurrentScore;
                    BestMove = CurrentMove;
                }

            }
        }

    }
    //std::cout << "Amount of Loops: " << loopCount << "\n";
    return  BestMove;

}

int APawnPlayer::minimax(TArray<FRows>& Board, int depth, int alpha, int beta, bool isMaximazing)
{

    // Win Condition
    int result = BoardReference->CheckWin();
    if (result == -1 || result == 1)
    {
        return result;

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
                if (BoardReference->ValidInputAI(CurrentMove) == true)
                {
                    BoardReference->CalcMove(CurrentMove, false);
                    CurrentScore = minimax(Board, depth + 1, alpha, beta, false);
                    BoardReference->ResetLastMadeMove(CurrentMove);

                    BestScore = FMath::Max(CurrentScore, BestScore);
                    alpha = FMath::Max(alpha, BestScore);
                    if (beta <= alpha)
                        break; // Beta cutoff
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
                if (BoardReference->ValidInputAI(CurrentMove) == true)
                {
                    BoardReference->CalcMove(CurrentMove, true);
                    CurrentScore = minimax(Board, depth + 1, alpha, beta, true);
                    BoardReference->ResetLastMadeMove(CurrentMove);

                    BestScore = FMath::Min(CurrentScore, BestScore);
                    beta = FMath::Min(beta, BestScore);
                    if (beta <= alpha)
                        break; // Alpha cutoff
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
