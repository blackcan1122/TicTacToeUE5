// Fill out your copyright notice in the Description page of Project Settings.


#include "TextureConversion.h"

// Sets default values
UTextureConversion::UTextureConversion()
{

}

void UTextureConversion::OpenTreeGraph()
{
    TSharedRef<SWindow> TreeGraphWindow = SNew(SWindow)
        .Title(FText::FromString(TEXT("My Big PP")))
        .ClientSize(FVector2D(800, 600))
        [
            SNew(STreeGraphWidget)
        ];

    FSlateApplication::Get().AddWindow(TreeGraphWindow);
}

void UTextureConversion::OpenTreeGraphPoop()
{
    UE_LOG(LogTemp, Warning, TEXT("HEHEHE"));
}
