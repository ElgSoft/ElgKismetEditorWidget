// Copyright 2019-2023 ElgSoft. All rights reserved. 


#include "ElgBESGraphEditablePinBase.h"
#include <ElgKEWUtils.h>
#include <Kismet2/BlueprintEditorUtils.h>
#include <Kismet2/KismetEditorUtilities.h>
#include "ElgKEW_Log.h"

#define LOCTEXT_NAMESPACE "FElgKismetEditorWidgetModule"


void UElgBESGraphEditablePinBase::Remove()
{
	if (!ValidateAndUpdate()) return;
	FElgKEWUtils::RemoveEditablePin(BlueprintPtr, NodePtr, PinName);
	PinPtr = nullptr;
	PinName = NAME_None;
}

#pragma region Setters

void UElgBESGraphEditablePinBase::SetDefaultValue(const FString InValue)
{
	if (!bInputPin) {
		UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("EditablePin [%s] is an OutputPin so you can't not set default value on it!"), *PinName.ToString());
		return;
	}
	if (!ValidateAndUpdate()) return;
	if (PinPtr->DefaultValue == InValue) return;

	const FScopedTransaction Transaction(LOCTEXT("ChangeDefaults", "Change Defaults"));
	NodePtr->Modify();
	BlueprintPtr->Modify();
	PinPtr->DefaultValue = InValue;
	FBlueprintEditorUtils::MarkBlueprintAsModified(BlueprintPtr);
}

void UElgBESGraphEditablePinBase::SetType(FS_ElgGraphPinType InNewType)
{
	if (!ValidateAndUpdate()) return;
	if (PinType.EdGraphPinType == InNewType.EdGraphPinType) return;
	if (!SupportExecPins() && InNewType.EPinCategory == EBPElgKEWPinCategory::PC_Exec) return;
	FElgKEWUtils::ChangeEditablePinType(NodePtr, PinName, InNewType);
}

void UElgBESGraphEditablePinBase::SetName(const FName InNewName)
{
	if (!ValidateAndUpdate()) return;
	PinName = FElgKEWUtils::ChangeEditablePinName(NodePtr, PinName, InNewName);
}

#pragma endregion


#pragma region PinOrder


bool UElgBESGraphEditablePinBase::MovePinUp()
{
	if (!ValidateAndUpdate()) return false;

	int32 currentIndex = INDEX_NONE;
	for (int32 index = 0; index < NodePtr->Pins.Num(); index++) {
		if (NodePtr->Pins[index]->PinId == Guid) {
			currentIndex = index;
		}
	}
	return FElgKEWUtils::MoveEditablePinUp(NodePtr, currentIndex);

}

bool UElgBESGraphEditablePinBase::MovePinDown()
{
	if (!ValidateAndUpdate()) return false;
	int32 currentIndex = INDEX_NONE;
	for (int32 index = 0; index < NodePtr->Pins.Num(); index++) {
		if (NodePtr->Pins[index]->PinId == Guid) {
			currentIndex = index;
		}
	}
	return FElgKEWUtils::MoveEditablePinDown(NodePtr, currentIndex);
}

#pragma endregion


#pragma region Getters


FString UElgBESGraphEditablePinBase::GetFriendlyName()
{
	if (!ValidateAndUpdate()) return FString();
	return FriendlyName;
}

FS_ElgGraphPinType UElgBESGraphEditablePinBase::GetPinType()
{
	if (!ValidateAndUpdate()) return FElgKEWUtils::GetDefaultPinType();
	return PinType;
}

FString UElgBESGraphEditablePinBase::GetDefaultValue()
{
	if (!bInputPin) return FString();
	if (!ValidateAndUpdate()) return FString();
	return DefaultValue;
}

bool UElgBESGraphEditablePinBase::GetIsInputPin()
{
	return bInputPin;
}

EBPElgKEWPinDirection UElgBESGraphEditablePinBase::GetDirection()
{
	return Direction;
}

#pragma endregion


void UElgBESGraphEditablePinBase::ShowInDetailsInternal()
{
	if (!ValidateAndUpdate()) return;
	FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(NodePtr, false);
}

FName UElgBESGraphEditablePinBase::GetNameInternal()
{
	if (!ValidateAndUpdate()) return NAME_None;
	return PinName;
}

void UElgBESGraphEditablePinBase::GetIconInternal(struct FSlateBrush& OutBrush)
{
	if (!ValidateAndUpdate()) return;
	FElgKEWUtils::GetVariableIconFromPinType(VarType, OutBrush);
}

void UElgBESGraphEditablePinBase::Setup(UBlueprint* InBlueprint, UEdGraph* InGraph, class UK2Node_EditablePinBase* InNode, UEdGraphPin* InPin)
{
	BlueprintPtr = InBlueprint;
	GraphPtr = InGraph;
	NodePtr = InNode;
	PinPtr = InPin;
	bCanAddExecPins = false;
	bInputPin = PinPtr->Direction == EGPD_Output ? true : false;
	Direction = bInputPin ? EBPElgKEWPinDirection::Input : EBPElgKEWPinDirection::Output;
	SetupCustom();
	UpdatePinProperties();
}


void UElgBESGraphEditablePinBase::UpdatePinProperties()
{
	if (PinPtr) {
		PinName = PinPtr->PinName;
		BaseName = PinName;
		VarType = PinPtr->PinType;
		PinType = FElgKEWUtils::ConvertToPinType(PinPtr->PinType);
		FriendlyName = PinPtr->PinFriendlyName.ToString();
		DefaultValue = PinPtr->DefaultValue;
		Guid = PinPtr->PinId;
	}
	else {
		UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("UpdateEditablePinProperties: Faild to find Pin [%s]"), *PinName.ToString());
		PinName = FName();
		BaseName = PinName;
		PinType = FElgKEWUtils::GetDefaultPinType();
		FriendlyName = FString();
		DefaultValue = FString();
		Guid.Invalidate();
	}
}

bool UElgBESGraphEditablePinBase::ValidateAndUpdate()
{
	if (NodePtr && Guid.IsValid()) {
		for (UEdGraphPin* pin : NodePtr->Pins) {
			if (pin->PinId == Guid) {
				PinPtr = pin;
				UpdatePinProperties();
				return true;
			}
		}
	}
	UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("EditablePin [%s] are no longer valid"), *PinName.ToString());
	return false;
}

#undef LOCTEXT_NAMESPACE