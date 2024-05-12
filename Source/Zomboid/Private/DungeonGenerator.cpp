// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonGenerator.h"
#include "TileMarks.h"
#include "DungeonTile.h"



// Sets default values
ADungeonGenerator::ADungeonGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlacementDirection = {
	{"FORWARD", FVector(150, 0, 0)},
	{"RIGHT", FVector(0, 150, 0)},
	{"BACKWARDS", FVector(-150, 0, 0)},
	{"LEFT", FVector(0, -150, 0)}
	};

}

// Called when the game starts or when spawned
void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADungeonGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADungeonGenerator::Prepare()
{
	for (TActorIterator<ADungeonTile> pups(GetWorld()); pups; ++pups)
	{
		ADungeonTile* Temp = *pups;
		Temp->SetNumberOfExits();
	}
}

void ADungeonGenerator::SpawnDungeonTiles(int Amount)
{
	LastPieceMade = nullptr;
	UWorld* CurrentWorld = this->GetWorld();
	if (CurrentWorld == nullptr)
	{
		return;
	}

	FVector StartPosition = FVector(0, 0, 0);
	int MaxAmount = AvaiableTiles.Num() - 1;

	for (int i = 0; i < Amount; i++)
	{
		if (i != 0)
		{
			bool OnlyOneExit = true;
			ADungeonTile* NewTile = nullptr;
			while (OnlyOneExit) // check if the Choosen Tile has more than one Exit
			{
				bool CorrectPosition = false;
				int RandomChoice = FMath::RandRange(0, AvaiableTiles.Num() - 1);

				UE_LOG(LogTemp, Warning, TEXT("The Amount of AvaiableTiles are :%d"), AvaiableTiles.Num());
				UE_LOG(LogTemp, Warning, TEXT("The Random Choice was :%d"), RandomChoice);


				PickedClass = AvaiableTiles[RandomChoice];
				NewTile = CurrentWorld->SpawnActor<ADungeonTile>(PickedClass);

				if (CheckForAmountOfExits(NewTile) > 1)
				{
					OnlyOneExit = false;
				}
				else
				{
					CurrentWorld->DestroyActor(NewTile);
				}
			}
			if (NewTile != nullptr)
			{
					// Do Stuff with the New Tile
					//NewTile->SetActorLocation(StartPosition);
					FString TempStr = FString::Printf(TEXT("Dungeon Tile Number %d"), i);
					NewTile->Tags.Add(FName(TempStr));
					if (CheckPossiblePositions(LastPieceMade, NewTile))
					{
						// If Every Mark of the LastPieceMade and the NewTile could be parsed correctly
						SetPositionAndRotation(NewTile);

					}
					else
					{
						// If Any Mark couldnt be correctly Parsed
						FString DebugNameNT = NewTile->GetName();
						FString DebugNameLPM = LastPieceMade->GetName();
						UE_LOG(LogTemp, Warning, TEXT("There were Some Problems with Parsing the Marks from New Tile: %s and Last Piece: %s"), *DebugNameNT, *DebugNameLPM);
						return;
					}



					// After Everything Finished Assign this Tile to be the "LastPieceMade"
					LastPieceMade = NewTile;
				 
			}

			
		}
		// Will only Be Picked Once in the Beginning
		else
		{
			for (auto Element : AvaiableTiles)
			{
				ADungeonTile* NewTile = GetWorld()->SpawnActor<ADungeonTile>(Element);

				if (NewTile->NumberOfExits != 1)
				{
					NewTile->Tags.Add(FName("SHITTY MC DUDEL"));
					GetWorld()->DestroyActor(NewTile);
				}
				else if (NewTile->NumberOfExits == 1)
				{
					//NewTile->SetActorLocation(StartPosition);
					NewTile->Tags.Add(FName("ERSTER!!!111!!"));
					LastPieceMade = NewTile;
					LastInstruction = FString("FORWARD");
					break;
				}
			}
		}
		// The Offset for Placing the New Tiles
	}

}

bool ADungeonGenerator::CheckPossiblePositions(ADungeonTile* LastMadePiece, ADungeonTile* PotentialNewTile)
{
	/*
	Clearing Variables to Fill for those two Specific Tiles
	*/
	LM_MarkWorldPositions.Empty();
	New_MarkWorldPositions.Empty();
	LM_MarkDirectionPosition.Empty();
	New_MarkDirectionPosition.Empty();
	LM_MarkWorldPositions.Empty();
	New_MarkWorldPositions.Empty();
	LastMadeFreeExits.Empty();
	NewFreeExits.Empty();

	/*
	Getting All Infos on the Last Made Piece
	*/
	LastMadeActTransform = LastMadePiece->GetActorTransform();
	LastMadeFreeExits = LastMadePiece->GetFreeExits();
	// Looping through all Free Exits and Saving their World Positions
	for (auto FreeExits : LastMadeFreeExits)
	{
		LM_MarkWorldPositions.Add(FreeExits->GetComponentLocation());

		if (PlacementDirection.FindKey(FreeExits->GetRelativeLocation()) != nullptr)
		{
			FString TempString = *PlacementDirection.FindKey(FreeExits->GetRelativeLocation());
			LM_MarkDirectionPosition.Add(TempString);

		}
		else
		{
			return false;
		}
	}

	/*
		Getting All Infos on Potential New Tile
	*/
	NewFreeExits = PotentialNewTile->GetFreeExits();
	NewActTransform = PotentialNewTile->GetActorTransform();
	for (auto FreeExits : NewFreeExits)
	{
		New_MarkWorldPositions.Add(FreeExits->GetComponentLocation());

		if (PlacementDirection.FindKey(FreeExits->GetRelativeLocation()) != nullptr)
		{
			FString TempString = *PlacementDirection.FindKey(FreeExits->GetRelativeLocation());
			New_MarkDirectionPosition.Add(TempString);
		}
		else
		{
			return false;
		}
	}


	return true;
}

void ADungeonGenerator::ClearDungeonTiles()
{
	for (TActorIterator<ADungeonTile> itr(GetWorld()); itr; ++itr)
	{
		GetWorld()->DestroyActor(*itr);
	}
}


void ADungeonGenerator::CheckForNeighbors(ADungeonTile* StartPos)
{
	Neighbors.Empty();
	float MinDistance = FLT_MAX;
	for (TActorIterator<ADungeonTile> Itr(GetWorld()); Itr; ++Itr)
	{
		if (*Itr != StartPos)
		{
			if (Itr->GetDistanceTo(StartPos) < MinDistance)
			{
				MinDistance = Itr->GetDistanceTo(StartPos);
			}
		}
	}
	for (TActorIterator<ADungeonTile> Itr(GetWorld()); Itr; ++Itr)
	{
		if (*Itr != StartPos)
		{
			if (Itr->GetDistanceTo(StartPos) == MinDistance)
			{
				ADungeonTile* TempTile = *Itr;
				Neighbors.Add(TempTile);
			}
		}
	}
}


FVector ADungeonGenerator::SetPositionAndRotation(ADungeonTile* NewTile)
{
	int RandomPick = INT_MIN;
	FVector Instruction;
	if (New_MarkDirectionPosition.Num() > 1)
	{
		do
		{
			RandomPick = FMath::RandRange(0, LM_MarkDirectionPosition.Num() - 1);
			Instruction = *PlacementDirection.Find(LM_MarkDirectionPosition[RandomPick]);

		} while (!StopBackwards(LM_MarkDirectionPosition[RandomPick]));
		Instruction = (Instruction * 2) + LastMadeActTransform.GetLocation();

		NewTile->SetActorLocation(Instruction);
		NewActTransform = NewTile->GetActorTransform();

		LastInstruction = LM_MarkDirectionPosition[RandomPick];
	}
	else if (New_MarkDirectionPosition.Num() == 1)
	{
		Instruction = *PlacementDirection.Find(LM_MarkDirectionPosition[0]);
		NewTile->SetActorLocation(FVector(0, 0, 0));
	}


	return Instruction;
}

int ADungeonGenerator::CheckForAmountOfExits(ADungeonTile* TileToCheck)
{
	return TileToCheck->GetFreeExits().Num();
}


bool ADungeonGenerator::StopBackwards(FString Instruction)
{ 
	TArray<FString> TempArray;
	PlacementDirection.GenerateKeyArray(TempArray);

	int LastInstructionIndex = TempArray.Find(LastInstruction);
	int NewInstructionIndex = TempArray.Find(Instruction);

	if (LastInstructionIndex == NewInstructionIndex - 2 || LastInstructionIndex == NewInstructionIndex + 2)
	{
		return false;
	}
	else
	{
		return true;
	}

}

bool ADungeonGenerator::CheckCorrectOrientation(ADungeonTile* NewTile)
{

}