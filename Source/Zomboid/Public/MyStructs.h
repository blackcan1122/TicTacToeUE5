// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyStructs.generated.h"



USTRUCT(BlueprintType)
struct FRows
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UStaticMeshComponent*> Columns;


	UStaticMeshComponent* operator[](int Index) const
	{
		return Columns[Index];
	}
};

/**
 * 
 */
class ZOMBOID_API MyStructs
{
public:
	MyStructs();
	~MyStructs();
};
