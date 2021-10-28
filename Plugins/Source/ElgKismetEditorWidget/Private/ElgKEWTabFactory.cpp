// Copyright 2019-2021 ElgSoft. All rights reserved. 

#include "ElgKEWTabFactory.h"

#include <WorkflowOrientedApp/WorkflowTabManager.h>
#include <BlueprintEditor.h>
#include <Toolkits/AssetEditorToolkit.h>
#include <EditorStyleSet.h>

#include <ElgKEWSettings.h>
#include <ElgKEWContainerWidget.h>


#define LOCTEXT_NAMESPACE "ElgKEWTabFactory"

// const FName FBlueprintEditorTabs::PaletteID( TEXT( "PaletteList" ) );
FElgKEWTabFactory::FElgKEWTabFactory( TSharedPtr<class FAssetEditorToolkit> InHostingApp, 
	FName InIdentifier, FS_ElgKEWConfig InWidgetInfo) 
	: FWorkflowTabFactory(TEXT("ElgKismetEditorWidget"), InHostingApp)
{
	TabIdentifier = InIdentifier;
	TabLabel = InWidgetInfo.TabLabel;
	TabIcon = FSlateIcon(FEditorStyle::GetStyleSetName(), "Icons.Warning");
	bIsSingleton = true;
	ViewMenuDescription = InWidgetInfo.MenuDescription;
	ViewMenuTooltip = InWidgetInfo.MenuToolTip;
}


TSharedRef<SWidget> FElgKEWTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FBlueprintEditor> BlueprintEditorPtr = StaticCastSharedPtr<FBlueprintEditor>(HostingApp.Pin());

	return SNew(SElgKEWContainerWidget )
		.Kismet2(BlueprintEditorPtr)
		.ViewIdentifier(TabIdentifier);
}

FText FElgKEWTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return ViewMenuTooltip;
}


#undef LOCTEXT_NAMESPACE