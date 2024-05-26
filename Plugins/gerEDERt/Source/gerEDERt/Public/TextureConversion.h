// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TextureConversion.generated.h"

/**
 * 
 */
UCLASS()
class GEREDERT_API UTextureConversion : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UTextureConversion();

private:

	//Methods
	UMaterialInstance* WorkingMaterial;
	
};
