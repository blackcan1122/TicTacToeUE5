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

FString APawnPlayer::MakeAIMove()
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


void APawnPlayer::ReceiveBoardRef(ABoard* Board)
{
	BoardReference = Board;
}
