// Fill out your copyright notice in the Description page of Project Settings.
//TEST

#include "Board.h"
#include "MyStructs.h"

ABoard::ABoard()
{
    // Enable ticking for this actor
    PrimaryActorTick.bCanEverTick = true;

    // Create a root scene component and set it as the root component
    MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootObject"));
    RootComponent = MyRoot;
}

void ABoard::BeginPlay()
{
    Super::BeginPlay();

    // Initialize the playboard
    InitPlayboard();
}

void ABoard::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initializing the Playboard by Cleaning the Current Board, Creating and naming the Components and Register them to the 2D Array
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ABoard::InitPlayboard()
{

    float CenteringOffsetRow = (Board.Num() * 100) / 2;
    float CenteringOffsetColumn = (Board[0].Columns.Num() * 100) / 2;
    // Clean up any existing components
    CleanComponents(Board);

    // Resize the board array to contain the specified number of rows
    Board.SetNum(AmountOfRows);

    // Iterate over each row
    for (int i = 0; i < Board.Num(); i++)
    {
        // Resize the columns array for the current row
        Board[i].Columns.SetNum(AmountOfColumns);

        // Calculate the offset for the current row

        FVector RowsOffset = FVector(0, 0, 0);
        RowsOffset.Z = (100 * (i + 1)) - CenteringOffsetRow - 50.f;

        // Iterate over each column in the current row
        for (int j = 0; j < Board[i].Columns.Num(); j++)
        {
            // Calculate the offset for the current column
            FVector ColumnOffset = FVector(0, 0, 0);
            ColumnOffset.X = (100 * (j + 1)) - CenteringOffsetColumn - 50.f;

            // Generate a unique name for the component
            FString ComponentName = FString::Printf(TEXT("%d,%d"), i, j);

            // Create a new static mesh component
            UStaticMeshComponent* NewComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *ComponentName);
            if (NewComponent)
            {
                // Register and add the new component
                NewComponent->RegisterComponent();
                AddInstanceComponent(NewComponent);
                NewComponent->AttachToComponent(MyRoot, FAttachmentTransformRules::KeepRelativeTransform);

                // Load the static mesh for the component
                UStaticMesh* Cube = LoadObject<UStaticMesh>(this, TEXT("/Game/LevelPrototyping/Meshes/SM_ChamferCube.SM_ChamferCube"));

                // Set the static mesh and location for the component
                NewComponent->SetStaticMesh(Cube);
                NewComponent->SetRelativeLocation(RowsOffset + ColumnOffset);

                // Store the created component in the board array
                Board[i].Columns[j] = NewComponent;

                // Log the name of the created component
                UE_LOG(LogTemp, Warning, TEXT("Component Name: %s"), *Board[i].Columns[j]->GetName());
            }
        }
    }

    // Trigger updating the render state
    MarkComponentsRenderStateDirty();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Completly Destroys all components and also emptying the Board Array
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ABoard::CleanComponents(TArray<FRows>& Boardinput)
{
    // Iterate over each row
    for (int i = 0; i < Boardinput.Num(); i++)
    {
        // Iterate over each column in the current row
        for (int j = 0; j < Boardinput[i].Columns.Num(); j++)
        {
            // Get a pointer to the current component
            UStaticMeshComponent* CurrentComponent = Boardinput[i].Columns[j];

            // Check if the current component is valid
            if (CurrentComponent)
            {
                // Destroy and unregister the current component
                CurrentComponent->DestroyComponent();
                CurrentComponent->UnregisterComponent();
                RemoveInstanceComponent(CurrentComponent);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Error when deleting components"));
            }
        }

        // Clear the columns array for the current row
        Boardinput[i].Columns.Empty();
    }

    // Clear the board array
    Boardinput.Empty();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Parsing a String like (2,1) into two integers 2 and 1 !IMPORTANT! There are misbehaviors with field greater than 9 columns and rows 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TArray<int> ABoard::ParseInputMove(FString Field)
{
    if (GEngine)
    {
        // Print the input field to the screen for debugging
        GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Cyan, Field);
    }

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
    if (GEngine && ValueWasSet)
    {
        FString Value1Str = FString::Printf(TEXT("Value 1: %i"), Value1);
        FString Value2Str = FString::Printf(TEXT("Value 2: %i"), Value2);
        GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Cyan, Value1Str);
        GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Magenta, Value2Str);
    }

    // Return the parsed values as an array
    TArray<int> returnArray;
    returnArray.Add(Value1);
    returnArray.Add(Value2);
    return returnArray;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sets the input as a Move, Checks if the Move was Valid, Adds it a List of all Taken Moves, Changes Material Depending on if its X or Oand Checks if someone Won
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ABoard::CalcMove(FString Field, bool IsX)
{
    // Check if the move is valid
    if (validateMove(Field))
    {
        // Add the move to the taken list
        takenListFull.AddUnique(Field);

        // Parse the input field to get row and column indices
        TArray<int> tempValue = ParseInputMove(Field);

        // Set material based on player type (X or O) And Tag For Win Condition
        if (IsX)
        {
            Board[tempValue[0]].Columns[tempValue[1]]->SetMaterial(0, XMaterial);
            Board[tempValue[0]].Columns[tempValue[1]]->ComponentTags.Add(FName("X"));
        }
        else
        {
            Board[tempValue[0]].Columns[tempValue[1]]->SetMaterial(0, OMaterial);
            Board[tempValue[0]].Columns[tempValue[1]]->ComponentTags.Add(FName("O"));
        }
        return true;
    }
    else
    {
        // Print error message if move is already taken and also Returns False, so the other Player (either Player or Enemy cant do another turn
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Cyan, FString(TEXT("ALREADY TAKEN")));
        }
        return false;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Checks if the Move made is not already taken
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ABoard::validateMove(FString Field)
{
    // Check if the move is not already taken
    if (!takenListFull.Contains(Field))
    {
        UE_LOG(LogTemp, Warning, TEXT("Move is not taken"));
        return true;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Move is already taken"));
        return false;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// simple Return to get the current Board
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TArray<FRows> ABoard::getBoard()
{
    // Return the board array
    return Board;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// simple Return to get the current TakenList
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TArray<FString> ABoard::GetTakenList()
{
    // Return the list of taken moves
    return takenListFull;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Win Condition for X and O 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function should be changed to Return type Enum or Int to Differentiate between the 3 Possible States (X:1  Win, O:-1 Win, Tie:0 and keep Running is 2 )
int ABoard::CheckWin()
{
    bool IsWon = false;


    

    int XCounter = 0;
    int OCounter = 0;

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Checking Every Row (Can be Extended with a Variable instead of Hardcoding 3 as a Win Condition)
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Goes through every Row and checks every Cell of this Row if it contains A X or A O -> Will Count Upwards for every Row to see if it Contains 3
    for (int i = 0; i < Board.Num(); i++)
    {
        // Resetting The Counter for every Row
        XCounter = 0;
        OCounter = 0;

        // Jede Zelle von jeder Reihe
        for (int j = 0; j < Board[i].Columns.Num(); j++)
        {
            if (Board[i][j]->ComponentHasTag(FName("X")) == true)
            {
                XCounter++;
            }
            if (Board[i][j]->ComponentHasTag(FName("O")) == true)
            {
                OCounter++;
            }
        }
        // Checking if Someone Won
        if (XCounter == 3)
        {
            // X Wins
            IsWon = true;
            UE_LOG(LogTemp, Warning, TEXT("X Won"));
            return 1;
        }
        if (OCounter == 3)
        {
            // O Wins
            IsWon = true;
            UE_LOG(LogTemp, Warning, TEXT("O Won"));
            return -1;
        }
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Checking Every Column (Can be Extended with a Variable instead of Hardcoding 3 as a Win Condition)
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Checking Every Column and Assuming every Row has the same Amount of Columns (Grid)
    for (int j = 0; j < Board[0].Columns.Num(); j++)
    {
        // Resetting The Counter for every Column
        XCounter = 0;
        OCounter = 0;

        // Jede Zelle von jeder Spalte
        for (int i = 0; i < Board.Num(); i++)
        {
            if (Board[i][j]->ComponentHasTag(FName("X")))
            {
                XCounter++;
            }
            if (Board[i][j]->ComponentHasTag(FName("O")))
            {
                OCounter++;
            }
        }

        if (XCounter == 3)
        {
            // X Wins
            IsWon = true;
            UE_LOG(LogTemp, Warning, TEXT("X Won"));
            return 1;
        }
        if (OCounter == 3)
        {
            // O Wins
            IsWon = true;
            UE_LOG(LogTemp, Warning, TEXT("O Won"));
            return -1;
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Checking Diagonal Lines (Can be Extended with a Variable instead of Hardcoding 3 as a Win Condition)
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Resetting the Counter before Checking Diagonal Lines !IMPORTANT! I am assuming that every Playboard is a Square, should throw out of Bound Exception when not!
    XCounter = 0;
    OCounter = 0;

    // Going through every Row and using the Current Index for the Current Columns (0,0 ; 1,1 ; 2,2)
    for (int i = 0; i < Board.Num(); i++)
    {
        if (Board[i][i]->ComponentHasTag(FName("X")))
        {
            XCounter++;
        }
        if (Board[i][i]->ComponentHasTag(FName("O")))
        {
            OCounter++;
        }
    }

    if (XCounter == 3)
    {
        // X Wins
        IsWon = true;
        UE_LOG(LogTemp, Warning, TEXT("X Won"));
        return 1;
    }
    if (OCounter == 3)
    {
        // O Wins
        IsWon = true;
        UE_LOG(LogTemp, Warning, TEXT("O Won"));
        return -1;
    }




    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Checking Anti Diagonal Lines (Can be Extended with a Variable instead of Hardcoding 3 as a Win Condition)
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
    // Resetting the Counter before Checking Diagonal Lines !IMPORTANT! I am assuming that every Playboard is a Square, should throw out of Bound Exception when not!
    XCounter = 0;
    OCounter = 0;

    // Going through every Row and using the Current Index for the Row
    // for the Columns using the Size of the Array - 1 (Cause Array starts at 0) and subtracting the index from it (0,2; 1,1; 2,0)

    for (int i = 0; i < Board.Num(); i++)
    {

        if (Board[i][Board.Num() - 1 - i]->ComponentHasTag(FName("X")))
        {
            XCounter++;
        }
        if (Board[i][Board.Num() - 1 - i]->ComponentHasTag(FName("O")))
        {
            OCounter++;
        }

    }

    if (XCounter == 3)
    {
        // X Wins
        IsWon = true;
        UE_LOG(LogTemp, Warning, TEXT("X Won"));
        return 1;
    }
    if (OCounter == 3)
    {
        // O Wins
        IsWon = true;
        UE_LOG(LogTemp, Warning, TEXT("O Won"));
        return -1;
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Tie Condition
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (IsWon == false && takenListFull.Num() == AmountOfRows * AmountOfColumns)
    {
        UE_LOG(LogTemp, Warning, TEXT("Taken List Full is: %i"), takenListFull.Num());
        UE_LOG(LogTemp, Warning, TEXT("Its a TIE!"));
        return 0;
    }


    // No One Won and Board not full
    return 2;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Debug Function to visualize a specific Cell
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ABoard::TestRow()
{
    // Test function to set material for a specific row and column
    Board[Rows].Columns[Columns]->SetMaterial(0, XMaterial);
}
