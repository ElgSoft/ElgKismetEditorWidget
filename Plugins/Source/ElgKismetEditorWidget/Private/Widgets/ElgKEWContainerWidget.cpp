// Copyright 2019-2023 ElgSoft. All rights reserved. 


#include "ElgKEWContainerWidget.h"

#include <Widgets/SBoxPanel.h>
#include <Engine/Blueprint.h>
#include <EditorUtilityWidget.h>
#include <Editor.h>
#include "Styling/AppStyle.h"

#include <ElgKEWSettings.h>
#include <ElgKEWEditorSubSystem.h>
#include "ElgKEW_Log.h"


#define LOCTEXT_NAMESPACE "ElgKEWContainerWidget"

void SElgKEWContainerWidget ::Construct(const FArguments& InArgs)
{
	BlueprintEditorPtr = InArgs._Kismet2;
	widgetInfoIdentifier = MakeShareable(new FName(InArgs._ViewIdentifier));

	// create the base layout
	ChildSlot
	[
		SNew(SVerticalBox)
		.AddMetaData<FTagMetaData>(FTagMetaData(TEXT("NodeInfo")))
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			SAssignNew(BorderWidget, SBorder)
			.Padding(0)
			.BorderImage(FAppStyle::GetBrush("NoBorder"))
		]
	];

	// create and try to add the EditorWidget
	ConstructEditorWidget();
}


void SElgKEWContainerWidget ::ConstructEditorWidget()
{
	FString errorMessage;
	if (widgetInfoIdentifier.IsValid()) {
		if (widgetInfoIdentifier.Get()) {
			FName widgetKey = *widgetInfoIdentifier;
			if (widgetKey.IsValid()) {
				WidgetInfo = GetDefault<UElgKEWSettings>()->Widgets.Find(*widgetInfoIdentifier.Get());
			}
		}
		if (WidgetInfo) {
			if (UObject* obj = WidgetInfo->EditorWidget.TryLoad()) {
				if (UBlueprint* blueprint = Cast<UBlueprint>(obj)) {
					if (TSubclassOf<UEditorUtilityWidget> widgetClass = (TSubclassOf<UEditorUtilityWidget>)blueprint->GeneratedClass) {
						if (UWorld* world = GEditor->GetEditorWorldContext().World()) {
							editorWidget = CreateWidget<UEditorUtilityWidget>(world, widgetClass);
							UElgKEWEditorSubSystem* kewSS = GEditor->GetEditorSubsystem<UElgKEWEditorSubSystem>();							
							kewSS->WidgetBPEditorMap.Add(editorWidget, BlueprintEditorPtr);
							blueprint->OnCompiled().AddRaw(this, &SElgKEWContainerWidget::HandleOnCompiled);
							if (!IsValid(editorWidget)) {
								errorMessage = "Failed to create the EditorWidget.";
							}
						} else {
							errorMessage = "Failed to get a valid UWorld for the Editor.";
						}
					} else {
						errorMessage = "Failed to cast to UEditorUtilityWidget.";
					}
				} else {
					errorMessage = "Failed to cast the loaded editor widget to Blueprint.";
				}
			} else {
				errorMessage = "Failed to load the EditorWidget.";
			}
		} else {
			errorMessage = "Failed to find a valid widget info struct.";
		}
	} else {
		errorMessage = "Failed to get a valid widget identifier.";
	}


	TSharedRef<SBorder> content =
		SNew(SBorder)
		.Padding(5)
		.BorderImage(FAppStyle::GetBrush("NoBorder"))
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center);

	if (IsValid(editorWidget)) {
		content->SetHAlign(WidgetInfo->HorizontalAlignment);
		content->SetVAlign(WidgetInfo->VerticalAlignment);
		content->SetContent(editorWidget->TakeWidget());
	} else {
		content->SetContent(SNew(STextBlock).Text(FText::FromString(errorMessage)));
		UE_LOG(LogElgKismetEditorWidget, Error, TEXT("KismetEditorWidget: %s"), *errorMessage);
	}

	BorderWidget->SetContent(content);
}


void SElgKEWContainerWidget ::HandleOnCompiled(UBlueprint* InBlueprint)
{
	//UE_LOG(LogElgKismetEditorWidget, Log, TEXT("The EditorWidget has been recompiled!!")); 
	UElgKEWEditorSubSystem* kewSS = GEditor->GetEditorSubsystem<UElgKEWEditorSubSystem>();
	if (editorWidget) kewSS->WidgetBPEditorMap.Remove(editorWidget);
	InBlueprint->OnCompiled().RemoveAll(this);
	ConstructEditorWidget();
}

SElgKEWContainerWidget ::~SElgKEWContainerWidget ()
{
	if (WidgetInfo) {
		if (UObject* obj = WidgetInfo->EditorWidget.TryLoad()) {
			if (UBlueprint* blueprint = Cast<UBlueprint>(obj)) {
				blueprint->OnCompiled().RemoveAll(this);
			}
		}
	}
	UElgKEWEditorSubSystem* kewSS = GEditor->GetEditorSubsystem<UElgKEWEditorSubSystem>();
	if (editorWidget) {		
		kewSS->WidgetBPEditorMap.Remove(editorWidget);		
	}

}

#undef LOCTEXT_NAMESPACE