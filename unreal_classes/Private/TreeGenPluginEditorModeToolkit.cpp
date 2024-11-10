// Copyright Epic Games, Inc. All Rights Reserved.

#include "TreeGenPluginEditorModeToolkit.h"
#include "TreeGenPluginEditorMode.h"
#include "Engine/Selection.h"

#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "TreeGenPluginEditorModeToolkit"

FTreeGenPluginEditorModeToolkit::FTreeGenPluginEditorModeToolkit()
{
}

void FTreeGenPluginEditorModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);
}

void FTreeGenPluginEditorModeToolkit::GetToolPaletteNames(TArray<FName>& PaletteNames) const
{
	PaletteNames.Add(NAME_Default);
}


FName FTreeGenPluginEditorModeToolkit::GetToolkitFName() const
{
	return FName("TreeGenPluginEditorMode");
}

FText FTreeGenPluginEditorModeToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("DisplayName", "TreeGenPluginEditorMode Toolkit");
}

#undef LOCTEXT_NAMESPACE
