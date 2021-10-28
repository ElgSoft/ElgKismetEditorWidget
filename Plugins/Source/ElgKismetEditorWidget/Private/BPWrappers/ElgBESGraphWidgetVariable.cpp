// Copyright 2019-2021 ElgSoft. All rights reserved. 


#include "BPWrappers/ElgBESGraphWidgetVariable.h"
#include <WidgetBlueprint.h>
#include <Blueprint/WidgetTree.h>
#include <ElgKEWUtils.h>
#include <K2Node_ComponentBoundEvent.h>
#include <K2Node_Variable.h>

UWidget* UElgBESGraphWidgetVariable::GetWidget()
{
	check(BlueprintPtr);
	UWidgetBlueprint* widgetBP = Cast<UWidgetBlueprint>(BlueprintPtr);
	return widgetBP->WidgetTree->FindWidget(VariableName);
}


int32 UElgBESGraphWidgetVariable::GetVarReferenceCount()
{
	int32 count = 0;

	// check for Variable nodes that reference it
	for (UEdGraph* graph : FElgKEWUtils::GetAllGraphs(BlueprintPtr)) {
		TArray<UK2Node_Variable*> graphNodes;
		graph->GetNodesOfClass(graphNodes);
		for (UK2Node_Variable* node : graphNodes) {
			if (node->GetPropertyForVariable()->GetFName() == VariableName) {
				count++;
			}
		}
		TArray<UK2Node_ComponentBoundEvent*> eventNodes;
		graph->GetNodesOfClass(eventNodes);
		if (eventNodes.Num()) {
			for (UK2Node_ComponentBoundEvent* nodeEvent : eventNodes) {
				if (nodeEvent->ComponentPropertyName == VariableName) {
					count++;
				}
			}
		}

	}
	return count;
}


EBPElgBESVariableType UElgBESGraphWidgetVariable::GetVarType()
{
	return EBPElgBESVariableType::Widget;
}


bool UElgBESGraphWidgetVariable::IsVarValid()
{
	return ValidateAndUpdate();
}

bool UElgBESGraphWidgetVariable::ValidateAndUpdate()
{
	PropertyPtr = GetProperty();
	if (PropertyPtr) {
		UpdateVariableProperty(PropertyPtr);
		if (Category.ToString().Equals(BlueprintPtr->GetFriendlyName())) {
			Category = FText::FromString("Default");
		}
		WidgetPtr = GetWidget();
		return true;
	}
	return false;
}

void UElgBESGraphWidgetVariable::Setup(UBlueprint* InBlueprint, FProperty* InProperty)
{
	BlueprintPtr = InBlueprint;
	WidgetPtr = GetWidget();
	UpdateVariableProperty(InProperty);
	if (Category.ToString().Equals(InBlueprint->GetFriendlyName())) {
		Category = FText::FromString("Default");
	}
}


UElgBESGraphWidgetVariable* UElgBESGraphWidgetVariable::MakeGraphWidgetVariable(UBlueprint* InBlueprint, FProperty* InProperty)
{
	UElgBESGraphWidgetVariable* bpVariable = NewObject<UElgBESGraphWidgetVariable>();
	bpVariable->Setup(InBlueprint, InProperty);
	return bpVariable;
}

