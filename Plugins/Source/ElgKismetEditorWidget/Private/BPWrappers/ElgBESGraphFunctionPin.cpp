// Copyright 2019-2021 ElgSoft. All rights reserved. 

#include "BPWrappers/ElgBESGraphFunctionPin.h"
#include <ElgKEWUtils.h>
#include <K2Node_VariableGet.h>
#include <K2Node_EditablePinBase.h>
#include <Kismet2/BlueprintEditorUtils.h>
#include <ElgKEWEditorSubSystem.h>


#define LOCTEXT_NAMESPACE "FElgKismetEditorWidgetModule"


void UElgBESGraphFunctionPin::PromoteToVariable(const bool bInToMemberVariable)
{
	UElgKEWEditorSubSystem* kewSS = GEditor->GetEditorSubsystem<UElgKEWEditorSubSystem>();
	if (UElgEditorContext_BlueprintEditor* context = kewSS->GetContext(BlueprintPtr)) {
		if (GetDefault<UEdGraphSchema_K2>()->CanPromotePinToVariable(*PinPtr, bInToMemberVariable)) {
			if (context->BlueprintEditorPtr.IsValid()) {
				context->BlueprintEditorPtr.Pin()->DoPromoteToVariable(BlueprintPtr, PinPtr, bInToMemberVariable);
			}
		}
	}
}


int32 UElgBESGraphFunctionPin::GetCountInternal()
{
	int32 count = 0;
	if (!bInputPin) return count;
	if (!ValidateAndUpdate()) return count;
	// any get nodes
	TArray<UK2Node_VariableGet*> graphNodes;
	GraphPtr->GetNodesOfClass(graphNodes);
	for (UK2Node_VariableGet* node : graphNodes) {
		if (node->GetPropertyForVariable()->GetFName() == PinName) {
			count++;
		}
	}

	// any direct connected
	count += PinPtr->LinkedTo.Num();
	return count;
}

UElgBESGraphFunctionPin* UElgBESGraphFunctionPin::MakeGraphPinVariable(UBlueprint* InBlueprint, UEdGraph* InGraph, class UK2Node_EditablePinBase* InEntry, UEdGraphPin* InPin)
{
	UElgBESGraphFunctionPin* pinVar = NewObject<UElgBESGraphFunctionPin>();
	pinVar->Setup(InBlueprint, InGraph, InEntry, InPin);
	return pinVar;
}

#undef LOCTEXT_NAMESPACE