// Copyright Epic Games, Inc. All Rights Reserved.

#include "TreeGenPluginModule.h"
#include "TreeGenPluginEditorModeCommands.h"

#define LOCTEXT_NAMESPACE "TreeGenPluginModule"

void FTreeGenPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FTreeGenPluginEditorModeCommands::Register();
}

void FTreeGenPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FTreeGenPluginEditorModeCommands::Unregister();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTreeGenPluginModule, TreeGenPluginEditorMode)