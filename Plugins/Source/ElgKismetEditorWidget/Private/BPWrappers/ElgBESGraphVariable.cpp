// Copyright 2019-2021 ElgSoft. All rights reserved. 


#include "ElgBESGraphVariable.h"
#include <ElgKEWUtils.h>
#include <Kismet2/BlueprintEditorUtils.h>
#include <ElgKEWEditorSubSystem.h>
#include <ElgEditorContext_BlueprintEditor.h>
#include "ElgKEW_Log.h"


void UElgBESGraphVariable::RemoveVariable(const bool bShowDialog /*= false*/)
{
	if (bShowDialog && IsInUse()) {
		EAppReturnType::Type DialogReturn = FMessageDialog::Open(EAppMsgType::YesNo, NSLOCTEXT("UnrealEd", "WidgetVariable", "The variable is referenced in the blueprint graph, are you sure you want remove it?"));
		if (DialogReturn == EAppReturnType::No) return;
	}
	FElgKEWUtils::RemoveVariable(BlueprintPtr, VariableName);
	ClearVariableDescription();
}

#pragma region Setters


void UElgBESGraphVariable::SetVariableName(FName InName)
{
	if (VariableName == InName) return;
	FBlueprintEditorUtils::RenameMemberVariable(BlueprintPtr, VariableName, InName);
	ValidateAndUpdate();
	UElgKEWEditorSubSystem* kewSS = GEditor->GetEditorSubsystem<UElgKEWEditorSubSystem>();
	if (UElgEditorContext_BlueprintEditor* context = kewSS->FindEditorContext(BlueprintPtr)) {
		context->HandleOnChanged(BlueprintPtr);
	}
}

void UElgBESGraphVariable::SetVariableType(FS_ElgGraphPinType InNewType)
{
	if (PinType.EdGraphPinType == InNewType.EdGraphPinType) return;
	FEdGraphPinType pinType = FElgKEWUtils::ConvertFromPinType(InNewType);
	FBlueprintEditorUtils::ChangeMemberVariableType(BlueprintPtr, VariableName, pinType);
	ValidateAndUpdate();
}

void UElgBESGraphVariable::SetVariableDefaultValue(const FString InValue)
{
	if (FProperty* variableProperty = FindFProperty<FProperty>(BlueprintPtr->GeneratedClass, VariableName)) {
		FElgKEWUtils::SetPropertyValueAsString(BlueprintPtr, variableProperty, InValue);
		ValidateAndUpdate();
		BlueprintPtr->Modify(true);
	}
}


void UElgBESGraphVariable::SetVariableInstanceEditable(const bool bSetFlag /*= true*/)
{
	FBlueprintEditorUtils::SetBlueprintOnlyEditableFlag(BlueprintPtr, VariableName, bSetFlag);
	ValidateAndUpdate();
}

void UElgBESGraphVariable::SetVariableBPReadOnly(const bool bSetFlag /*= true*/)
{
	FBlueprintEditorUtils::SetBlueprintPropertyReadOnlyFlag(BlueprintPtr, VariableName, bSetFlag);
	ValidateAndUpdate();
}

void UElgBESGraphVariable::SetVariableExposeOnSpawn(const bool bSetFlag /*= true*/)
{
	if (bSetFlag) {
		FBlueprintEditorUtils::SetBlueprintVariableMetaData(BlueprintPtr, VariableName, NULL, FBlueprintMetadata::MD_ExposeOnSpawn, TEXT("true"));
	}
	else {
		FBlueprintEditorUtils::RemoveBlueprintVariableMetaData(BlueprintPtr, VariableName, NULL, FBlueprintMetadata::MD_ExposeOnSpawn);
	}
	ValidateAndUpdate();
}

void UElgBESGraphVariable::SetVariablePrivate(const bool bSetFlag /*= true*/)
{
	if (bSetFlag) {
		FBlueprintEditorUtils::SetBlueprintVariableMetaData(BlueprintPtr, VariableName, NULL, FBlueprintMetadata::MD_Private, TEXT("true"));
	}
	else {
		FBlueprintEditorUtils::RemoveBlueprintVariableMetaData(BlueprintPtr, VariableName, NULL, FBlueprintMetadata::MD_Private);
	}
	ValidateAndUpdate();
}

void UElgBESGraphVariable::SetVariableExposeToCinematic(const bool bSetFlag /*= true*/)
{
	FBlueprintEditorUtils::SetInterpFlag(BlueprintPtr, VariableName, bSetFlag);
	ValidateAndUpdate();
}

void UElgBESGraphVariable::SetVariableReplicationCondition(ELifetimeCondition InCondition)
{
	int32 index = FBlueprintEditorUtils::FindNewVariableIndex(BlueprintPtr, VariableName);
	if (index != INDEX_NONE) {
		BlueprintPtr->NewVariables[index].ReplicationCondition = InCondition;
		FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(BlueprintPtr);
	}
	ValidateAndUpdate();
}

void UElgBESGraphVariable::SetVariableRepNotifyFunc(const FName InFunc)
{
	if (FProperty* variableProperty = FindFProperty<FProperty>(BlueprintPtr->GeneratedClass, VariableName)) {
		FBlueprintEditorUtils::SetBlueprintVariableRepNotifyFunc(BlueprintPtr, VariableName, InFunc);
		uint64* PropFlagPtr = FBlueprintEditorUtils::GetBlueprintVariablePropertyFlags(BlueprintPtr, VariableName);
		if (PropFlagPtr != NULL) {
			if (InFunc != NAME_None) {
				*PropFlagPtr |= CPF_RepNotify;
				*PropFlagPtr |= CPF_Net;
			}
			else {
				*PropFlagPtr &= ~CPF_RepNotify;
			}
		}
		ValidateAndUpdate();
	}
}


void UElgBESGraphVariable::SetVariableReplicationType(const EBPElgVariableReplication InType)
{
	uint64* PropFlagPtr = FBlueprintEditorUtils::GetBlueprintVariablePropertyFlags(BlueprintPtr, VariableName);
	if (PropFlagPtr == NULL) return;

	switch (InType) {
	case EBPElgVariableReplication::None:
		*PropFlagPtr &= ~CPF_Net;
		SetVariableRepNotifyFunc(FName(NAME_None));
		SetVariableReplicationCondition(ELifetimeCondition::COND_None);
		break;

	case EBPElgVariableReplication::Replicated:
		*PropFlagPtr |= CPF_Net;
		SetVariableRepNotifyFunc(FName(NAME_None));
		break;

	case EBPElgVariableReplication::RepNotify:
		*PropFlagPtr |= CPF_Net;
		FString NewFuncName = FString::Printf(TEXT("OnRep_%s"), *VariableName.ToString());
		UEdGraph* FuncGraph = FindObject<UEdGraph>(BlueprintPtr, *NewFuncName);
		if (!FuncGraph) {
			FuncGraph = FBlueprintEditorUtils::CreateNewGraph(BlueprintPtr, FName(*NewFuncName), UEdGraph::StaticClass(), UEdGraphSchema_K2::StaticClass());
			FBlueprintEditorUtils::AddFunctionGraph<UClass>(BlueprintPtr, FuncGraph, false, NULL);
		}

		if (FuncGraph) {
			SetVariableRepNotifyFunc(FName(*NewFuncName));
		}
		break;
	}
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(BlueprintPtr);
}


void UElgBESGraphVariable::SetVariableConfigState(const bool bSetFlag /*= true*/)
{
	SetVariableFlag(EBPElgKEWPropertyFlags::CPF_Config, bSetFlag);
	ValidateAndUpdate();
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(BlueprintPtr);
}

void UElgBESGraphVariable::SetVariableTransient(const bool bSetFlag /*= true*/)
{
	FBlueprintEditorUtils::SetVariableTransientFlag(BlueprintPtr, VariableName, bSetFlag);
	ValidateAndUpdate();
}

void UElgBESGraphVariable::SetVariableSaveGame(const bool bSetFlag /*= true*/)
{
	FBlueprintEditorUtils::SetVariableSaveGameFlag(BlueprintPtr, VariableName, bSetFlag);
	ValidateAndUpdate();
}

void UElgBESGraphVariable::SetVariableAdvanceDisplay(const bool bSetFlag /*= true*/)
{
	FBlueprintEditorUtils::SetVariableAdvancedDisplayFlag(BlueprintPtr, VariableName, bSetFlag);
	ValidateAndUpdate();
}

void UElgBESGraphVariable::SetVariableDepricated(const bool bSetFlag /*= true*/)
{
	FBlueprintEditorUtils::SetVariableDeprecatedFlag(BlueprintPtr, VariableName, bSetFlag);
	ValidateAndUpdate();
}

void UElgBESGraphVariable::SetVariableDeprecationMessage(const FText& InText)
{
	if (InText.IsEmpty()) {
		FBlueprintEditorUtils::RemoveBlueprintVariableMetaData(BlueprintPtr, VariableName, NULL, FBlueprintMetadata::MD_DeprecationMessage);
	} else {
		FBlueprintEditorUtils::SetBlueprintVariableMetaData(BlueprintPtr, VariableName, NULL, FBlueprintMetadata::MD_DeprecationMessage, InText.ToString());
	}
}

void UElgBESGraphVariable::SetVariableToolTip(const FText& InText)
{
	FBlueprintEditorUtils::SetBlueprintVariableMetaData(BlueprintPtr, VariableName, NULL, TEXT("tooltip"), InText.ToString());
}

#pragma endregion


#pragma region PropertyFlags


void UElgBESGraphVariable::SetVariableFlag(EBPElgKEWPropertyFlags InFlag, const bool bAdd /*= true*/)
{
	EPropertyFlags flag = FElgKEWUtils::ConvertToEPropertyFlags(InFlag);
	int32 index = FBlueprintEditorUtils::FindNewVariableIndex(BlueprintPtr, VariableName);
	if (index != INDEX_NONE) {
		if (bAdd) {
			BlueprintPtr->NewVariables[index].PropertyFlags |= flag;
		}
		else {
			BlueprintPtr->NewVariables[index].PropertyFlags &= ~flag;
		}
	}
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(BlueprintPtr);

}

bool UElgBESGraphVariable::HasVariableFlag(EBPElgKEWPropertyFlags InFlag)
{
	if (!ValidateAndUpdate()) return false;
	EPropertyFlags flag = FElgKEWUtils::ConvertToEPropertyFlags(InFlag);
	if (FProperty* variableProperty = FindFProperty<FProperty>(BlueprintPtr->GeneratedClass, VariableName)) {
		return variableProperty->HasAnyPropertyFlags(flag);
	}
	return false;
}

bool UElgBESGraphVariable::HasVariableFlags(TArray<EBPElgKEWPropertyFlags> InFlags)
{
	if (!ValidateAndUpdate()) return false;
	for (EBPElgKEWPropertyFlags flag : InFlags) {
		if (!HasVariableFlag(flag)) return false;
	}
	return true;
}

#pragma endregion


#pragma region Getters


bool UElgBESGraphVariable::GetVariableInstanceEditable()
{
	if (!ValidateAndUpdate()) return false;
	return !HasVariableFlag(EBPElgKEWPropertyFlags::CPF_DisableEditOnInstance);
}

bool UElgBESGraphVariable::GetVariableBlueprintReadOnly()
{
	if (!ValidateAndUpdate()) return false;
	return HasVariableFlag(EBPElgKEWPropertyFlags::CPF_BlueprintReadOnly);
}

bool UElgBESGraphVariable::GetVariableExposeOnSpawn()
{
	if (!ValidateAndUpdate()) return false;
	return HasVariableFlag(EBPElgKEWPropertyFlags::CPF_ExposeOnSpawn);

}

bool UElgBESGraphVariable::GetVariablePrivate()
{
	if (!ValidateAndUpdate()) return false;
	bool bPrivate = false;
	if (FProperty* variableProperty = FindFProperty<FProperty>(BlueprintPtr->GeneratedClass, VariableName)) {
		bPrivate = (variableProperty->GetBoolMetaData(FBlueprintMetadata::MD_Private) != false);
	}
	return bPrivate;
}

bool UElgBESGraphVariable::GetVariableExposeToCinematics()
{
	if (!ValidateAndUpdate()) return false;
	return HasVariableFlag(EBPElgKEWPropertyFlags::CPF_Interp);

}

bool UElgBESGraphVariable::GetVariableIsConfig()
{
	if (!ValidateAndUpdate()) return false;
	if (FProperty* variableProperty = FindFProperty<FProperty>(BlueprintPtr->GeneratedClass, VariableName)) {
		return variableProperty->HasAnyPropertyFlags(CPF_Config);
	}
	return false;

}

bool UElgBESGraphVariable::GetVariableTransient()
{
	if (!ValidateAndUpdate()) return false;
	if (FProperty* variableProperty = FindFProperty<FProperty>(BlueprintPtr->GeneratedClass, VariableName)) {
		return variableProperty->HasAnyPropertyFlags(CPF_Transient);
	}
	return false;
}

bool UElgBESGraphVariable::GetVariableSaveGame()
{
	if (!ValidateAndUpdate()) return false;
	if (FProperty* variableProperty = FindFProperty<FProperty>(BlueprintPtr->GeneratedClass, VariableName)) {
		return variableProperty->HasAnyPropertyFlags(CPF_SaveGame);
	}
	return false;
}

bool UElgBESGraphVariable::GetVariableAdvancedDisplay()
{
	if (!ValidateAndUpdate()) return false;
	if (FProperty* variableProperty = FindFProperty<FProperty>(BlueprintPtr->GeneratedClass, VariableName)) {
		return variableProperty->HasAnyPropertyFlags(CPF_AdvancedDisplay);
	}
	return false;
}

bool UElgBESGraphVariable::GetVariableDeprecated()
{
	if (!ValidateAndUpdate()) return false;
	if (FProperty* variableProperty = FindFProperty<FProperty>(BlueprintPtr->GeneratedClass, VariableName)) {
		return variableProperty->HasAnyPropertyFlags(CPF_Deprecated);
	}
	return false;
}

FString UElgBESGraphVariable::GetVariableDeprecatedMessage()
{
	if (!ValidateAndUpdate()) return FString();
	FName key = TEXT("DeprecationMessage");
	if (VariableDescription.HasMetaData(key)) {
		return VariableDescription.GetMetaData(key);
	}
	return FString();
}

ELifetimeCondition UElgBESGraphVariable::GetVariableReplicationCondition()
{
	if (!ValidateAndUpdate()) return ELifetimeCondition::COND_None;
	ELifetimeCondition repCondition = COND_None;
	if (FProperty* variableProperty = FindFProperty<FProperty>(BlueprintPtr->GeneratedClass, VariableName)) {
		repCondition = variableProperty->GetBlueprintReplicationCondition();
	}
	return repCondition;
}

FName UElgBESGraphVariable::GetVariableRepNotifyFunc()
{
	if (!ValidateAndUpdate()) return FName();
	return FBlueprintEditorUtils::GetBlueprintVariableRepNotifyFunc(BlueprintPtr, VariableName);
}

EBPElgVariableReplication UElgBESGraphVariable::GetVariableReplicationType()
{
	if (!ValidateAndUpdate()) return EBPElgVariableReplication::None;
	EBPElgVariableReplication varRep = EBPElgVariableReplication::None;

	uint64* propFlagPtr = FBlueprintEditorUtils::GetBlueprintVariablePropertyFlags(BlueprintPtr, VariableName);
	if (propFlagPtr != NULL) {
		uint64 propFlags = *propFlagPtr;
		bool isReplicated = (propFlags & CPF_Net) > 0;
		bool bHasRepNotify = FBlueprintEditorUtils::GetBlueprintVariableRepNotifyFunc(BlueprintPtr, VariableName) != NAME_None;
		if (bHasRepNotify) {
			UClass* genClass = BlueprintPtr->SkeletonGeneratedClass;
			UFunction* onRepFunc = genClass->FindFunctionByName(FBlueprintEditorUtils::GetBlueprintVariableRepNotifyFunc(BlueprintPtr, VariableName));
			// if the function no longer exist or are invalid remove it from the variable
			if (onRepFunc == NULL || onRepFunc->NumParms != 0 || onRepFunc->GetReturnProperty() != NULL) {
				bHasRepNotify = false;
				SetVariableRepNotifyFunc(FName(NAME_None));
			}
		}
		varRep = !isReplicated ? EBPElgVariableReplication::None :
			bHasRepNotify ? EBPElgVariableReplication::RepNotify : EBPElgVariableReplication::Replicated;
	}
	return varRep;
}

bool UElgBESGraphVariable::GetVariableIsReplicated()
{
	if (!ValidateAndUpdate()) return false;
	EBPElgVariableReplication varRep = GetVariableReplicationType();
	if (varRep == EBPElgVariableReplication::Replicated || varRep == EBPElgVariableReplication::RepNotify) {
		return true;
	}
	return false;
}

#pragma endregion


void UElgBESGraphVariable::Setup(UBlueprint* InBlueprint, FBPVariableDescription InVariableDescription)
{
	BlueprintPtr = InBlueprint;
	UpdateVariableDescription(InVariableDescription);
}



void UElgBESGraphVariable::Setup(UBlueprint* InBlueprint, FProperty* InVariableProperty)
{
	BlueprintPtr = InBlueprint;
	UpdateVariableProperty(InVariableProperty);
}

#pragma region overrides

EBPElgBESVariableType UElgBESGraphVariable::GetVarType()
{
	return EBPElgBESVariableType::Member;
}

FString UElgBESGraphVariable::GetDefaultValue()
{
	if (!ValidateAndUpdate()) return FString();
	return GetDefaultValueFromPropterty();
}

bool UElgBESGraphVariable::ValidateAndUpdate()
{
	if (!BlueprintPtr || VariableName.IsNone()) {
		UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("Variable [%s] object are invalid..."), *VariableName.ToString());
		return false;
	}
	for (FBPVariableDescription var : BlueprintPtr->NewVariables) {
		if (var.VarName == VariableName) {
			UpdateVariableDescription(var);
			return true;
		}
	}
	if (FProperty* variableProperty = FindFProperty<FProperty>(BlueprintPtr->GeneratedClass, VariableName)) {
		UpdateVariableProperty(variableProperty);
		return true;			 
	}

	UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("Failed to find any variable with name [%s]"), *VariableName.ToString());
	return false;
}


FString UElgBESGraphVariable::GetVarTipText()
{
	return GetVarPropteryTipText();
}


#pragma endregion



UElgBESGraphVariable* UElgBESGraphVariable::MakeGraphVariableObject(UBlueprint* InBlueprint, FBPVariableDescription InVariableDescription)
{
	UElgBESGraphVariable* bpVariable = NewObject<UElgBESGraphVariable>();
	bpVariable->Setup(InBlueprint, InVariableDescription);
	return bpVariable;
}

UElgBESGraphVariable* UElgBESGraphVariable::MakeGraphVariableObject(UBlueprint* InBlueprint, FProperty* InVariableProperty)
{
	UElgBESGraphVariable* bpVariable = NewObject<UElgBESGraphVariable>();
	bpVariable->Setup(InBlueprint, InVariableProperty);
	return bpVariable;
}
