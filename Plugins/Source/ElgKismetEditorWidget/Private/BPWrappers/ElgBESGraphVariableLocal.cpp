// Copyright 2019-2023 ElgSoft. All rights reserved. 


#include "BPWrappers/ElgBESGraphVariableLocal.h"
#include <ElgKEWUtils.h>
#include <Kismet2/BlueprintEditorUtils.h>
#include <ElgEditorContext_BlueprintEditor.h>
#include <ElgKEWEditorSubSystem.h>
#include <K2Node_Variable.h>
#include "ElgKEW_Log.h"

#define LOCTEXT_NAMESPACE "FElgKismetEditorWidgetModule"


void UElgBESGraphVariableLocal::RemoveVariable(const bool bShowDialog/* = false*/)
{
	if (!ValidateAndUpdate()) return;
	if (bShowDialog && IsInUse()) {
		EAppReturnType::Type DialogReturn = FMessageDialog::Open(EAppMsgType::YesNo, NSLOCTEXT("UnrealEd", "WidgetVariable", "The variable is referenced in the blueprint graph, are you sure you want remove it?"));
		if (DialogReturn == EAppReturnType::No) return;
	}

	if (UFunction* varScope = FElgKEWUtils::GetLocalVariableScope(FunctionPtr, VariableName)) {
		const FScopedTransaction Transaction(LOCTEXT("RemoveLocalVariable", "Remove Local Variable"));
		BlueprintPtr->Modify();
		EntryNodePtr->Modify();
		FBlueprintEditorUtils::RemoveLocalVariable(BlueprintPtr, varScope, VariableName);
		ClearVariableDescription();
	} else {
		UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("Failed to find a Scope for local variable [%s]"), *VariableName.ToString());
	}
}

#pragma region Setters

void UElgBESGraphVariableLocal::SetVariableName(FName InName)
{
	if (!ValidateAndUpdate()) return;
	if (VariableName == InName) return;
	if (FProperty* variableProperty = FElgKEWUtils::GetFunctionProperty(FunctionPtr, VariableName)) {
		UFunction* varScope = variableProperty->GetOwner<UFunction>();
		FBlueprintEditorUtils::RenameLocalVariable(BlueprintPtr, varScope, VariableName, InName);
		// need to use the Guid instead of the name here
		ValidateAndUpdate();
		UElgKEWEditorSubSystem* kewSS = GEditor->GetEditorSubsystem<UElgKEWEditorSubSystem>();
		if (UElgEditorContext_BlueprintEditor* context = kewSS->FindEditorContext(BlueprintPtr)) {
			context->ShowPropertyInInspector(NULL);
			if (context->BlueprintEditorPtr.IsValid()) {
				context->BlueprintEditorPtr.Pin()->RefreshEditors(ERefreshBlueprintEditorReason::BlueprintCompiled);
			}
		}
	}
}

void UElgBESGraphVariableLocal::SetVariableType(FS_ElgGraphPinType InNewType)
{
	if (!ValidateAndUpdate()) return;
	if (PinType.EdGraphPinType == InNewType.EdGraphPinType) return;
	FEdGraphPinType pinType = FElgKEWUtils::ConvertFromPinType(InNewType);
	if (FProperty* variableProperty = FElgKEWUtils::GetFunctionProperty(FunctionPtr, VariableName)) {
		UFunction* varScope = variableProperty->GetOwner<UFunction>();
		FBlueprintEditorUtils::ChangeLocalVariableType(BlueprintPtr, varScope, VariableName, pinType);
		ValidateAndUpdate();
	}
}

void UElgBESGraphVariableLocal::SetVariableDefaultValue(const FString InValue)
{
	if (!ValidateAndUpdate()) return;
	for (FBPVariableDescription& var : EntryNodePtr->LocalVariables) {
		if (var.VarGuid == VariableGuid) {
			const FScopedTransaction Transaction(LOCTEXT("ChangeDefaults", "Change Defaults"));
			EntryNodePtr->Modify();
			BlueprintPtr->Modify();
			var.DefaultValue = InValue;
			EntryNodePtr->RefreshFunctionVariableCache();
			FBlueprintEditorUtils::MarkBlueprintAsModified(BlueprintPtr);
			ValidateAndUpdate();
		}
	}
}

#pragma endregion 


#pragma region overrides

void UElgBESGraphVariableLocal::Setup(UBlueprint* InBlueprint, UEdGraph* InGraph, UK2Node_FunctionEntry* InEntry, FBPVariableDescription InVariableDescription)
{
	BlueprintPtr = InBlueprint;
	GraphPtr = InGraph;
	EntryNodePtr = InEntry;
	FunctionPtr = FElgKEWUtils::FindFunctionInBlueprint(BlueprintPtr, GraphPtr->GetFName());
	UpdateVariableDescription(InVariableDescription);
}


int32 UElgBESGraphVariableLocal::GetVarReferenceCount()
{
	int32 count = 0;
	TArray<UK2Node_Variable*> graphNodes;
	GraphPtr->GetNodesOfClass(graphNodes);
	for (UK2Node_Variable* node : graphNodes) {
		if (node->GetPropertyForVariable()->GetFName() == VariableName) {
			count++;
		}
	}
	return count;
}


EBPElgBESVariableType UElgBESGraphVariableLocal::GetVarType()
{
	return EBPElgBESVariableType::Local;
}

FName UElgBESGraphVariableLocal::DuplicateVar()
{
	return FElgKEWUtils::DuplicateVariable(BlueprintPtr, GetProperty(), false);
}


FProperty* UElgBESGraphVariableLocal::GetProperty()
{
	return FElgKEWUtils::GetFunctionProperty(FunctionPtr, VariableName);
}


bool UElgBESGraphVariableLocal::IsVarValid()
{
	if (!GraphPtr) return false;

	TArray<UK2Node_FunctionEntry*> functionEntryNodes;
	GraphPtr->GetNodesOfClass(functionEntryNodes);
	for (UK2Node_FunctionEntry* node : functionEntryNodes) {
		for (FBPVariableDescription var : node->LocalVariables) {
			if (var.VarGuid == VariableGuid) {
				EntryNodePtr = node;
				FunctionPtr = FElgKEWUtils::FindFunctionInBlueprint(BlueprintPtr, GraphPtr->GetFName());
				UpdateVariableDescription(var);
				return true;
			}
		}
	}
	return false;
}


FString UElgBESGraphVariableLocal::GetDefaultValue()
{
	if (!ValidateAndUpdate()) return FString();
	if (FProperty* variableProperty = FElgKEWUtils::GetFunctionProperty(FunctionPtr, VariableName)) {
		TSharedPtr<FStructOnScope> StructData = MakeShareable(new FStructOnScope((UFunction*)FunctionPtr));
		FBlueprintEditorUtils::PropertyValueToString(variableProperty, StructData->GetStructMemory(), DefaultValue, EntryNodePtr);
	}
	return DefaultValue;
}


UFunction* UElgBESGraphVariableLocal::GetVariableScope()
{
	if (FProperty* variableProperty = FElgKEWUtils::GetFunctionProperty(FunctionPtr, VariableName)) {
		UFunction* varScope = variableProperty->GetOwner<UFunction>();
		return varScope;
	}
	return nullptr;
}


bool UElgBESGraphVariableLocal::ValidateAndUpdate()
{
	if (!GraphPtr) {
		UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("Invalid GraphPtr for local variable [%s]"), *VariableName.ToString());
		return false;
	}
	TArray<UK2Node_FunctionEntry*> functionEntryNodes;
	GraphPtr->GetNodesOfClass(functionEntryNodes);
	for (UK2Node_FunctionEntry* node : functionEntryNodes) {
		for (FBPVariableDescription var : node->LocalVariables) {
			if (var.VarGuid == VariableGuid) {
				EntryNodePtr = node;
				FunctionPtr = FElgKEWUtils::FindFunctionInBlueprint(BlueprintPtr, GraphPtr->GetFName());
				UpdateVariableDescription(var);
				return true;
			}
		}
	}
	UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("Failed to find any local variable with name [%s]"), *VariableName.ToString());
	return false;
}

#pragma endregion



UElgBESGraphVariableLocal* UElgBESGraphVariableLocal::MakeGraphLocalVariableObject(UBlueprint* InBlueprint, UEdGraph* InGraph, UK2Node_FunctionEntry* InEntry, FBPVariableDescription InVariableDescription)
{
	UElgBESGraphVariableLocal* bpVariable = NewObject<UElgBESGraphVariableLocal>();
	bpVariable->Setup(InBlueprint, InGraph, InEntry, InVariableDescription);
	return bpVariable;
}



#undef LOCTEXT_NAMESPACE
