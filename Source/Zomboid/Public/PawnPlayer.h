// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "UObject/UObjectIterator.h"
#include "Board.h"
#include "GameLogic.h"
#include "PawnPlayer.generated.h"

class AGameLogic;

UCLASS()
class ZOMBOID_API APawnPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnPlayer();

	UPROPERTY(EditAnywhere)
	float MySpringArmLength = 500;

	UFUNCTION()
	void ReceiveGameLogicRef(AGameLogic* GameLogic);

	UFUNCTION()
	void ReceiveBoardRef(ABoard* Board);

	UFUNCTION(BlueprintCallable)
	void SetIsTurn(bool SetIsTurn);

	UFUNCTION(BlueprintCallable)
	bool GetIsTurn();

	UPROPERTY(EditAnywhere)
	bool IsX = true;

	UFUNCTION()
	FString MakeAIMove();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Member Variables
	UPROPERTY(EditAnywhere)
	USpringArmComponent* MySpringArm;

	UPROPERTY(EditAnywhere)
	UCameraComponent* MyCamera;

	UPROPERTY(BlueprintReadWrite)
	bool ValidRayHit;

	UPROPERTY()
	AGameLogic* GameLogicReference;

	UPROPERTY()
	ABoard* BoardReference;
	//Methods

	UFUNCTION()
	void ChangeSpringArmLength();

	UFUNCTION(BlueprintCallable)
	void MakeMove(FString ComponentHit);


private:
	UPROPERTY()
	bool IsTurn = false;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
