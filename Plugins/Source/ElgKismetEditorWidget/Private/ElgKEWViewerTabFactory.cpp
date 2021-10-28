// Copyright 2019-2021 ElgSoft. All rights reserved. 


#include "ElgKEWViewerTabFactory.h"


#include <WorkflowOrientedApp/WorkflowTabManager.h>
#include <EditorStyleSet.h>
#include <ElgKEWViewerWidget.h>
#include <Toolkits/AssetEditorToolkit.h>
#include <BlueprintEditor.h>


#define LOCTEXT_NAMESPACE "ElgEditorWidgetViewer"

FElgKEWViewerTabFactory::FElgKEWViewerTabFactory(TSharedPtr<class FAssetEditorToolkit> InHostingApp) :
	FWorkflowTabFactory(TEXT("EditorWidgetViewer"), InHostingApp)
{
	TabLabel = LOCTEXT("TabTitle", "EditorWidgetViewer");
	TabIcon = FSlateIcon(FEditorStyle::GetStyleSetName(), "Kismet.Tabs.Palette");
	bIsSingleton = true;
	ViewMenuDescription = LOCTEXT("ViewMenuDescription", "EditorWidgetViewer");
	ViewMenuTooltip = LOCTEXT("ViewMenuToolTip", "Show any Editor Widget.");
}


TSharedRef<SWidget> FElgKEWViewerTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FBlueprintEditor> BlueprintEditorPtr = StaticCastSharedPtr<FBlueprintEditor>(HostingApp.Pin());
	return SNew(SElgKEWViewerWidget).Kismet2(BlueprintEditorPtr);
}

FText FElgKEWViewerTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return LOCTEXT("Tooltip", "Select any Editor Widget to show it inside the Blueprint Editor.");
}

#undef LOCTEXT_NAMESPACE