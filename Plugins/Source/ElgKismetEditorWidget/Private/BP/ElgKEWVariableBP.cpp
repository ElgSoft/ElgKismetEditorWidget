// Copyright 2019-2021 ElgSoft. All rights reserved. 


#include "ElgKEWVariableBP.h"
#include "ElgKEWStructs.h"
#include "ElgKEWEnum.h"
#include <ElgEditorContext_BlueprintEditor.h>
#include <ElgKEWEditorSubSystem.h>
#include <ElgKEWUtils.h>

#include <ElgBESGraphVariable.h>



void UElgKEWVariableBP::SortVariableByName(TArray<UElgBESGraphVariable*> InVariables)
{
	InVariables.Sort([&](const UElgBESGraphVariable& varA, const UElgBESGraphVariable& varB) {
		//return varA.Name.to < varB.Name;
		return !varA.VariableName.FastLess(varB.VariableName);
		});
}


bool UElgKEWVariableBP::FindVariableName(TArray<UElgBESGraphVariable*> InVariables, const FName InName, UElgBESGraphVariable*& OutVariable)
{
	for (UElgBESGraphVariable* var : InVariables) {
		if (var->VariableName == InName) {
			OutVariable = var;
			return true;
		}
	}
	return false;
}

bool UElgKEWVariableBP::FindVariableGuid(TArray<UElgBESGraphVariable*> InVariables, const FGuid InGuid, UElgBESGraphVariable*& OutVariable)
{
	for (UElgBESGraphVariable* var : InVariables) {
		if (var->GetVariableGuid() == InGuid) {
			OutVariable = var;
			return true;
		}
	}
	return false;
}


TArray<UElgBESGraphVariable*> UElgKEWVariableBP::FilterVariableExposeOnSpawn(TArray<UElgBESGraphVariable*> InVariables)
{
	TArray<UElgBESGraphVariable*> out;
	for (UElgBESGraphVariable* var : InVariables){
		if (var->HasVariableFlag(EBPElgKEWPropertyFlags::CPF_ExposeOnSpawn)) {
			out.Add(var);
		}
	}
	return out;
}


TArray<UElgBESGraphVariable*> UElgKEWVariableBP::FilterVariableSaveGame(TArray<UElgBESGraphVariable*> InVariables)
{
	TArray<UElgBESGraphVariable*> out;
	for (UElgBESGraphVariable* var : InVariables) {
		if (var->GetVariableSaveGame()) {
			out.Add(var);
		}
	}
	return out;
}

TArray<UElgBESGraphVariable*> UElgKEWVariableBP::FilterVariableCategory(TArray<UElgBESGraphVariable*> InVariables, const FString InCategory)
{
	TArray<UElgBESGraphVariable*> out;
	for (UElgBESGraphVariable* var : InVariables) {
		if (var->GetCategory().ToString() == InCategory) {
			out.Add(var);
		}
	}
	return out;
}

TArray<UElgBESGraphVariable*> UElgKEWVariableBP::FilterVariableType(TArray<UElgBESGraphVariable*> InVariables, EBPElgKEWPinCategory InType)
{
	TArray<UElgBESGraphVariable*> out;
	for (UElgBESGraphVariable* var : InVariables) {
		if (var->GetVariablePinType().EPinCategory == InType) {
			out.Add(var);
		}
	}
	return out;
}

#define LOCTEXT_NAMESPACE "BlueprintPalette"
FText UElgKEWVariableBP::GetVariableExposedTipText(const bool bExposedState)
{
	FText toolTipText = FText::GetEmpty();
	if (bExposedState) {
		toolTipText = LOCTEXT("VariablePrivacy_is_public_Tooltip", "Variable is public and is editable on each instance of this Blueprint.");
	} else {
		toolTipText = LOCTEXT("VariablePrivacy_not_public_Tooltip", "Variable is not public and will not be editable on an instance of this Blueprint.");
	}
	return toolTipText;
}
#undef LOCTEXT_NAMESPACE

void UElgKEWVariableBP::GetVariableExposedIcon(const bool bExposedState, FSlateBrush& OutBrush)
{
	if (bExposedState) {
		OutBrush = *FEditorStyle::GetBrush("Kismet.VariableList.ExposeForInstance");
	} else {
		OutBrush = *FEditorStyle::GetBrush("Kismet.VariableList.HideForInstance");
	}
}


void UElgKEWVariableBP::GetVariableExposedColor(const bool bExposedState, FLinearColor& OutColor)
{
	if (bExposedState) {
		OutColor = FColor(215, 219, 119).ReinterpretAsLinear();
	} else {
		OutColor = FColor(64, 64, 64).ReinterpretAsLinear();
	}
}


