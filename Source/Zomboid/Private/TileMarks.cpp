// Fill out your copyright notice in the Description page of Project Settings.


#include "TileMarks.h"
#include "UObject/ConstructorHelpers.h"



// Sets default values for this component's properties
UTileMarks::UTileMarks()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

    // Check if the static mesh asset is found successfully
    
    static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshAsset(TEXT("/Game/Helper/Arrow.Arrow"));
    if (StaticMeshAsset.Succeeded())
    {
        // Set the static mesh for this component
        SetStaticMesh(StaticMeshAsset.Object);
        //UE_LOG(LogTemp, Warning, TEXT("FOUND"));
    }
    else
    {
        // Handle the case when the static mesh asset is not found
        UE_LOG(LogTemp, Warning, TEXT("NOT FOUND"));
    }
}


// Called when the game starts
void UTileMarks::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTileMarks::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}