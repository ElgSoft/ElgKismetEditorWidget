// Copyright 2019-2023 ElgSoft. All rights reserved. 

#include "ElgBESGraphInterfaceFunction.h"
#include <ElgKEWUtils.h>
#include <ElgKEWEditorSubSystem.h>
#include <ElgEditorContext_BlueprintEditor.h>
#include <Kismet2/KismetEditorUtilities.h>
#include <Kismet2/BlueprintEditorUtils.h>
#include <K2Node_CallFunction.h>
#include "ObjectEditorUtils.h"
#include <K2Node_Event.h>
#include "ElgKEW_Log.h"
#include "K2Node_FunctionEntry.h"


#define LOCTEXT_NAMESPACE "FElgKismetEditorWidgetModule"


void UElgBESGraphInterfaceFunction::OpenOrSelect()
{
	if (!ValidateFunction()) return;
	if (bFunction && !bInherit) { 
		FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(EntryNodePtr, false);
	} else {
		UFunction* OverrideFunc = nullptr;
		UClass* const OverrideFuncClass = FBlueprintEditorUtils::GetOverrideFunctionClass(BlueprintPtr, FunctionName, &OverrideFunc);
		if (UK2Node_Event* ExistingNode = FBlueprintEditorUtils::FindOverrideForFunction(BlueprintPtr, OverrideFuncClass, FunctionName)) {
			FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(ExistingNode, false);
		}
	}
}


void UElgBESGraphInterfaceFunction::OpenFunctionGraph()
{
	if (!ValidateFunction()) return;
	if (!GraphPtr) return;
	FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(GraphPtr, false);
}


bool UElgBESGraphInterfaceFunction::ImpletentEvent()
{
	if (!ValidateFunction()) return false;
	if (bFunction) return false;

	UFunction* OverrideFunc = nullptr;
	UClass* const OverrideFuncClass = FBlueprintEditorUtils::GetOverrideFunctionClass(BlueprintPtr, FunctionName, &OverrideFunc);
	if (UK2Node_Event* ExistingNode = FBlueprintEditorUtils::FindOverrideForFunction(BlueprintPtr, OverrideFuncClass, FunctionName)) {
		FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(ExistingNode, false);
		return false;
	}
	return FElgKEWUtils::SpawnEventNodeInGraph(BlueprintPtr, FunctionName);
}


bool UElgBESGraphInterfaceFunction::ImpletentInherited()
{
	if (!bInherit) {
		UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("Interface function [%s] is not inherited and can't be impletemted with this function!"), *FunctionName.ToString());
		return false;
	}

	if (FElgKEWUtils::AddOverridableFunction(BlueprintPtr, FunctionName)) {
		for (UEdGraph* graph : BlueprintPtr->FunctionGraphs) {
			if (graph->GetFName() == FunctionName) {
				GraphPtr = graph;
				EntryNodePtr = FElgKEWUtils::GetFunctionEntryNodeFromGraph(GraphPtr);
				ResultNodePtr = FElgKEWUtils::GetFunctionResultNodeFromGraph(GraphPtr);
				FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(GraphPtr, false);
				return true;
			}
		}
	}
	return false;
}


bool UElgBESGraphInterfaceFunction::HasFunctionFlag(EElgFunctionFlags InFlag)
{
	if (!ValidateFunction()) return false;
	return FElgKEWUtils::DoFunctionHasFlag(GetFunction(), InFlag);

}

bool UElgBESGraphInterfaceFunction::HasFunctionFlags(TArray<EElgFunctionFlags> InFlags)
{
	if (!ValidateFunction()) return false;
	UFunction* func = GetFunction();
	for (EElgFunctionFlags flag : InFlags) {
		if (!FElgKEWUtils::DoFunctionHasFlag(func, flag)) return false;
	}
	return true;
}

#pragma region PinVariables

TArray<UElgBESGraphFunctionPin*> UElgBESGraphInterfaceFunction::GetFunctionPinVariables()
{
	TArray<UElgBESGraphFunctionPin*> outVars;
	if (!ValidateFunction()) return outVars;
	if (!GraphPtr) return outVars;
	outVars.Append(FElgKEWUtils::GetInputPinVariables(GetBlueprint(), GraphPtr));
	outVars.Append(FElgKEWUtils::GetOutputPinVariables(GetBlueprint(), GraphPtr));

	outVars.Sort([&](const UElgBESGraphFunctionPin& dataA, const UElgBESGraphFunctionPin& dataB) {
		return dataA.PinName.ToString() < dataB.PinName.ToString();
		});

	return outVars;
}

TArray<UElgBESGraphFunctionPin*> UElgBESGraphInterfaceFunction::GetFunctionInputPinVariables()
{
	TArray<UElgBESGraphFunctionPin*> outVars;
	if (!ValidateFunction()) return outVars;
	if (!GraphPtr) return outVars;
	outVars = FElgKEWUtils::GetInputPinVariables(GetBlueprint(), GraphPtr);
	outVars.Sort([&](const UElgBESGraphFunctionPin& dataA, const UElgBESGraphFunctionPin& dataB) {
		return dataA.PinName.ToString() < dataB.PinName.ToString();
		});
	return outVars;
}

bool UElgBESGraphInterfaceFunction::GetFunctionInputPinVariable(const FName InName, UElgBESGraphFunctionPin*& OutPinVariable)
{
	if (!ValidateFunction()) return false;
	if (!GraphPtr) return false;
	OutPinVariable = FElgKEWUtils::GetInputPinVariable(GetBlueprint(), GraphPtr, InName);
	if (OutPinVariable->PinName.IsNone()) return false;
	return true;
}

TArray<UElgBESGraphFunctionPin*> UElgBESGraphInterfaceFunction::GetFunctionOutputPinVariables()
{
	TArray<UElgBESGraphFunctionPin*> outVars;
	if (!ValidateFunction()) return outVars;
	if (!GraphPtr) return outVars;
	outVars = FElgKEWUtils::GetOutputPinVariables(GetBlueprint(), GraphPtr);
	outVars.Sort([&](const UElgBESGraphFunctionPin& dataA, const UElgBESGraphFunctionPin& dataB) {
		return dataA.PinName.ToString() < dataB.PinName.ToString();
		});
	return outVars;
}

bool UElgBESGraphInterfaceFunction::GetFunctionOutputPinVariable(const FName InName, UElgBESGraphFunctionPin*& OutPinVariable)
{
	if (!ValidateFunction()) return false;
	if (!GraphPtr) return false;
	OutPinVariable = FElgKEWUtils::GetInputPinVariable(GetBlueprint(), GraphPtr, InName);
	if (OutPinVariable->PinName.IsNone()) return false;
	return true;
}

#pragma endregion


#pragma region FunctionGetters

bool UElgBESGraphInterfaceFunction::GetIsFunction()
{
	if (!ValidateFunction()) return true;
	return bFunction;
}

FName UElgBESGraphInterfaceFunction::GetBlueprintName()
{
	if (!ValidateFunction()) return FName();
	return GetBlueprint()->GetFName();
}

FText UElgBESGraphInterfaceFunction::GetKeywords()
{
	if (!ValidateFunction()) return FText();
	if (InterfaceMetaData) {
		return InterfaceMetaData->Keywords;
	}
	return FText();
}

FText UElgBESGraphInterfaceFunction::GetCompactNodeTitle()
{
	if (!ValidateFunction()) return FText();
	if (InterfaceMetaData) {
		return InterfaceMetaData->CompactNodeTitle;
	}
	return FText();
}

FString UElgBESGraphInterfaceFunction::GetDeprecationMessage()
{
	if (!ValidateFunction()) return FString();
	if (InterfaceMetaData) {
		return InterfaceMetaData->DeprecationMessage;
	}
	return FString();
}

bool UElgBESGraphInterfaceFunction::GetIsDeprecated()
{
	if (!ValidateFunction()) return false;
	if (InterfaceMetaData) {
		return InterfaceMetaData->bIsDeprecated;
	}
	return false;
}

bool UElgBESGraphInterfaceFunction::GetCallInEditor()
{
	if (!ValidateFunction()) return false;
	if (InterfaceMetaData) {
		return InterfaceMetaData->bCallInEditor;
	}
	return false;
}

bool UElgBESGraphInterfaceFunction::GetSupportDragDrop()
{
	if (!ValidateFunction()) return false;
	bool isCodeEvent = (FunctionPtr->HasAnyFunctionFlags(FUNC_Event));
	return (FunctionPtr->HasAnyFunctionFlags((FUNC_BlueprintCallable | FUNC_BlueprintPure)) && !isCodeEvent);
}

int32 UElgBESGraphInterfaceFunction::GetNumParamsp()
{
	if (!ValidateFunction()) return 0;
	return FunctionPtr->NumParms;
}

bool UElgBESGraphInterfaceFunction::GetHasReturnValue()
{
	if (!ValidateFunction()) return false;	
	return (HasFunctionFlag(EElgFunctionFlags::FUNC_HasOutParms)) ? true : false;
}

bool UElgBESGraphInterfaceFunction::GetIsValid()
{
	if (FunctionName.IsValid() && InterfaceName.IsValid() && BlueprintPtr) return true;
	return false;
}

void UElgBESGraphInterfaceFunction::IsValidBranch(EBPElgKEWWResult& Branch)
{
	Branch = GetIsValid() ? EBPElgKEWWResult::outTrue : EBPElgKEWWResult::outFalse;
}




void UElgBESGraphInterfaceFunction::SelectInMyBlueprintInternal()
{
	if (!ValidateFunction()) return;
	UElgKEWEditorSubSystem* kewSS = GEditor->GetEditorSubsystem<UElgKEWEditorSubSystem>();
	if (UElgEditorContext_BlueprintEditor* context = kewSS->FindEditorContext(GetBlueprint())) {
		context->ClearSelectionAll();
		context->SelectItemInMyBlueprint(FunctionName);
	}
}

FName UElgBESGraphInterfaceFunction::GetNameInternal()
{
	if (!ValidateFunction()) return FName();
	return FunctionName;
}

FText UElgBESGraphInterfaceFunction::GetToolTipInternal()
{
	FText tip = FText();
	if (!ValidateFunction()) return tip;
	if (InterfaceMetaData) {
		tip = InterfaceMetaData->ToolTip;
	}
	if (tip.IsEmptyOrWhitespace()) {
		tip = FunctionPtr->GetToolTipText();
	}
	return tip;
}

FText UElgBESGraphInterfaceFunction::GetCategoryInternal()
{
	FText outText = FText();
	if (!ValidateFunction()) return outText;
	if (InterfaceMetaData) {
		outText = InterfaceMetaData->Category;
	}
	if (outText.IsEmptyOrWhitespace()) {
		outText = FObjectEditorUtils::GetCategoryText(FunctionPtr);
	}
	return outText;
}

int32 UElgBESGraphInterfaceFunction::GetCountInternal()
{
	int32 usage = 0;
	if (!ValidateFunction()) return usage;

	if (bFunction) {
		for (UEdGraph* graph : FElgKEWUtils::GetAllGraphs(BlueprintPtr)) {
			TArray<UK2Node_CallFunction*> callNodes;
			graph->GetNodesOfClass(callNodes);
			if (callNodes.Num()) {
				for (UK2Node_CallFunction* node : callNodes) {
					if (node->GetFunctionName() == FunctionName) {
						usage++;
					}
				}
			}
		}
	} else {
		for (UEdGraph* graph : FElgKEWUtils::GetAllGraphs(BlueprintPtr)) {
			TArray<UK2Node_Event*> delegateNodes;
			graph->GetNodesOfClass(delegateNodes);
			if (delegateNodes.Num()) {
				for (UK2Node_Event* nodeDelegate : delegateNodes) {
					if (nodeDelegate->GetFunctionName() == FunctionName) {
						usage++;
					}
				}
			}
		}
	}
	return usage;
}

void UElgBESGraphInterfaceFunction::GetIconInternal(struct FSlateBrush& OutBrush)
{
	if (!ValidateFunction()) return;

	FLinearColor tint = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	if (!bFunction) {
		tint = FLinearColor::Yellow;
	}
	OutBrush = *FAppStyle::GetBrush(TEXT("GraphEditor.InterfaceFunction_16x"));
	OutBrush.TintColor = tint;

}


#pragma region ptrGetters

UFunction* UElgBESGraphInterfaceFunction::GetFunction()
{
	return FunctionPtr;
}


UBlueprint* UElgBESGraphInterfaceFunction::GetBlueprint()
{
	return BlueprintPtr;
}


UEdGraph* UElgBESGraphInterfaceFunction::GetGraph()
{
	return GraphPtr;
}


UK2Node_FunctionEntry* UElgBESGraphInterfaceFunction::GetFunctionEntryNode()
{
	return EntryNodePtr;
}


UK2Node_FunctionResult* UElgBESGraphInterfaceFunction::GetFunctionResultNode()
{
	return ResultNodePtr;
}

#pragma endregion


void UElgBESGraphInterfaceFunction::Setup(UBlueprint* InBlueprint, UFunction* InFunction, FBPInterfaceDescription InInterface)
{
	BlueprintPtr = InBlueprint;
	FunctionPtr = InFunction;
	FunctionName = InFunction->GetFName();
	InterfaceDescription = InInterface;
	InterfaceClassPtr = InterfaceDescription.Interface.Get();
	InterfaceName = FElgKEWUtils::GetInterfaceClassName(InterfaceClassPtr);
	bFunction = false; 
	bInherit = false;

	for (UEdGraph* const graph : InterfaceDescription.Graphs) {
		if (graph && graph->GetFName() == FunctionName) {
			bFunction = true;
			GraphPtr = graph;
			EntryNodePtr = FElgKEWUtils::GetFunctionEntryNodeFromGraph(GraphPtr);
			ResultNodePtr = FElgKEWUtils::GetFunctionResultNodeFromGraph(GraphPtr);
		}
	}

	// To access the meta data set in a BP interface we need to find the function graph in the interface object.
	if (InterfaceClassPtr->ClassGeneratedBy) {
		if (UBlueprint* SourceBlueprint = Cast<UBlueprint>(InterfaceClassPtr->ClassGeneratedBy)) {
			for (UEdGraph* graph : SourceBlueprint->FunctionGraphs) {
				if (graph->GetFName() == FunctionName) {
					InterfaceGraphPtr = graph;
					InterfaceMetaData = FBlueprintEditorUtils::GetGraphFunctionMetaData(graph);
				}
			}
		}
	}
}


void UElgBESGraphInterfaceFunction::Setup(UBlueprint* InBlueprint, UFunction* InFunction, UClass* InInterfaceClass)
{
	BlueprintPtr = InBlueprint;
	FunctionPtr = InFunction;
	FunctionName = InFunction->GetFName();
	InterfaceClassPtr = InInterfaceClass;
	InterfaceName = FElgKEWUtils::GetInterfaceClassName(InterfaceClassPtr);
	bFunction = false;
	bInherit = true;

	// To access the meta data set in a BP interface we need to find the function graph in the interface object.
	if (InterfaceClassPtr->ClassGeneratedBy) {
		if (UBlueprint* SourceBlueprint = Cast<UBlueprint>(InterfaceClassPtr->ClassGeneratedBy)) {
			for (UEdGraph* graph : SourceBlueprint->FunctionGraphs) {
				if (graph->GetFName() == FunctionName) {
					bFunction = FunctionPtr->HasAllFunctionFlags(FUNC_HasOutParms);
					InterfaceGraphPtr = graph;
					InterfaceMetaData = FBlueprintEditorUtils::GetGraphFunctionMetaData(graph);
				}
			}
		}
	}
}


bool UElgBESGraphInterfaceFunction::ValidateFunction()
{
	if (!FunctionName.IsValid() || !InterfaceName.IsValid()) return false;
	if (BlueprintPtr) {
		if (!bInherit) {
			for (int32 i = 0; i < BlueprintPtr->ImplementedInterfaces.Num(); i++) {
				FBPInterfaceDescription& InterfaceDesc = BlueprintPtr->ImplementedInterfaces[i];
				if (FElgKEWUtils::GetInterfaceClassName(InterfaceDesc.Interface.Get()) == InterfaceName) {
					for (TFieldIterator<UFunction> FunctionIt(InterfaceDesc.Interface.Get(), EFieldIteratorFlags::IncludeSuper); FunctionIt; ++FunctionIt) {
						UFunction* Function = *FunctionIt;
						if (Function->GetFName() == FunctionName) {
							Setup(BlueprintPtr, Function, InterfaceDesc);
							return true;
						}
					}
				}
			}
		} else {
			if (InterfaceClassPtr && FElgKEWUtils::GetInterfaceClassName(InterfaceClassPtr) == InterfaceName){
				return true;
			}
		}
	}

	UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("interfaceFunction are no longer valid!"));
	return false;
}


UElgBESGraphInterfaceFunction* UElgBESGraphInterfaceFunction::MakeGraphInterfaceFunction(UBlueprint* InBlueprint, UFunction* InFunction, FBPInterfaceDescription InInterface)
{
	UElgBESGraphInterfaceFunction* bpFunc = NewObject<UElgBESGraphInterfaceFunction>();
	bpFunc->Setup(InBlueprint, InFunction, InInterface);
	return bpFunc;
}


UElgBESGraphInterfaceFunction* UElgBESGraphInterfaceFunction::MakeGraphInterfaceFunction(UBlueprint* InBlueprint, UFunction* InFunction, UClass* InInterfaceClass)
{
	UElgBESGraphInterfaceFunction* bpFunc = NewObject<UElgBESGraphInterfaceFunction>();
	bpFunc->Setup(InBlueprint, InFunction, InInterfaceClass);
	return bpFunc;
}

#undef LOCTEXT_NAMESPACE