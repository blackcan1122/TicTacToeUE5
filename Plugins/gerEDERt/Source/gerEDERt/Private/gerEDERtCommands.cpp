// Copyright Epic Games, Inc. All Rights Reserved.

#include "gerEDERtCommands.h"

#define LOCTEXT_NAMESPACE "FgerEDERtModule"

void FgerEDERtCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "gerEDERt", "Start Very Cool Stuff i Swear", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
