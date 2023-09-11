// Copyright 2019-2023 ElgSoft. All rights reserved. 


#include "ElgKEWBlueprintEditorBP.h"
#include <ElgEditorContext_BlueprintEditor.h>
#include <ElgKEWEditorSubSystem.h>
#include <EdGraph/EdGraphSchema.h>
#include <EdGraph/EdGraph.h>
#include <Subsystems/AssetEditorSubsystem.h>
#include <BlueprintEditor.h>
#include <ElgKEWUtils.h>
#include <ElgKEWStructs.h>
#include "ElgKEW_Log.h"


UElgEditorContext_BlueprintEditor* UElgKEWBlueprintEditorBP::GetKismetContext(UObject* InSelf)
{	
	UElgKEWEditorSubSystem* kewSS = GEditor->GetEditorSubsystem<UElgKEWEditorSubSystem>();
	UElgEditorContext_BlueprintEditor* context = kewSS->GetContext(InSelf);
	if (!context) {
		// if we failed to find a context for InSelf, it can be that it's a child widget created in a KismetEditorWidget.
		// so go through all the Outers for InSelf and see if we can find a context for that instead.
		if (UObject* outerEditorWidget = FElgKEWUtils::GetEditorWidgetParent(InSelf)) {
			return GetKismetContext(outerEditorWidget);
		}

		// check if we have another blueprint instance, this can happen with change events...
		// use IsContextValid when reacting on On* events.
		if (UObject* genBy = InSelf->GetClass()->ClassGeneratedBy) {

			for (auto It = kewSS->WidgetBPEditorMap.CreateConstIterator(); It; ++It) {
				UObject* const widgetInstance = It.Key();
				if (UClass* widgetClass = widgetInstance->GetClass()) {
					if (UObject* widgetGenBy = widgetClass->ClassGeneratedBy) {
						if (widgetGenBy == genBy) {
							UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("KismetEditorWidget: This blueprint instance is not longer valid"));
							return nullptr;
						}
					}
				}
			}
		}

		UE_LOG(LogElgKismetEditorWidget, Error, TEXT("KismetEditorWidget: Failed to find any valid BlueprintEditor Context for this graph [%s], are you running it outside a BlueprintEditor?"), *InSelf->GetName());
		return nullptr;
	}
	return context;
}


EBPElgKEWGraphType UElgKEWBlueprintEditorBP::GetGraphType(UEdGraph* InGraph)
{
	if (InGraph) {
		if (const UEdGraphSchema* schema = InGraph->GetSchema()) {
			switch (schema->GetGraphType(InGraph)) {
				case 		GT_Ubergraph:		return EBPElgKEWGraphType::GT_Ubergraph;
				case 		GT_Animation:		return EBPElgKEWGraphType::GT_Animation;
				case 		GT_Function:		return EBPElgKEWGraphType::GT_Function;
				case 		GT_Macro:			return EBPElgKEWGraphType::GT_Macro;
				case 		GT_StateMachine:	return EBPElgKEWGraphType::GT_StateMachine;
				default:						return EBPElgKEWGraphType::GT_Function;
			}
		}
	}
	return EBPElgKEWGraphType::GT_Ubergraph;
}


void UElgKEWBlueprintEditorBP::IsContextValid(UObject* InSelf, EBPElgKEWWResult& Branch)
{
	Branch = EBPElgKEWWResult::outFalse;
	UElgKEWEditorSubSystem* kewSS = GEditor->GetEditorSubsystem<UElgKEWEditorSubSystem>();
	UElgEditorContext_BlueprintEditor* context = kewSS->GetContext(InSelf);
	if (IsValid(context)) {
		if (IsValid(context->BlueprintPtr) && context->BlueprintEditorPtr.IsValid()) {
			Branch = EBPElgKEWWResult::outTrue;
			return; 
		}
	}
}


void UElgKEWBlueprintEditorBP::MakePinType(EBPElgKEWPinCategory InType, EBPElgPinContainerType InContainterType, FS_ElgGraphPinType& OutPinType)
{	
	FEdGraphPinType pinType = FElgKEWUtils::PinCategoryToPinType(InType);
	pinType.ContainerType = FElgKEWUtils::ConvertFromPinContainerType(InContainterType);
	OutPinType = FElgKEWUtils::ConvertToPinType(pinType);
}



void UElgKEWBlueprintEditorBP::TestFindBlueprintEditor(UObject* InSelf)
{
	if (GEditor) {
		if (IAssetEditorInstance* AssetEditor = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->FindEditorForAsset(InSelf->GetClass()->ClassGeneratedBy, false)) {
			FBlueprintEditor* BlueprintEditor = static_cast<FBlueprintEditor*>(AssetEditor);
			UE_LOG(LogElgKismetEditorWidget, Log, TEXT("Found it!"));
			GetKismetContext(InSelf);
			return;
		}
	}
	UE_LOG(LogElgKismetEditorWidget, Log, TEXT("No BlueprintEditor!"));
}

