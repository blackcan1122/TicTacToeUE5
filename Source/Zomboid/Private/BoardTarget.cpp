// Fill out your copyright notice in the Description page of Project Settings.

#include "BoardTarget.h"
#include "GameLogic.h"

// Sets default values
ABoardTarget::ABoardTarget()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    DirectionMap = {
        {"UP", FVector2D(0, 1)},
        {"RIGHT", FVector2D(1, 0)},
        {"DOWN", FVector2D(0, -1)},
        {"LEFT", FVector2D(-1, 0)},
        {"ORIGIN", FVector2D(0,0)}
    };

}

// Called when the game starts or when spawned
void ABoardTarget::BeginPlay()
{
    Super::BeginPlay();
    
}

// Called every frame
void ABoardTarget::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ABoardTarget::DrawTarget()
{
    BoardReference->AmountOfRows;

    float CenteringOffsetRow = (BoardReference->AmountOfRows * 100) / 2;
    float CenteringOffsetColumn = (BoardReference->AmountOfColumns * 100) / 2;
    // Clean up any existing components
    
}

void ABoardTarget::ReceiveGameLogicRef(AGameLogic* PGameLogicRef)
{
    //GameLogicRef = PGameLogicRef;

    //BoardReference = GameLogicRef->SendOtherBoardRefMethod();
}

void ABoardTarget::GenerateTarget()
{
    TargetBoard.Empty();
    InstructionsBoard.Empty();

    bool WrongIndex = false;

    int ColNum = BoardReference->AmountOfColumns;
    int RowNum = BoardReference->AmountOfRows;

    //Origin Point
    TargetBoard.Add(FVector2D(0, 0));
    InstructionsBoard.Add(FString("ORIGIN"));

    int AmountLoops = FMath::RandRange(1, 5);

    int LastStep = INT_MAX;
    bool Duplicate = false;
    int RngChoose = 0;

    for (int i = 0; i < AmountLoops; i++)
    {
        // Just in Case there is something different thant 0-3
        do {
            do {
                RngChoose = FMath::RandRange(0, DirectionMap.Num() - 2);
                if (LastStep == RngChoose - 2 || LastStep == RngChoose + 2)
                {
                    Duplicate = true;
                }
                else
                {
                    Duplicate = false;
                    LastStep = RngChoose;
                }

            } while (Duplicate);

            switch (RngChoose)
            {
            case 0:
                WrongIndex = false;
                TargetBoard.Add(*DirectionMap.Find("UP"));
                InstructionsBoard.Add(FString("UP"));
                break;

            case 1:
                WrongIndex = false;
                TargetBoard.Add(*DirectionMap.Find("RIGHT"));
                InstructionsBoard.Add(FString("RIGHT"));
                break;

            case 2:
                WrongIndex = false;
                TargetBoard.Add(*DirectionMap.Find("DOWN"));
                InstructionsBoard.Add(FString("DOWN"));
                break;

            case 3:
                WrongIndex = false;
                TargetBoard.Add(*DirectionMap.Find("LEFT"));
                InstructionsBoard.Add(FString("LEFT"));
                break;

            default:
                WrongIndex = true;
                break;
            }
        } while (WrongIndex);
    }
      
}

void ABoardTarget::DebugGeneratedTargets()
{

    CleanComponents();
    GenerateTarget();
    //@todo: need to be correctly implemented to create a visual representation of the instructions
    //float CenteringOffsetRow = (AmountOfRows * 100) / 2;
    //float CenteringOffsetColumn = (AmountOfColumns * 100) / 2;
    // Clean up any existing components
    //CleanComponents(Board);

    // Resize the board array to contain the specified number of rows


    int i = 0;

    FVector MemoryPos = FVector(0, 0, 0);

    for (auto& Move : InstructionsBoard)
    {
        FVector2D CurrentMove = DirectionMap[Move];
        MemoryPos = MemoryPos + FVector(CurrentMove.X * 100, CurrentMove.Y * 100, 0);
        FVector PositionOffset = FVector(CurrentMove.X + MemoryPos.X,CurrentMove.Y + MemoryPos.Y, 0);
        FName ComponentName = FName(Move + FString::FromInt(i));

        UStaticMeshComponent* NewComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), ComponentName);
        if (NewComponent && GetWorld())
        {
            // Register and add the new component
            NewComponent->RegisterComponent();
            AddInstanceComponent(NewComponent);
            NewComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

            // Set the static mesh and location for the component
            NewComponent->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/CUBES/STONE/Cubeb.Cubeb'")));
            //NewComponent->SetMaterial(0, BaseMaterial);
            NewComponent->SetRelativeLocation(PositionOffset);
            FString XStr = FString::SanitizeFloat(PositionOffset.X);
            FString YStr = FString::SanitizeFloat(PositionOffset.Y);
            FString ZStr = FString::SanitizeFloat(PositionOffset.Z);
            FString NameStr = FString(TEXT("Vector_")) + XStr + TEXT("_") + YStr + TEXT("_") + ZStr;
            NewComponent->ComponentTags.Add(FName(*NameStr));

            // Log the name of the created component
            //UE_LOG(LogTemp, Warning, TEXT("Component Name: %s"), *Board[i].Columns[j]->GetName());
        }
        i++;
    }

    // Trigger updating the render state
    MarkComponentsRenderStateDirty();
}

void ABoardTarget::CleanComponents()
{
    TArray<USceneComponent*> AllComponents;
    this->GetComponents(AllComponents);
    TArray<UStaticMeshComponent*> MeshComponents;
    for (auto SingleComponent : AllComponents)
    {
        if (Cast<UStaticMeshComponent>(SingleComponent) != nullptr)
        {
            MeshComponents.Add(Cast<UStaticMeshComponent>(SingleComponent));
        }
    }

    for (auto Component : MeshComponents)
    {
        Component->DestroyComponent();
        Component->UnregisterComponent();
        RemoveInstanceComponent(Component);
    }
}


TMap<FString, FVector2D> ABoardTarget::ReturnDirectionMap()
{
    return DirectionMap;
}

TArray<FString> ABoardTarget::GetInstructions()
{
    return InstructionsBoard;
}