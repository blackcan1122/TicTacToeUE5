// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonGenerator.h"
#include "TileMarks.h"
#include "DungeonTile.h"
#include "RenderingThread.h"



// Sets default values
ADungeonGenerator::ADungeonGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LastPieceMade = nullptr;

	PickedClass = nullptr;

	PositionOffsetDungeonTile = 0;
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

// Init Function for Generating the Dungeon
void ADungeonGenerator::SpawnDungeonTiles(int Amount)
{

	LastPieceMade = nullptr;
	UWorld* CurrentWorld = this->GetWorld();
	if (CurrentWorld == nullptr)
	{
		return;
	}

	//How Many Starting Cells the Dungeon Layout should have
	for (int i = 0; i < Amount; i++)
	{
		// Making a Case for the First Iteration (Startpoint)
		if (i != 0)
		{
			ADungeonTile* NewTile = nullptr;

			// Returning a Random Tile with the Amount of Exits above a Specific Number
			NewTile = ReturnTileAboveExits(CurrentWorld, 1);

			if (NewTile != nullptr)
			{
				// Setting up Location and Rotation for the First Pass
				if (!ProcessNewTile(NewTile, i))
				{
					return;
				}

				// After Everything Finished Assign this Tile to be the "LastPieceMade"
				LastPieceMade = NewTile;
				 
			}

			
		}
		// Will only Be Picked Once in the Beginning
		else
		{
			SpawnDeadEnds();
		}
		// The Offset for Placing the New Tiles
	}

	for (TActorIterator<ADungeonTile> itr(GetWorld()); itr; ++itr)
	{
		if (CheckIfAllMarksConnected(*itr) == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("There Were Some Problems with %s"), *itr->GetName());
		}
	}
}

bool ADungeonGenerator::CheckPossiblePositions(ADungeonTile* LastMadePiece, ADungeonTile* PotentialNewTile)
{
	/*
		Clearing Variables to Fill for those two Specific Tiles
	*/

	LM_MarkWorldPositions.Empty();
	New_MarkWorldPositions.Empty();
	LastMadeFreeExits.Empty();
	NewFreeExits.Empty();

	/*
		Getting All Infos on the Last Made Piece
	*/
	LastMadeActTransform = LastMadePiece->GetActorTransform();
	LastMadeActTransform = LastMadePiece->GetActorTransform();
	LastMadeFreeExits = LastMadePiece->GetFreeExits();
	// Looping through all Free Exits and Saving their World Positions
	for (auto FreeExits : LastMadeFreeExits)
	{
		LM_MarkWorldPositions.Add(FreeExits->GetComponentLocation());
	}

	/*
		Getting All Infos on Potential New Tile
	*/
	NewFreeExits = PotentialNewTile->GetFreeExits();
	NewActTransform = PotentialNewTile->GetActorTransform();
	for (auto FreeExits : NewFreeExits)
	{
		New_MarkWorldPositions.Add(FreeExits->GetComponentLocation());
	}


	return true;
}

/*
	Clearing All Dungeon Tiles existing
*/
void ADungeonGenerator::ClearDungeonTiles()
{
	for (TActorIterator<ADungeonTile> itr(GetWorld()); itr; ++itr)
	{
		if (itr->ActorHasTag(FName("Persistent")) == false)
		{
			GetWorld()->DestroyActor(*itr);
		}
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
			if (Itr->GetDistanceTo(StartPos) <= MinDistance)
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


bool ADungeonGenerator::SetPositionAndRotation(ADungeonTile* NewTile)
{
	int RandomPick = INT_MIN;
	FVector Instruction;
	if (New_MarkWorldPositions.Num() > 1)
	{
		TArray<FVector> PossibleWorldPosLM = LM_MarkWorldPositions;
		do
		{
			RandomPick = FMath::RandRange(0, PossibleWorldPosLM.Num() - 1);
			Instruction = PossibleWorldPosLM[RandomPick];
			Instruction = Instruction - LastMadeActTransform.GetLocation();
			Instruction = Instruction * 2;
			Instruction = Instruction + LastMadeActTransform.GetLocation();

			if (!CheckValidPosition(Instruction))
			{
				PossibleWorldPosLM.RemoveAt(RandomPick);
				if (PossibleWorldPosLM.Num() == 0)
				{
					UE_LOG(LogTemp, Warning, TEXT("Something Went Horrible Wrong No correct Instructions Was found"));
					NewTile->Tags.Add("No correct Instructions Was found");
					FName NameOfLastPiece = FName(*LastPieceMade->GetName());
					NewTile->Tags.Add(NameOfLastPiece);
					return false;
				}
			}

		} while (!CheckValidPosition(Instruction));


		NewTile->SetActorLocation(Instruction);
		NewActTransform = NewTile->GetActorTransform();
		bool WrongRotation = true;
		if (!CheckCorrectOrientation(NewTile))
		{
			UE_LOG(LogTemp, Warning, TEXT("Something Went Horrible Wrong"));
			NewTile->Tags.Add("UUUUWWUUUUU NOOOO");
			return false;
		}
	}
	else if (New_MarkWorldPositions.Num() == 1)
	{
		// For DeadEnds and Beginning
		NewTile->SetActorLocation(FVector(0, 0, 0));
	}


	return true;
}

int ADungeonGenerator::CheckForAmountOfExits(ADungeonTile* TileToCheck)
{
	return TileToCheck->GetFreeExits().Num();
}


bool ADungeonGenerator::CheckValidPosition(FVector PossibleCoords)
{ 
	for (TActorIterator<ADungeonTile> itr(GetWorld()); itr; ++itr)
	{

		FVector Distance = PossibleCoords - itr->GetActorLocation();

		if (Distance.Length() < 150)
		{
			return false;
		}
	}
	return true;
}

bool ADungeonGenerator::CheckCorrectOrientation(ADungeonTile* NewTile)
{
	int i = 0;
	bool NotFound = true;

	TArray<UTileMarks*> TempMarksNewTile = NewTile->GetFreeExits();
	TArray<UTileMarks*> TempMarksOldTile = LastMadeFreeExits;
	do
	{
		for (const auto& MarkOldTile : TempMarksOldTile)
		{
			for (const auto& MarkNewTile : TempMarksNewTile)
			{

				MarkOldTile->GetComponentLocation();
				MarkNewTile->GetComponentLocation();

				FVector Distance = MarkOldTile->GetComponentLocation() - MarkNewTile->GetComponentLocation();
				Distance.Length();

				if (Distance.Length() < 25)
				{
					MarkOldTile->CurrentConnectionType.CurrentMarkType = EMarkType::InUse;
					MarkOldTile->CurrentConnectionType.ConnectedTo = NewTile;
					MarkNewTile->CurrentConnectionType.CurrentMarkType = EMarkType::InUse;
					MarkNewTile->CurrentConnectionType.ConnectedTo = LastPieceMade;

					return true;
				}
			}
		}

		FRotator CurrentRotation = NewTile->GetActorRotation();
		NewTile->SetActorRelativeRotation(CurrentRotation + FRotator(0, 90, 0));
		++i;

		if (i == 4)
		{
			UE_LOG(LogTemp, Warning, TEXT("NoRotation That Matches Was Found"));
			NotFound = false;
		}

	} while (NotFound);
	return false;
}


void ADungeonGenerator::PrintAllErrorTiles()
{
	ProblematicTiles.Empty();
	for (TActorIterator<ADungeonTile> itr(GetWorld()); itr; ++itr)
	{
		if (itr->ActorHasTag(FName("Something Went Horrible Wrong")) || itr->ActorHasTag(FName("No correct Instructions Was found")))
		{
			ProblematicTiles.Add(*itr);
		}
	}
}

void ADungeonGenerator::ChooseANewContinue()
{
	TArray<ADungeonTile*> PotentialFreeTiles;
	for (TActorIterator<ADungeonTile> itr(GetWorld()); itr; ++itr)
	{
		if (itr->GetFreeExits().Num() > 0)
		{
			for (auto PotentialMark : itr->GetFreeExits())
			{
				FVector WorldPosOfMark = PotentialMark->GetComponentLocation();
				FVector WorldPosOfActor = itr->GetActorLocation();

				FVector Instruction = WorldPosOfMark - WorldPosOfActor;
				Instruction = Instruction * 2;
				Instruction = Instruction + WorldPosOfActor;
				if (CheckValidPosition(Instruction))
				{
					PotentialFreeTiles.Add(*itr);
				}
			}

		}
	}

	int RandomPick = FMath::RandRange(0, PotentialFreeTiles.Num() - 1);

	LastPieceMade = PotentialFreeTiles[RandomPick];


}


bool ADungeonGenerator::SpawnDeadEnds()
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
			return true;
		}
	}
	return false;
}


// Returns A Spawned Tile which as A Amount of Exit above a Specific Amount
ADungeonTile* ADungeonGenerator::ReturnTileAboveExits(UWorld* CurrentWorld, int AboveAmount)
{
	// Array to make sure that a Not Valid Piece will be picked infinitley

	TArray<TSubclassOf<ADungeonTile>> TileList = AvaiableTiles; // Copy The Array of Avaiaible Tiles, so we can Alter it
	bool OnlyOneExit = true;
	while (TileList.Num() != 0) // check if the Choosen Tile has more than one Exit
	{
		int RandomChoice = FMath::RandRange(0, TileList.Num() - 1);

		PickedClass = TileList[RandomChoice];
		ADungeonTile* NewTile;
		NewTile = CurrentWorld->SpawnActor<ADungeonTile>(PickedClass);
		
		if (NewTile->GetFreeExits().Num() > AboveAmount)
		{
			return NewTile;
		}
		else
		{
			CurrentWorld->DestroyActor(NewTile);
		}
	
	}
	return FallBackSolution;
}


bool ADungeonGenerator::SpawnNextTile()
{
	return false;
}

bool ADungeonGenerator::ProcessNewTile(ADungeonTile* NewTile, int iteration)
{
	// Do Stuff with the New Tile

	FString TempStr = FString::Printf(TEXT("Dungeon Tile Number %d"), iteration);
	NewTile->Tags.Add(FName(TempStr));
	if (CheckPossiblePositions(LastPieceMade, NewTile))
	{
		// If Every Mark of the LastPieceMade and the NewTile could be parsed correctly
		if (SetPositionAndRotation(NewTile) == false)
		{
			NewTile->Tags.Add(FName("This One Has To Go Somewhere Else"));
			ChooseANewContinue();
			CheckPossiblePositions(LastPieceMade, NewTile);
			SetPositionAndRotation(NewTile);
		}
		return true;
	}
	else
	{
		// If Any Mark couldnt be correctly Parsed
		FString DebugNameNT = NewTile->GetName();
		FString DebugNameLPM = LastPieceMade->GetName();
		UE_LOG(LogTemp, Warning, TEXT("There were Some Problems with Parsing the Marks from New Tile: %s and Last Piece: %s"), *DebugNameNT, *DebugNameLPM);
		return false;
	}
}


bool ADungeonGenerator::CheckIfAllMarksConnected(ADungeonTile* StartPos)
{
	if (StartPos->GetFreeExits().Num() == 0)
	{
		return true;
	}

	CheckForNeighbors(StartPos);

	for (auto& Mark : StartPos->GetFreeExits())
	{
		for (auto Neighbor : Neighbors)
		{
			for (auto& NeighborMark : Neighbor->GetFreeExits())
			{
				if (IsWithinDistance(Mark->GetComponentLocation(), NeighborMark->GetComponentLocation()) < 20)
				{
					Mark->CurrentConnectionType.CurrentMarkType = EMarkType::InUse;
					Mark->CurrentConnectionType.ConnectedTo = Neighbor;

					NeighborMark->CurrentConnectionType.CurrentMarkType = EMarkType::InUse;
					NeighborMark->CurrentConnectionType.ConnectedTo = StartPos;
				}
			}
		}
	}

	if (StartPos->GetFreeExits().Num() == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
	//@todo: Should Add all Actors which return false into a Array to Either Fix them or Building Branching Corridors
	
}


float ADungeonGenerator::IsWithinDistance(const FVector& Pos1, const FVector& Pos2)
{
	FVector Distance = Pos1 - Pos2;
	return Distance.Length();
}