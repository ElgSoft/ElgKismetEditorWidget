// Copyright 2019-2023 ElgSoft. All rights reserved. 


#include "ElgBESGraphMacro.h"
#include <ElgKEWUtils.h>
#include <K2Node_Tunnel.h>
#include <K2Node_MacroInstance.h>
#include <ElgBESGraphMacroPin.h>
#include <ElgKEWEditorSubSystem.h>
#include <ElgEditorContext_BlueprintEditor.h>
#include <Kismet2/KismetEditorUtilities.h>
#include <Kismet2/BlueprintEditorUtils.h>
#include <K2Node_CreateDelegate.h>
#include "ElgKEW_Log.h"


#define LOCTEXT_NAMESPACE "FElgKismetEditorWidgetModule"

#pragma region PinVariables


TArray<UElgBESGraphMacroPin*> UElgBESGraphMacro::GetMacroPinVariables()
{
	TArray<UElgBESGraphMacroPin*> outVars;
	if (!ValidateMacro()) return outVars;

	for (UEdGraphPin* pin : InputNodePtr->Pins) {
		if (FElgKEWUtils::NameToPinCategoryEnum(pin->PinType.PinCategory) == EBPElgKEWPinCategory::PC_Exec) continue;
		outVars.Add(UElgBESGraphMacroPin::MakeGraphMacroPinVariable(GetBlueprint(), GraphPtr, InputNodePtr, pin));
	}
	for (UEdGraphPin* pin : OutputNodePtr->Pins) {
		if (FElgKEWUtils::NameToPinCategoryEnum(pin->PinType.PinCategory) == EBPElgKEWPinCategory::PC_Exec) continue;
		outVars.Add(UElgBESGraphMacroPin::MakeGraphMacroPinVariable(GetBlueprint(), GraphPtr, OutputNodePtr, pin));
	}

	outVars.Sort([&](const UElgBESGraphMacroPin& dataA, const UElgBESGraphMacroPin& dataB) {
		return dataA.PinName.ToString() < dataB.PinName.ToString();
		});

	return outVars;
}

TArray<UElgBESGraphMacroPin*> UElgBESGraphMacro::GetMacroInputPinVariables()
{
	TArray<UElgBESGraphMacroPin*> outVars;
	if (!ValidateMacro()) return outVars;

	for (UEdGraphPin* pin : InputNodePtr->Pins) {
		if (FElgKEWUtils::NameToPinCategoryEnum(pin->PinType.PinCategory) == EBPElgKEWPinCategory::PC_Exec) continue;
		outVars.Add(UElgBESGraphMacroPin::MakeGraphMacroPinVariable(GetBlueprint(), GraphPtr, InputNodePtr, pin));
	}

	return outVars;

}

bool UElgBESGraphMacro::GetMacroInputPinVariable(const FName InName, UElgBESGraphMacroPin*& OutPinVariable)
{
	if (!ValidateMacro()) return false;
	for (UEdGraphPin* pin : InputNodePtr->Pins) {
		if (FElgKEWUtils::NameToPinCategoryEnum(pin->PinType.PinCategory) == EBPElgKEWPinCategory::PC_Exec) continue;
		if (pin->PinName == InName) {
			OutPinVariable = UElgBESGraphMacroPin::MakeGraphMacroPinVariable(GetBlueprint(), GraphPtr, InputNodePtr, pin);
			return true;
		}		
	}
	return false;
}

TArray<UElgBESGraphMacroPin*> UElgBESGraphMacro::GetMacroOutputPinVariables()
{
	TArray<UElgBESGraphMacroPin*> outVars;
	if (!ValidateMacro()) return outVars;

	for (UEdGraphPin* pin : OutputNodePtr->Pins) {
		if (FElgKEWUtils::NameToPinCategoryEnum(pin->PinType.PinCategory) == EBPElgKEWPinCategory::PC_Exec) continue;
		outVars.Add(UElgBESGraphMacroPin::MakeGraphMacroPinVariable(GetBlueprint(), GraphPtr, OutputNodePtr, pin));
	}
	return outVars;
}

bool UElgBESGraphMacro::GetMacroOutputPinVariable(const FName InName, UElgBESGraphMacroPin*& OutPinVariable)
{
	if (!ValidateMacro()) return false;
	for (UEdGraphPin* pin : OutputNodePtr->Pins) {
		if (FElgKEWUtils::NameToPinCategoryEnum(pin->PinType.PinCategory) == EBPElgKEWPinCategory::PC_Exec) continue;
		if (pin->PinName == InName) {
			OutPinVariable = UElgBESGraphMacroPin::MakeGraphMacroPinVariable(GetBlueprint(), GraphPtr, InputNodePtr, pin);
			return true;
		}
	}
	return false;

}

UElgBESGraphMacroPin* UElgBESGraphMacro::AddInputPin(FS_ElgGraphPinType InType, FName InPinName, const FString InDefaultValue)
{
	if (!ValidateMacro()) return nullptr;
	if (InPinName.IsNone()) {
		InPinName = FName(TEXT("newparam"));
	}
	UElgBESGraphMacroPin* newPin = nullptr;
	InPinName = FElgKEWUtils::AddEditablePin(GetBlueprint(), InputNodePtr, InPinName, InType, InDefaultValue, EGPD_Output);
	GetMacroInputPinVariable(InPinName, newPin);
	return newPin;
}

UElgBESGraphMacroPin* UElgBESGraphMacro::AddOutputPin(FS_ElgGraphPinType InType, FName InPinName)
{
	if (!ValidateMacro()) return nullptr;
	if (InPinName.IsNone()) {
		InPinName = FName(TEXT("newparam"));
	}
	UElgBESGraphMacroPin* newPin = nullptr;
	InPinName = FElgKEWUtils::AddEditablePin(GetBlueprint(), OutputNodePtr, InPinName, InType, FString(), EGPD_Input);
	GetMacroOutputPinVariable(InPinName, newPin);
	return newPin;
}

#pragma endregion


#pragma region Misc

void UElgBESGraphMacro::OpenMacroGraph()
{
	if (!ValidateMacro()) return;
	FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(GraphPtr, false);
}

void UElgBESGraphMacro::Remove(const bool bShowDialog /*= false*/)
{
	if (!ValidateMacro()) return;

	if (GraphPtr && GraphPtr->bAllowDeletion)
	{
		if (bShowDialog && IsInUse()) {
			EAppReturnType::Type DialogReturn = FMessageDialog::Open(EAppMsgType::YesNo, NSLOCTEXT("UnrealEd", "Macro", "The Macro is referenced in the blueprint graph, are you sure you want remove it?"));
			if (DialogReturn == EAppReturnType::No) return;
		}

		const FScopedTransaction Transaction(LOCTEXT("RemoveMacro", "Remove Macro"));
		GetBlueprint()->Modify();
		GraphPtr->Modify();

		UElgKEWEditorSubSystem* kewSS = GEditor->GetEditorSubsystem<UElgKEWEditorSubSystem>();
		if (UElgEditorContext_BlueprintEditor* context = kewSS->FindEditorContext(GetBlueprint())) {
			context->CloseTab(GraphPtr);
		}
		FBlueprintEditorUtils::RemoveGraph(GetBlueprint(), GraphPtr, EGraphRemoveFlags::Recompile);

		for (TObjectIterator<UK2Node_CreateDelegate> It(RF_ClassDefaultObject, /** bIncludeDerivedClasses */ true, /** InternalExcludeFlags */ EInternalObjectFlags::Garbage); It; ++It)
		{
			if (IsValid(*It) && It->GetGraph() && IsValid(It->GetGraph()) && It->GetGraph() != GraphPtr) {
				It->HandleAnyChange();
			}
		}

		// invalidate this object as the function is removed.
		GraphPtr = nullptr;
		Name = NAME_None;
		Guid.Invalidate();
	}
}


void UElgBESGraphMacro::SetCategory(FText InCategoryName)
{
	if (InCategoryName.IsEmpty()) {
		InCategoryName = FText::FromString(TEXT("Default"));
	}
	FBlueprintEditorUtils::SetBlueprintFunctionOrMacroCategory(GraphPtr, InCategoryName);

	UElgKEWEditorSubSystem* kewSS = GEditor->GetEditorSubsystem<UElgKEWEditorSubSystem>();
	if (UElgEditorContext_BlueprintEditor* context = kewSS->FindEditorContext(GetBlueprint())) {
		context->bOnGraphChanged = true;
	}
}

void UElgBESGraphMacro::SetMacroName(FName InNewName)
{
	if (InNewName.IsNone()) return;
	UEdGraph* ExistingGraph = FindObject<UEdGraph>(GraphPtr->GetOuter(), *InNewName.ToString());
	if (ExistingGraph == nullptr || ExistingGraph == GraphPtr) {
		const FScopedTransaction Transaction(LOCTEXT("RenameMacro", "Rename Macro"));
		FBlueprintEditorUtils::RenameGraph(GraphPtr, InNewName.ToString());
	}
}

void UElgBESGraphMacro::Duplicate(UElgBESGraphMacro*& OutMacro)
{
	if (!ValidateMacro()) return;
	if (UEdGraph* newGraph = FElgKEWUtils::DuplicateGraph(GetBlueprint(), GraphPtr)) {
		OutMacro = UElgBESGraphMacro::MakeGraphMacroObject(newGraph);
		return;
	}
	OutMacro = nullptr;
}

#pragma endregion


#pragma region Getters

FGuid UElgBESGraphMacro::GetGuid()
{
	if (!ValidateMacro()) return FGuid();
	Guid = GraphPtr->GraphGuid;
	return Guid;
}

FName UElgBESGraphMacro::GetBlueprintName()
{
	if (!ValidateMacro()) return FName();
	return GetBlueprint()->GetFName();
}

FText UElgBESGraphMacro::GetKeywords()
{
	if (!ValidateMacro()) return FText();
	return InputNodePtr->MetaData.Keywords;
}

FText UElgBESGraphMacro::GetCompactNodeTitle()
{
	if (!ValidateMacro()) return FText();
	return InputNodePtr->MetaData.CompactNodeTitle;
}


bool UElgBESGraphMacro::IsMacroValid()
{
	if (!ValidateMacro()) return false;
	return true;
}

void UElgBESGraphMacro::ShowInDetailsInternal()
{
	if (!ValidateMacro()) return;
	UElgKEWEditorSubSystem* kewSS = GEditor->GetEditorSubsystem<UElgKEWEditorSubSystem>();
	if (UElgEditorContext_BlueprintEditor* context = kewSS->FindEditorContext(GetBlueprint())) {
		context->ShowGraphInInspector(GraphPtr);
	}

}

FName UElgBESGraphMacro::GetNameInternal()
{
	if (!ValidateMacro()) return FName();
	Name = GraphPtr->GetFName();
	return Name;
}

FText UElgBESGraphMacro::GetToolTipInternal()
{
	if (!ValidateMacro()) return FText();
	return InputNodePtr->MetaData.ToolTip;
}

FText UElgBESGraphMacro::GetCategoryInternal()
{
	if (!ValidateMacro()) return FText::FromString(TEXT("Default"));
	FText cat = InputNodePtr->MetaData.Category;
	if (cat.IsEmpty()) {
		cat = FText::FromString(TEXT("Default"));
	}
	return cat;
}

int32 UElgBESGraphMacro::GetCountInternal()
{
	int32 usage = 0;
	if (!ValidateMacro()) return usage;

	for (UEdGraph* graph : FElgKEWUtils::GetAllGraphs(GetBlueprint())) {
		TArray<UK2Node_MacroInstance*> callNodes;
		graph->GetNodesOfClass(callNodes);
		for (UK2Node_MacroInstance* node : callNodes) {
			if (node->GetMacroGraph()->GetFName() == Name) {
				usage++;
			}
		}
	}
	return usage;
}

void UElgBESGraphMacro::GetIconInternal(struct FSlateBrush& OutBrush)
{
	if (!ValidateMacro()) return;
	FLinearColor tint = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	FSlateIcon icon = FSlateIcon("EditorStyle", "GraphEditor.Macro_16x");
	OutBrush = *icon.GetOptionalIcon();
	OutBrush.TintColor = tint;
}

#pragma endregion


#pragma region ptrGetters

UBlueprint* UElgBESGraphMacro::GetBlueprint()
{
	return Cast<UBlueprint>(GraphPtr->GetOuter());
}


UEdGraph* UElgBESGraphMacro::GetGraph()
{
	return GraphPtr;
}

#pragma endregion

void UElgBESGraphMacro::Setup(UEdGraph* InGraph)
{
	GraphPtr = InGraph;
	Name = InGraph->GetFName();
	BaseName = Name;
	Guid = InGraph->GraphGuid;
	GetMacroTunnelNodes(InGraph, InputNodePtr, OutputNodePtr);
}

bool UElgBESGraphMacro::ValidateMacro()
{
	if (GraphPtr && Guid.IsValid()) return true;
	UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("Function are no longer valid!"));
	return false;

}

UElgBESGraphMacro* UElgBESGraphMacro::MakeGraphMacroObject(UEdGraph* InGraph)
{
	if (!InGraph) return nullptr;
	UElgBESGraphMacro* bpMacro = NewObject<UElgBESGraphMacro>();
	bpMacro->Setup(InGraph);
	return bpMacro;

}


void UElgBESGraphMacro::GetMacroTunnelNodes(UEdGraph* InGraph, UK2Node_Tunnel*& OutInputNode, UK2Node_Tunnel*& OutOutputNode)
{
	if (!InGraph) return;
	TArray<UK2Node_Tunnel*> tunnelNodes;
	InGraph->GetNodesOfClass(tunnelNodes);
	for (UK2Node_Tunnel* node : tunnelNodes) {
		if (node->bCanHaveInputs) {
			OutOutputNode = node;
		} else if (node->bCanHaveOutputs) {
			OutInputNode = node;
		}
	}
}

#undef LOCTEXT_NAMESPACE