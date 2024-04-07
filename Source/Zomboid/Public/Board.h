// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Actor.h"
#include "MyStructs.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Board.generated.h"


UCLASS(BlueprintType, Blueprintable)
class ZOMBOID_API ABoard : public AActor
{
	GENERATED_BODY()

public:	

	// Sets default values for this actor's properties
	ABoard();

	UPROPERTY(EditAnywhere)
	int AmountOfColumns = 3;

	UPROPERTY(EditAnywhere)
	int AmountOfRows = 3;

	UPROPERTY(EditAnywhere)
	UMaterialInstance* XMaterial;

	UPROPERTY(EditAnywhere)
	UMaterialInstance* OMaterial;

	UPROPERTY(EditAnywhere)
	UMaterialInstance* BaseMaterial;

	UPROPERTY(EditAnywhere)
	UStaticMesh* BaseObject;

	UPROPERTY(EditAnywhere)
	UStaticMesh* XOBject;

	UPROPERTY(EditAnywhere)
	UStaticMesh* OOBject;

	UPROPERTY(EditAnywhere)
	int Columns;

	UPROPERTY(EditAnywhere)
	int Rows;

	UPROPERTY()
	TArray<FString> SavedTakenList;

	UFUNCTION()
	float TurnCubeOverTime(float Value1);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void CleanComponents(TArray<FRows>& Boardinput);

	UPROPERTY()
	TArray<FRows> SavedBoard;

	UPROPERTY()
	FTimeline MyTimeline;

	UPROPERTY(EditAnywhere)
	UCurveFloat* MyFloatCurve;

private:

	// Member Variablen

	UPROPERTY()
	USceneComponent* MyRoot;

	UPROPERTY()
	bool lastMoveByPlayer;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(CallInEditor, BlueprintCallable)
	void InitPlayboard();

	UFUNCTION(BlueprintCallable)
	TArray<int> ParseInputMove(FString Field);

	UFUNCTION(BlueprintCallable)
	TArray<FRows> getBoard();

	UFUNCTION(BlueprintCallable)
	bool CalcMove(FString Field, bool IsX, bool IsSimulate);

	UFUNCTION(BlueprintCallable)
	bool validateMove(FString Field);

	UFUNCTION()
	TArray<FString> GetTakenList();

	UFUNCTION()
	int CheckWin();

	UFUNCTION(CallInEditor)
	void TestRow();

	UPROPERTY(VisibleAnywhere);
	TArray<FString> takenListFull;

	UFUNCTION(CallInEditor)
	void SaveBoard();

	UFUNCTION(CallInEditor)
	void RestoreBoard();

	UFUNCTION()
	bool ValidInputAI(FString& input);

	UFUNCTION()
	void ResetLastMadeMove(FString& currentMove);

	UPROPERTY()
	TArray<FRows> Board;

};