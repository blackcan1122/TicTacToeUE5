// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TreeGraphWindow.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TextureConversion.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GEREDERT_API UTextureConversion : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UTextureConversion();
public:

	UFUNCTION(BlueprintCallable, Category = "TreeGraph")
	static void OpenTreeGraph();

	UFUNCTION(BlueprintCallable)
	static void OpenTreeGraphPoop();

private:

	//Methods
	UMaterialInstance* WorkingMaterial;
	
};
