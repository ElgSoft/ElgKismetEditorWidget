// Copyright 2019-2021 ElgSoft. All rights reserved. 

#include "BPWrappers/ElgBESGraphVariableBase.h"
#include <ElgKEWUtils.h>
#include <ElgKEWEditorSubSystem.h>
#include <ElgEditorContext_BlueprintEditor.h>
#include <Kismet2/BlueprintEditorUtils.h>
#include <K2Node_Variable.h>
#include <ObjectEditorUtils.h>
#include "ElgKEW_Log.h"


void UElgBESGraphVariableBase::SetVariableCategory(FText InCategoryName)
{
	if (!ValidateAndUpdate()) return;
	if (Category.ToString() == InCategoryName.ToString()) return;
	UFunction* varScope = GetVariableScope();
	FBlueprintEditorUtils::SetBlueprintVariableCategory(BlueprintPtr, VariableName, varScope, InCategoryName);
	ValidateAndUpdate();

	UElgKEWEditorSubSystem* kewSS = GEditor->GetEditorSubsystem<UElgKEWEditorSubSystem>();
	if (UElgEditorContext_BlueprintEditor* context = kewSS->FindEditorContext(BlueprintPtr)) {
		if (context->BlueprintEditorPtr.IsValid()) {
			context->BlueprintEditorPtr.Pin()->RefreshEditors(ERefreshBlueprintEditorReason::BlueprintCompiled);
		}
	}		
}

FName UElgBESGraphVariableBase::Duplicate()
{
	return DuplicateVar();
}

#pragma region Getters


bool UElgBESGraphVariableBase::GetVariableIsValid()
{
	return IsVarValid();
}

FS_ElgGraphPinType UElgBESGraphVariableBase::GetVariablePinType()
{
	if (!ValidateAndUpdate()) return FElgKEWUtils::GetDefaultPinType();
	return PinType;
}

FName UElgBESGraphVariableBase::GetVariablePinTypeName()
{
	if (!ValidateAndUpdate()) return FElgKEWUtils::PinCategoryEnumToName(FElgKEWUtils::GetDefaultPinType().EPinCategory);
	return FElgKEWUtils::PinCategoryEnumToName(PinType.EPinCategory);
}

FString UElgBESGraphVariableBase::GetVariablePinTypeNameAsString()
{
	return GetVariablePinTypeName().ToString();
}

FString UElgBESGraphVariableBase::GetVariableDefaultValue()
{
	return GetDefaultValue();
}


FGuid UElgBESGraphVariableBase::GetVariableGuid()
{
	if (!ValidateAndUpdate()) return VariableGuid;
	return VariableGuid;
}

FString UElgBESGraphVariableBase::GetVariableGuidAsString()
{
	if (!ValidateAndUpdate()) return FString();
	return VariableGuid.ToString();
}

FString UElgBESGraphVariableBase::GetVariableFriendlyName()
{
	if (!ValidateAndUpdate()) return FString();
	return FriendlyName;
}

EBPElgBESVariableType UElgBESGraphVariableBase::GetVariableType()
{
	return GetVarType();
}

#pragma endregion


void UElgBESGraphVariableBase::GetIconFromProperty(FSlateBrush& OutBrush)
{
	FSlateColor iconColorOut;
	const FSlateBrush* secBrushOut;
	FSlateColor secColorOut;
	OutBrush = *FBlueprintEditor::GetVarIconAndColorFromProperty(PropertyPtr, iconColorOut, secBrushOut, secColorOut);
	OutBrush.TintColor = iconColorOut;
}

FString UElgBESGraphVariableBase::GetVarPropteryTipText()
{
	return PropertyPtr->GetToolTipText().ToString();
}

FString UElgBESGraphVariableBase::GetDefaultValueFromPropterty()
{
	if (FProperty* variableProperty = FindFProperty<FProperty>(BlueprintPtr->GeneratedClass, VariableName)) {
		DefaultValue = FElgKEWUtils::GetPropertyValueAsString(BlueprintPtr, variableProperty);
		return DefaultValue;
	}
	return FString();
}

FS_ElgGraphPinType UElgBESGraphVariableBase::GetPinTypeFromProperty()
{
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();
	K2Schema->ConvertPropertyToPinType(PropertyPtr, VarType);
	PinType = FElgKEWUtils::ConvertToPinType(VarType);
	return PinType;
}

FProperty* UElgBESGraphVariableBase::GetProperty()
{
	check(BlueprintPtr);
	return FindFProperty<FProperty>(BlueprintPtr->SkeletonGeneratedClass, VariableName);
}



int32 UElgBESGraphVariableBase::GetVarReferenceCount()
{
	int32 count = 0;
	for (UEdGraph* graph : FElgKEWUtils::GetAllGraphs(BlueprintPtr)) {
		TArray<UK2Node_Variable*> graphNodes;
		graph->GetNodesOfClass(graphNodes);
		for (UK2Node_Variable* node : graphNodes) {
			if (node->GetPropertyForVariable()->GetFName() == VariableName) {
				count++;
			}
		}
	}
	return count;
}


FName UElgBESGraphVariableBase::DuplicateVar()
{
	return FElgKEWUtils::DuplicateVariable(BlueprintPtr, GetProperty(), true);
}


bool UElgBESGraphVariableBase::IsVarValid()
{
	if (!VariableName.IsNone() && BlueprintPtr) {
		return true;
	}
	return false;
}

bool UElgBESGraphVariableBase::ValidateAndUpdate()
{
	if (!VariableName.IsNone() && BlueprintPtr) {
		return true;
	}
	UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("Failed to find any variable with name [%s]"), *VariableName.ToString());
	return false;
}


void UElgBESGraphVariableBase::ShowInDetailsInternal()
{
	if (PropertyPtr) {
		UElgKEWEditorSubSystem* kewSS = GEditor->GetEditorSubsystem<UElgKEWEditorSubSystem>();
		if (UElgEditorContext_BlueprintEditor* context = kewSS->FindEditorContext(BlueprintPtr)) {
			context->ShowPropertyInInspector(PropertyPtr);
		}
	}
}



FName UElgBESGraphVariableBase::GetNameInternal()
{
	if (!ValidateAndUpdate()) return FName();
	return VariableName;
}

FText UElgBESGraphVariableBase::GetToolTipInternal()
{
	return FText::FromString(GetVarTipText());
}

FText UElgBESGraphVariableBase::GetCategoryInternal()
{
	if (!ValidateAndUpdate()) return FText();
	return Category;
}

int32 UElgBESGraphVariableBase::GetCountInternal()
{
	return GetVarReferenceCount();
}

void UElgBESGraphVariableBase::GetIconInternal(struct FSlateBrush& OutBrush)
{
	if (!ValidateAndUpdate()) return;
	if (PropertyPtr) {
		GetIconFromProperty(OutBrush);
	} else {
		FElgKEWUtils::GetVariableIconFromPinType(VarType, OutBrush);
	}
}

void UElgBESGraphVariableBase::UpdateVariableDescription(FBPVariableDescription InVariableDescription)
{	
	VariableName = InVariableDescription.VarName;
	BaseName = VariableName;
	VariableGuid = InVariableDescription.VarGuid;
	VarType = InVariableDescription.VarType;
	FriendlyName = InVariableDescription.FriendlyName;
	Category = InVariableDescription.Category;
	PinType = FElgKEWUtils::ConvertToPinType(InVariableDescription.VarType);
	PropertyPtr = GetProperty();
	VariableDescription = InVariableDescription;
}


void UElgBESGraphVariableBase::UpdateVariableProperty(FProperty* InProperty)
{	
	VariableName = InProperty->GetFName();
	BaseName = VariableName;
	PropertyPtr = InProperty;
	Category = FElgKEWUtils::GetPropertyCategoryText(InProperty);
	FriendlyName = InProperty->GetFName().ToString();	
	VariableGuid = FBlueprintEditorUtils::FindMemberVariableGuidByName(BlueprintPtr, VariableName);
	GetPinTypeFromProperty();
}


void UElgBESGraphVariableBase::ClearVariableDescription()
{
	BaseName = NAME_None;
	VariableName = NAME_None;
	VariableGuid.Invalidate();
	VarType = FEdGraphPinType();
	FriendlyName.Empty();
	Category = FText();
	PinType = FElgKEWUtils::GetDefaultPinType();
	VariableDescription = FBPVariableDescription();
	PropertyPtr = nullptr;
}

