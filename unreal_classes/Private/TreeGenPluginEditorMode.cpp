// Copyright Epic Games, Inc. All Rights Reserved.

#include "TreeGenPluginEditorMode.h"
#include "TreeGenPluginEditorModeToolkit.h"
#include "EdModeInteractiveToolsContext.h"
#include "InteractiveToolManager.h"
#include "TreeGenPluginEditorModeCommands.h"


//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
// AddYourTool Step 1 - include the header file for your Tools here
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
#include "Tools/TreeGenPluginSimpleTool.h"
#include "Tools/TreeGenPluginInteractiveTool.h"

// step 2: register a ToolBuilder in FTreeGenPluginEditorMode::Enter() below


#define LOCTEXT_NAMESPACE "TreeGenPluginEditorMode"

const FEditorModeID UTreeGenPluginEditorMode::EM_TreeGenPluginEditorModeId = TEXT("EM_TreeGenPluginEditorMode");

FString UTreeGenPluginEditorMode::SimpleToolName = TEXT("TreeGenPlugin_ActorInfoTool");
FString UTreeGenPluginEditorMode::InteractiveToolName = TEXT("TreeGenPlugin_MeasureDistanceTool");


UTreeGenPluginEditorMode::UTreeGenPluginEditorMode()
{
	FModuleManager::Get().LoadModule("EditorStyle");

	// appearance and icon in the editing mode ribbon can be customized here
	Info = FEditorModeInfo(UTreeGenPluginEditorMode::EM_TreeGenPluginEditorModeId,
		LOCTEXT("ModeName", "TreeGenPlugin"),
		FSlateIcon(),
		true);
}


UTreeGenPluginEditorMode::~UTreeGenPluginEditorMode()
{
}


void UTreeGenPluginEditorMode::ActorSelectionChangeNotify()
{
}

void UTreeGenPluginEditorMode::Enter()
{
	UEdMode::Enter();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// AddYourTool Step 2 - register the ToolBuilders for your Tools here.
	// The string name you pass to the ToolManager is used to select/activate your ToolBuilder later.
	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////// 
	const FTreeGenPluginEditorModeCommands& SampleToolCommands = FTreeGenPluginEditorModeCommands::Get();

	RegisterTool(SampleToolCommands.SimpleTool, SimpleToolName, NewObject<UTreeGenPluginSimpleToolBuilder>(this));
	RegisterTool(SampleToolCommands.InteractiveTool, InteractiveToolName, NewObject<UTreeGenPluginInteractiveToolBuilder>(this));

	// active tool type is not relevant here, we just set to default
	GetToolManager()->SelectActiveToolType(EToolSide::Left, SimpleToolName);
}

void UTreeGenPluginEditorMode::CreateToolkit()
{
	Toolkit = MakeShareable(new FTreeGenPluginEditorModeToolkit);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> UTreeGenPluginEditorMode::GetModeCommands() const
{
	return FTreeGenPluginEditorModeCommands::Get().GetCommands();
}

#undef LOCTEXT_NAMESPACE
