// Copyright 2019-2023 ElgSoft. All rights reserved.

#include "ElgKismetEditorWidget.h"
#include <BlueprintEditorModule.h>
#include <WorkflowOrientedApp/WorkflowTabManager.h>
#include <BlueprintEditor.h>
#include <ElgKEWSettings.h>
#include <ElgKEWTabFactory.h>
#include <ElgKEWViewerTabFactory.h>
#include "ElgKEW_Log.h"

#define LOCTEXT_NAMESPACE "FElgKismetEditorWidgetModule"

void FElgKismetEditorWidgetModule::StartupModule()
{
	FBlueprintEditorModule& bpEditor = FModuleManager::LoadModuleChecked<FBlueprintEditorModule>("Kismet");
	bpEditor.OnRegisterTabsForEditor().AddRaw(this, &FElgKismetEditorWidgetModule::OnRegisterTabSpawner);
}

void FElgKismetEditorWidgetModule::ShutdownModule()
{
	FBlueprintEditorModule& bpEditor = FModuleManager::LoadModuleChecked<FBlueprintEditorModule>("Kismet");
	bpEditor.OnRegisterLayoutExtensions().RemoveAll(this);
	BlueprintEditorPtr = nullptr;
}


void FElgKismetEditorWidgetModule::OnRegisterTabSpawner(class FWorkflowAllowedTabSet& InTabSpawners, FName InModeName, 
	TSharedPtr<class FBlueprintEditor> InBlueprintEditor)
{
	BlueprintEditorPtr = InBlueprintEditor;

	for (auto& widgetInfo : GetDefault<UElgKEWSettings>()->Widgets) {
		if (!widgetInfo.Value.EditorWidget.IsValid()) {
			UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("KismetEditorWidget: No valid editor widget found for [%s]"), *widgetInfo.Key.ToString());
			continue;
		};
		if (widgetInfo.Value.TabLabel.IsEmpty()) {
			UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("KismetEditorWidget: TabLabel for [%s] is empty!"), *widgetInfo.Key.ToString());
			continue;
		}
		InTabSpawners.RegisterFactory(MakeShareable(new FElgKEWTabFactory(InBlueprintEditor, widgetInfo.Key, widgetInfo.Value)));
	}
	InTabSpawners.RegisterFactory(MakeShareable(new FElgKEWViewerTabFactory(InBlueprintEditor)));
}


TWeakPtr<FBlueprintEditor> FElgKismetEditorWidgetModule::GetBlueprintEditor()
{
	if (BlueprintEditorPtr.IsValid()) return BlueprintEditorPtr;
	return nullptr;
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FElgKismetEditorWidgetModule, ElgKismetEditorWidget)