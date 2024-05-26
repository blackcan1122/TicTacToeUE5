// Copyright Epic Games, Inc. All Rights Reserved.

#include "gerEDERt.h"
#include "gerEDERtStyle.h"
#include "gerEDERtCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

static const FName gerEDERtTabName("gerEDERt");

#define LOCTEXT_NAMESPACE "FgerEDERtModule"

void FgerEDERtModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FgerEDERtStyle::Initialize();
	FgerEDERtStyle::ReloadTextures();

	FgerEDERtCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FgerEDERtCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FgerEDERtModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FgerEDERtModule::RegisterMenus));
}

void FgerEDERtModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FgerEDERtStyle::Shutdown();

	FgerEDERtCommands::Unregister();
}

void FgerEDERtModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FgerEDERtModule::PluginButtonClicked()")),
							FText::FromString(TEXT("gerEDERt.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FgerEDERtModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("Tools");
			Section.AddMenuEntryWithCommandList(FgerEDERtCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PoopTool");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FgerEDERtCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FgerEDERtModule, gerEDERt)