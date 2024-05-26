// Copyright Epic Games, Inc. All Rights Reserved.

#include "gerEDERtStyle.h"
#include "gerEDERt.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FgerEDERtStyle::StyleInstance = nullptr;

void FgerEDERtStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FgerEDERtStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FgerEDERtStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("gerEDERtStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FgerEDERtStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("gerEDERtStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("gerEDERt")->GetBaseDir() / TEXT("Resources"));

	Style->Set("gerEDERt.PluginAction", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
	return Style;
}

void FgerEDERtStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FgerEDERtStyle::Get()
{
	return *StyleInstance;
}
