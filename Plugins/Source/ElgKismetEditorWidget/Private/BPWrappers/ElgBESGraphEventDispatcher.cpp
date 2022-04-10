// Copyright 2019-2021 ElgSoft. All rights reserved. 


#include "BPWrappers/ElgBESGraphEventDispatcher.h"
#include <ElgKEWUtils.h>
#include <K2Node_CreateDelegate.h>
#include <Kismet2/BlueprintEditorUtils.h>
#include <ElgEditorContext_BlueprintEditor.h>
#include <ElgKEWEditorSubSystem.h>
#include <K2Node_BaseMCDelegate.h>
#include <Kismet2/KismetEditorUtilities.h>
#include <ElgBESGraphEventDispatcherPin.h>
#include <K2Node_EditablePinBase.h>
#include <K2Node_FunctionEntry.h>
#include "ElgKEW_Log.h"


#define LOCTEXT_NAMESPACE "FElgKismetEditorWidgetModule"


void UElgBESGraphEventDispatcher::OpenEventGraph()
{
	FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(NodePtr, false);
}

void UElgBESGraphEventDispatcher::RemoveEventDispatcher()
{
	ValidateAndUpdate();
	
	const FScopedTransaction Transaction(LOCTEXT("RemoveDelegate", "Remove Event Dispatcher"));
	BlueprintPtr->Modify();

	UElgKEWEditorSubSystem* kewSS = GEditor->GetEditorSubsystem<UElgKEWEditorSubSystem>();
	if (UElgEditorContext_BlueprintEditor* context = kewSS->FindEditorContext(BlueprintPtr)) {
		context->CloseTab(GraphPtr);
	}
	GraphPtr->Modify();

	//FBlueprintEditorUtils::RemoveMemberVariable(BlueprintPtr, GraphPtr->GetFName());
	const int32 VarIndex = FBlueprintEditorUtils::FindNewVariableIndex(BlueprintPtr, VariableName);
	if (VarIndex != INDEX_NONE) {
		BlueprintPtr->NewVariables.RemoveAt(VarIndex);
		FBlueprintEditorUtils::RemoveVariableNodes(BlueprintPtr, VariableName);
	}
	FBlueprintEditorUtils::RemoveGraph(BlueprintPtr, GraphPtr, EGraphRemoveFlags::Recompile);

	for (TObjectIterator<UK2Node_CreateDelegate> It(RF_ClassDefaultObject, /** bIncludeDerivedClasses */ true, /** InternalExcludeFlags */ EInternalObjectFlags::Garbage); It; ++It)
	{
		if (IsValid(*It) && It->GetGraph() && IsValid(It->GetGraph())) {
			It->HandleAnyChange();
		}
	}

	ClearVariableDescription();
	GraphPtr = nullptr;
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(BlueprintPtr);
	
}



TArray<UElgBESGraphEventDispatcherPin*> UElgBESGraphEventDispatcher::GetPinVariables()
{
	TArray<UElgBESGraphEventDispatcherPin*> outPins;
	if (!ValidateAndUpdate()) return outPins;
	for (UEdGraphPin* pin : NodePtr->Pins) {
		if (FElgKEWUtils::HasEditablePin(NodePtr, pin->PinName)) {
			outPins.Add(UElgBESGraphEventDispatcherPin::MakeGraphEventDispatcherPin(BlueprintPtr, GraphPtr, NodePtr, pin));
		}		
	}
	return outPins;
}

TArray<UElgBESGraphEventDispatcherPin*> UElgBESGraphEventDispatcher::GetInputPinVariables()
{
	return GetPinVariables();
}

bool UElgBESGraphEventDispatcher::GetInputPinVariable(const FName InName, UElgBESGraphEventDispatcherPin*& OutPinVariable)
{
	if (!ValidateAndUpdate()) return false;
	if (!FElgKEWUtils::HasEditablePin(NodePtr, InName)) return false;

	for (UEdGraphPin* pin : NodePtr->Pins) {
		if (pin->PinName == InName) {
			OutPinVariable = UElgBESGraphEventDispatcherPin::MakeGraphEventDispatcherPin(BlueprintPtr, GraphPtr, NodePtr, pin);
			return true;
		}
	}
	return false;
}

UElgBESGraphEventDispatcherPin* UElgBESGraphEventDispatcher::AddInputPin(FS_ElgGraphPinType InType, FName InPinName, const FString InDefaultValue)
{
	if (!ValidateAndUpdate()) return nullptr;
	if (InType.EPinCategory == EBPElgKEWPinCategory::PC_Exec) {
		UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("GraphEventDispatcher: You can't add Exec pins to EventDispatchers."));
		return nullptr;
	}

	if (InPinName.IsNone()) {
		InPinName = FName(TEXT("newparam"));
	}
	UElgBESGraphEventDispatcherPin* newPin = nullptr;
	InPinName = FElgKEWUtils::AddEditablePin(BlueprintPtr, NodePtr, InPinName, InType, InDefaultValue, EEdGraphPinDirection::EGPD_Output);
	GetInputPinVariable(InPinName, newPin);
	return newPin;
}


void UElgBESGraphEventDispatcher::CopySignature(const FName InFunctionName)
{
	if (!ValidateAndUpdate()) return;
	if (InFunctionName.IsNone()) return;
	
	if (UFunction* NewSignature = BlueprintPtr->SkeletonGeneratedClass->FindFunctionByName(InFunctionName)) {
		const UEdGraphSchema_K2* Schema = GetDefault<UEdGraphSchema_K2>();

		const FScopedTransaction Transaction(LOCTEXT("CopySignature", "Copy Signature"));

		while (NodePtr->UserDefinedPins.Num())
		{
			TSharedPtr<FUserPinInfo> Pin = NodePtr->UserDefinedPins[0];
			NodePtr->RemoveUserDefinedPin(Pin);
		}

		for (TFieldIterator<FProperty> PropIt(NewSignature); PropIt && (PropIt->PropertyFlags & CPF_Parm); ++PropIt)
		{
			FProperty* FuncParam = *PropIt;
			FEdGraphPinType TypeOut;
			Schema->ConvertPropertyToPinType(FuncParam, TypeOut);
			NodePtr->CreateUserDefinedPin(FuncParam->GetFName(), TypeOut, EGPD_Output);
		}

		FElgKEWUtils::EditablePinChanged(NodePtr);
	}
}


FMulticastDelegateProperty* UElgBESGraphEventDispatcher::GetDelegateProperty()
{
	return FindFProperty<FMulticastDelegateProperty>(BlueprintPtr->SkeletonGeneratedClass, VariableName);
}


void UElgBESGraphEventDispatcher::Setup(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	BlueprintPtr = InBlueprint;
	GraphPtr = InGraph;
	NodePtr = FElgKEWUtils::GetFunctionEntryNodeFromGraph(InGraph);
	ValidateAndUpdate();	
}


FName UElgBESGraphEventDispatcher::DuplicateVar()
{
	// we can't duplicate event dispatchers
	return NAME_None; 
}



bool UElgBESGraphEventDispatcher::IsVarValid()
{
	if (!GraphPtr) {
		return false;
	}

	for (FBPVariableDescription var : BlueprintPtr->NewVariables) {
		if (var.VarName == GraphPtr->GetFName()) {
			UpdateVariableDescription(var);
			return true;
		}
	}
	return false;
}


int32 UElgBESGraphEventDispatcher::GetVarReferenceCount()
{
	int32 count = 0;
	if (!ValidateAndUpdate()) return count;
	TArray<UK2Node_BaseMCDelegate*> NodeUsingDelegate;
	FBlueprintEditorUtils::GetAllNodesOfClass<UK2Node_BaseMCDelegate>(BlueprintPtr, NodeUsingDelegate);
	for (UK2Node_BaseMCDelegate* FunctionNode : NodeUsingDelegate) {
		if (FunctionNode->DelegateReference.IsSelfContext() && (FunctionNode->DelegateReference.GetMemberName() == VariableName)) {
			count++;
		}
	}
	return count;
}


bool UElgBESGraphEventDispatcher::ValidateAndUpdate()
{
	if (!GraphPtr) {
		UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("Invalid GraphPtr for EventDispatcher [%s]"), *VariableName.ToString());
		return false;
	}
	
	for (FBPVariableDescription var : BlueprintPtr->NewVariables) {
		if (var.VarName == GraphPtr->GetFName()) {
			UpdateVariableDescription(var);
			return true;
		}
	}

	UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("Failed to find any EventDispatcher with name [%s]"), *VariableName.ToString());
	return false;
}



void UElgBESGraphEventDispatcher::ShowInDetailsInternal()
{
	UElgKEWEditorSubSystem* kewSS = GEditor->GetEditorSubsystem<UElgKEWEditorSubSystem>();
	if (UElgEditorContext_BlueprintEditor* context = kewSS->FindEditorContext(BlueprintPtr)) {
		context->ShowDelegateInInspector(GraphPtr, NodePtr);
	}
}


TArray<FName> UElgBESGraphEventDispatcher::GetFunctionsSignature(UBlueprint* InBlueprint)
{
	TArray<FName> outSignatures;
	if (InBlueprint) {
		if (UClass* ScopeClass = InBlueprint->SkeletonGeneratedClass) {
			for (TFieldIterator<UFunction> It(ScopeClass, EFieldIteratorFlags::IncludeSuper); It; ++It) {
				UFunction* Func = *It;
				if (UEdGraphSchema_K2::FunctionCanBeUsedInDelegate(Func) && !UEdGraphSchema_K2::HasFunctionAnyOutputParameter(Func)) {
					outSignatures.Add(Func->GetFName());
				}
			}
		}
	}
	return outSignatures;

}


UElgBESGraphEventDispatcher* UElgBESGraphEventDispatcher::MakeGraphEventDispatcherObject(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	UElgBESGraphEventDispatcher* eventDispatcher = NewObject<UElgBESGraphEventDispatcher>();
	eventDispatcher->Setup(InBlueprint, InGraph);
	return eventDispatcher;
}


#undef LOCTEXT_NAMESPACE