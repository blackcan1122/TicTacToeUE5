// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "gerEDERtStyle.h"

class FgerEDERtCommands : public TCommands<FgerEDERtCommands>
{
public:

	FgerEDERtCommands()
		: TCommands<FgerEDERtCommands>(TEXT("gerEDERt"), NSLOCTEXT("Contexts", "gerEDERt", "gerEDERt Plugin"), NAME_None, FgerEDERtStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
