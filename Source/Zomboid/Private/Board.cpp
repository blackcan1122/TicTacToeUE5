// Fill out your copyright notice in the Description page of Project Settings.

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

void ABoard::InitPlayboard()
{
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
        RowsOffset.Z = 100 * (i + 1);

        // Iterate over each column in the current row
        for (int j = 0; j < Board[i].Columns.Num(); j++)
        {
            // Calculate the offset for the current column
            FVector ColumnOffset = FVector(0, 0, 0);
            ColumnOffset.X = 100 * (j + 1);

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

bool ABoard::CalcMove(FString Field, bool IsX)
{
    // Check if the move is valid
    if (validateMove(Field))
    {
        // Add the move to the taken list
        takenListFull.Add(Field);

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
        // Print error message if move is already taken
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Cyan, FString(TEXT("ALREADY TAKEN")));
        }
        return false;
    }
}

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

TArray<FRows> ABoard::getBoard()
{
    // Return the board array
    return Board;
}

TArray<FString> ABoard::GetTakenList()
{
    // Return the list of taken moves
    return takenListFull;
}

bool ABoard::CheckWin()
{
    // To be implemented
    return true;
}

void ABoard::TestRow()
{
    // Test function to set material for a specific row and column
    Board[Rows].Columns[Columns]->SetMaterial(0, XMaterial);
}
