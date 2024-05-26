// Fill out your copyright notice in the Description page of Project Settings.


#include "SortingClass.h"
#include "EditorActorFolders.h"
#include "Editor/EditorEngine.h"
#include "UnrealEd.h"

#if WITH_EDITOR
FFolder USortingClass::ToFolder(FString Folderpath)
{

	if (GEditor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("GEditor not Found"));
		return FFolder();
	}
	UWorld* World = GEditor->GetEditorWorldContext().World();
	FFolder::FRootObject FolderRoot = FFolder::GetWorldRootFolder(World).GetRootObject();


	return FFolder(FolderRoot, *Folderpath);
}

void USortingClass::CreaterFolderInWorld(FString Folderpath, bool& bOutSuccess, FString& OutInfoMessage)
{
	if (GEditor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("GEditor not Found"));
		return;
	}
	UWorld* World = GEditor->GetEditorWorldContext().World();
	FFolder FolderToCreate = ToFolder(Folderpath);
	bool OutSuccess = FActorFolders::Get().CreateFolder(*World, FolderToCreate);

	World->MarkPackageDirty();

	if (OutSuccess == true)
	{
		bOutSuccess = true;
		OutInfoMessage = FString::Printf(TEXT("Folder was Successfully Created in World - %s"), *Folderpath);
	}
	else
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Folder Creation is Failed in World - %s"), *Folderpath);
	}
}

void USortingClass::MoveActorInWorldFolder(AActor* Actor, FString FolderPath, bool& bOutSuccess, FString& OutInfoMessage)
{
	if (Actor == nullptr)
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Actor is not Valid"));
		return;
	}

	Actor->SetFolderPath_Recursively(*FolderPath);
	Actor->GetWorld()->MarkPackageDirty();
	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Actor %s Was Correctly Moved Inside Folder %s"), *Actor->GetName(), *FolderPath);
	return;
}

TArray<FString> USortingClass::GetAllFolderUnderFolder(FString FolderPath, bool bRecursive, bool& bOutSuccess, FString& OutInfoMessage)
{
	if (GEditor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("GEditor not Found"));
		return TArray<FString>();
	}
	UWorld* World = GEditor->GetEditorWorldContext().World();

	FolderPath = ToFolder(FolderPath).ToString();

	TArray<FString> Folders;

	// Lambda Function to See if Folder is a Child of Provided Folder
	FActorFolders::Get().ForEachFolder(*World, [&Folders, &FolderPath, &bRecursive](const FFolder& Folder)
	{
		if (Folder.ToString() != FolderPath && Folder.ToString().Contains(FolderPath))
		{
			if (bRecursive == true || Folder.GetParent().ToString() == FolderPath)
			{
				Folders.Add(Folder.ToString());
			}
		}
		return true;
	}
	);

	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Child Folder were Found for %s"), *FolderPath);
	return Folders;

}


TArray<AActor*> USortingClass::GetAllActorUnderFolder(FString FolderPath, bool bRecursive, bool& bOutSuccess, FString& OutInfoMessage)
{
	if (GEditor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("GEditor not Found"));
		return TArray<AActor*>();
	}
	UWorld* World = GEditor->GetEditorWorldContext().World();

	TArray<FName> Folders = TArray<FName>({ *FolderPath });

	if (bRecursive)
	{
		Folders.Append(GetAllFolderUnderFolder(FolderPath, true, bOutSuccess, OutInfoMessage));
	}

	TArray<AActor*> Actors;
	FActorFolders::Get().GetActorsFromFolders(*World, Folders, Actors);

	bOutSuccess = true;
	return Actors;
}

void USortingClass::DeleteFolderFromWorld(FString FolderPath, bool& bOutSuccess, FString& OutInfoMessage)
{
	if (GEditor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("GEditor not Found"));
		return;
	}
	UWorld* World = GEditor->GetEditorWorldContext().World();

	FFolder FolderToDetele = ToFolder(FolderPath);

	TArray<AActor*> ActorsUnderFolder = GetAllActorUnderFolder(FolderPath, true, bOutSuccess, OutInfoMessage);

	for (AActor* Actor : ActorsUnderFolder)
	{
		FFolder OldFolder = ToFolder(Actor->GetFolderPath().ToString());
		FFolder NewFolder = ToFolder(OldFolder.ToString().Replace(*FolderToDetele.ToString(), *FolderToDetele.GetParent().ToString()));
		MoveActorInWorldFolder(Actor, NewFolder.ToString(), bOutSuccess, OutInfoMessage);
	}

	TArray<FString> Paths = GetAllFolderUnderFolder(FolderPath, true, bOutSuccess, OutInfoMessage);

	for (FString Folder : Paths)
	{
		FFolder OldFolder = ToFolder(Folder);
		FFolder NewFolder = ToFolder(OldFolder.ToString().Replace(*FolderToDetele.ToString(), *FolderToDetele.GetParent().ToString()));
		FActorFolders::Get().RenameFolderInWorld(*World, OldFolder, NewFolder);
	}

	FActorFolders::Get().DeleteFolder(*World, FolderToDetele);

}
#endif //WITH_EDITOR