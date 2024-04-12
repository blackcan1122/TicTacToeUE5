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
    float CenteringOffsetRow = (AmountOfRows * 100) / 2;
    float CenteringOffsetColumn = (AmountOfColumns * 100) / 2;
    // Clean up any existing components
    CleanComponents(Board);

    // Resize the board array to contain the specified number of rows
    Board.SetNum(AmountOfRows);

    // Iterate over each row
    if (AmountOfRows == 0 || AmountOfColumns == 0)
    {
        return;
    }
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
            if (NewComponent && GetWorld())
            {
                // Register and add the new component
                NewComponent->RegisterComponent();
                AddInstanceComponent(NewComponent);
                NewComponent->AttachToComponent(MyRoot, FAttachmentTransformRules::KeepRelativeTransform);

                // Set the static mesh and location for the component
                NewComponent->SetStaticMesh(BaseObject);
                NewComponent->SetMaterial(0, BaseMaterial);
                NewComponent->SetRelativeLocation(RowsOffset + ColumnOffset);

                // Store the created component in the board array
                Board[i].Columns[j] = NewComponent;

                // Log the name of the created component
                //UE_LOG(LogTemp, Warning, TEXT("Component Name: %s"), *Board[i].Columns[j]->GetName());
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
    if (Boardinput.Num() == 0)
    {
        return;
    }
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
    takenListFull.Empty();
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
bool ABoard::CalcMove(FString Field, bool IsX, bool IsSimulate)
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
            if (IsSimulate == false)
            {
            Board[tempValue[0]].Columns[tempValue[1]]->AddRelativeRotation(FRotator(0.f, 0.f, 90.f));
            }
        }
        else
        {
            Board[tempValue[0]].Columns[tempValue[1]]->SetMaterial(0, OMaterial);
            Board[tempValue[0]].Columns[tempValue[1]]->ComponentTags.Add(FName("O"));
            if (IsSimulate == false)
            {
                Board[tempValue[0]].Columns[tempValue[1]]->AddRelativeRotation(FRotator(0.f, 0.f, -90.f));
            }
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
    //// Check if the move is not already taken
    //if (!takenListFull.Contains(Field))
    //{
    //    ////UE_LOG(LogTemp, Warning, TEXT("Move is not taken"));
    //    return true;
    //}
    //else
    //{
    //    ////UE_LOG(LogTemp, Warning, TEXT("Move is already taken"));
    //    return false;
    //}

    TArray<int> tempValue = ParseInputMove(Field);
    if (Board[tempValue[0]][tempValue[1]]->ComponentTags.Contains("X") || Board[tempValue[0]][tempValue[1]]->ComponentTags.Contains("O"))
    {
        return false;
    }
    else
    {
        return true;
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
// Function should be changed to Return type Enum or Int to Differentiate between the 3 Possible States (X:1 Win, O:-1 Win, Tie:0 and keep Running is 2 )
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
            //UE_LOG(LogTemp, Warning, TEXT("X Won"));
            return 1;
        }
        if (OCounter == 3)
        {
            // O Wins
            IsWon = true;
            //UE_LOG(LogTemp, Warning, TEXT("O Won"));
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
            //UE_LOG(LogTemp, Warning, TEXT("X Won"));
            return 1;
        }
        if (OCounter == 3)
        {
            // O Wins
            IsWon = true;
            //UE_LOG(LogTemp, Warning, TEXT("O Won"));
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
        //UE_LOG(LogTemp, Warning, TEXT("X Won"));
        return 1;
    }
    if (OCounter == 3)
    {
        // O Wins
        IsWon = true;
        //UE_LOG(LogTemp, Warning, TEXT("O Won"));
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
        //UE_LOG(LogTemp, Warning, TEXT("X Won"));
        return 1;
    }
    if (OCounter == 3)
    {
        // O Wins
        IsWon = true;
        //UE_LOG(LogTemp, Warning, TEXT("O Won"));
        return -1;
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Tie Condition
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (IsWon == false && takenListFull.Num() == AmountOfRows * AmountOfColumns)
    {
        //UE_LOG(LogTemp, Warning, TEXT("Its a TIE!"));
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

void ABoard::SaveBoard()
{
    SavedBoard = Board;
    SavedTakenList = takenListFull;
}

void ABoard::RestoreBoard()
{
    Board = SavedBoard;
    takenListFull = SavedTakenList;
}

bool ABoard::ValidInputAI(FString& input)
{
    if (takenListFull.Contains(input) == true)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void ABoard::ResetLastMadeMove(FString& currentMove)
{
    if (takenListFull.Contains(currentMove))
    {
        int i = ParseInputMove(currentMove)[0];
        int j = ParseInputMove(currentMove)[1];
        takenListFull.Remove(currentMove);
        Board[i][j]->ComponentTags.Empty();
        Board[i][j]->SetMaterial(0, BaseMaterial);
    }
    else
    {
        //UE_LOG(LogTemp, Warning, TEXT("WARNING: couldnt remove AI Move from Taken List"));
        return;
    }

}

float ABoard::TurnCubeOverTime(float Value1)
{
    return 1.f;
}

void ABoard::SwitchTwoColumns(int Column1, int Column2)
{
    TArray<UMaterialInstance*> MaterialsRow1;
    TArray<FRotator> RotationRow1;
    TArray<FName> TagsRow1;

    TArray<UMaterialInstance*> MaterialsRow2;
    TArray<FRotator> RotationRow2;
    TArray<FName> TagsRow2;

    for (int i = 0; i < Board[Column1].Columns.Num(); i++)
    { 
        MaterialsRow1.Add(Cast<UMaterialInstance>(Board[Column1][i]->GetMaterial(0)));
        RotationRow1.Add(Board[Column1][i]->GetRelativeRotation());
        if (Board[Column1][i]->ComponentTags.Num() > 0)
        {
            TagsRow1.Add(Board[Column1][i]->ComponentTags[0]);
        }
        else
        {
            TagsRow1.Add(" ");
        }

    }
    for (int i = 0; i < Board[Column2].Columns.Num(); i++)
    {
        MaterialsRow2.Add(Cast<UMaterialInstance>(Board[Column2][i]->GetMaterial(0)));
        RotationRow2.Add(Board[Column2][i]->GetRelativeRotation());
        if (Board[Column2][i]->ComponentTags.Num() > 0)
        {
            TagsRow2.Add(Board[Column2][i]->ComponentTags[0]);
        }
        else
        {
            TagsRow2.Add(" ");
        }


        Board[Column2][i]->SetMaterial(0, MaterialsRow1[i]);
        Board[Column2][i]->SetRelativeRotation(RotationRow1[i]);
        Board[Column2][i]->ComponentTags.Empty();
        Board[Column2][i]->ComponentTags.Add(TagsRow1[i]);
    }

    for (int i = 0; i < Board[Column1].Columns.Num(); i++)
    {
        Board[Column1][i]->SetMaterial(0, MaterialsRow2[i]);
        Board[Column1][i]->SetRelativeRotation(RotationRow2[i]);
        Board[Column1][i]->ComponentTags.Empty();
        Board[Column1][i]->ComponentTags.Add(TagsRow2[i]);
    }
}

void ABoard::MarkColumn(int Row)
{
    for (int i = 0; i < Board.Num(); i++)
    {
        for (int j = 0; j < Board[i].Columns.Num(); j++)
        {
            Board[i][j]->SetOverlayMaterial(NULL);
        }
    }
    for (int i = 0; i < Board.Num(); i++)
    {
        for (int j = 0; j < Board[i].Columns.Num(); j++)
        {
            Board[Row][i]->SetOverlayMaterial(OverlayMaterialRow);
        }
    }
}

void ABoard::ClearOverlayMaterials()
{
    for (int i = 0; i < Board.Num(); i++)
    {
        for (int j = 0; j < Board[i].Columns.Num(); j++)
        {
            Board[i][j]->SetOverlayMaterial(NULL);
        }
    }
}