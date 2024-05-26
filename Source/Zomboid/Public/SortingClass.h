// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SortingClass.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBOID_API USortingClass : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:

	static FFolder ToFolder(FString Folderpath);


public:
	/*
	* Creater Folder in World Outliner
	* 
	* @param FolderPath			Path to Folder "MyFolder/MyFolder2"
	* @param bOutSuccess		If The Action was a Sucess
	* @param OutInfoMessage		Additional Information on the Action Result
	* 
	*/
	UFUNCTION(BlueprintCallable, Category = "Outliner")
	static void CreaterFolderInWorld(FString Folderpath, bool& bOutSuccess, FString& OutInfoMessage);


	UFUNCTION(BlueprintCallable, Category = "Outliner")
	static void MoveActorInWorldFolder(AActor* Actor, FString FolderPath, bool& bOutSuccess, FString& OutInfoMessage);



	/*
	* 
	* @param bRecursive			Gets Actor Inside folders, which are inside Folders
	* @param FolderPath			Path to Folder "MyFolder/MyFolder2"
	* @param bOutSuccess		If The Action was a Sucess
	* @param OutInfoMessage		Additional Information on the Action Result
	* 
	*/

	UFUNCTION(BlueprintCallable, Category = "Outliner")
	static TArray<FString> GetAllFolderUnderFolder(FString FolderPath, bool bRecursive, bool& bOutSuccess, FString& OutInfoMessage);

	UFUNCTION(BlueprintCallable, Category = "Outliner")
	static TArray<AActor*> GetAllActorUnderFolder(FString FolderPath, bool bRecursive, bool& bOutSuccess, FString& OutInfoMessage);



	UFUNCTION(BlueprintCallable, Category = "Outliner")
	static void DeleteFolderFromWorld(FString FolderPath, bool& bOutSuccess, FString& OutInfoMessage);

};
