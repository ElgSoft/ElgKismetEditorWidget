// Copyright 2019-2021 ElgSoft. All rights reserved. 


#include "ElgBESGraphFunction.h"
#include <ElgKEWUtils.h>
#include <ElgKEWEditorSubSystem.h>
#include <ElgEditorContext_BlueprintEditor.h>
#include <ElgBESGraphVariableLocal.h>

#include <Kismet2/KismetEditorUtilities.h>
#include <Kismet2/BlueprintEditorUtils.h>
#include <K2Node_CallFunction.h>
#include <K2Node_CreateDelegate.h>
#include <GraphEditorSettings.h>
#include <K2Node_FunctionTerminator.h>
#include <K2Node_FunctionResult.h>
#include <K2Node_FunctionEntry.h>
#include "ObjectEditorUtils.h"
#include "ElgKEW_Log.h"


#define LOCTEXT_NAMESPACE "FElgKismetEditorWidgetModule"

#pragma region FunctionSetters

void UElgBESGraphFunction::SetIsPure(const bool bIsPure /*= true*/)
{
	if (!ValidateFunction()) return;

	UFunction* func = GetFunction();
	if (FElgKEWUtils::IsFunctionPure(func) == bIsPure) return;

	const FScopedTransaction Transaction(LOCTEXT("ChangePure", "Change Pure"));
	EntryNodePtr->Modify();
	func->Modify();
	func->FunctionFlags ^= EFunctionFlags::FUNC_BlueprintPure;
	EntryNodePtr->SetExtraFlags(EntryNodePtr->GetExtraFlags() ^ EFunctionFlags::FUNC_BlueprintPure);
	FElgKEWUtils::UpdatedNodeParameters(EntryNodePtr);
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
}


void UElgBESGraphFunction::SetAccessSpecifier(EBPElgKEWFunctionAccess InAccessSpecifier)
{
	if (!ValidateFunction()) return;

	UFunction* func = GetFunction();
	const FScopedTransaction Transaction(LOCTEXT("ChangeAccessSpecifier", "Change Access Specifier"));
	EntryNodePtr->Modify();
	func->Modify();

	const EFunctionFlags clearAccessSpecifierMask = ~FUNC_AccessSpecifiers;
	int32 extraFlags = EntryNodePtr->GetExtraFlags();
	extraFlags &= clearAccessSpecifierMask;

	EFunctionFlags flag = EFunctionFlags::FUNC_Public;
	switch (InAccessSpecifier) {
	case EBPElgKEWFunctionAccess::Public:		flag = EFunctionFlags::FUNC_Public; break;
	case EBPElgKEWFunctionAccess::Protected:	flag = EFunctionFlags::FUNC_Protected; break;
	case EBPElgKEWFunctionAccess::Private:		flag = EFunctionFlags::FUNC_Private; break;
	}
	extraFlags |= flag;
	EntryNodePtr->SetExtraFlags(extraFlags);
	FElgKEWUtils::UpdatedNodeParameters(EntryNodePtr);
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
}


void UElgBESGraphFunction::SetCategory(FText InText)
{
	if (!ValidateFunction()) return;
	if (InText.IsEmpty()) {
		InText = FText::FromString(TEXT("Default"));
	}
	FBlueprintEditorUtils::SetBlueprintFunctionOrMacroCategory(GraphPtr, InText);

	UElgKEWEditorSubSystem* kewSS = GEditor->GetEditorSubsystem<UElgKEWEditorSubSystem>();
	if (UElgEditorContext_BlueprintEditor* context = kewSS->FindEditorContext(GetBlueprint())) {
		context->bOnGraphChanged = true;
	}
}


void UElgBESGraphFunction::SetTipText(const FText InText)
{
	if (!ValidateFunction()) return;
	UFunction* func = GetFunction();
	FScopedTransaction Transaction(LOCTEXT("SetBlueprintFunctionMetaData", "Set Function MetaData"));
	FBlueprintEditorUtils::ModifyFunctionMetaData(GraphPtr);
	func->Modify();
	func->SetMetaData(FBlueprintMetadata::MD_Tooltip, *InText.ToString());
	FElgKEWUtils::UpdatedNodeParameters(EntryNodePtr);
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
}


void UElgBESGraphFunction::SetKeywords(const FText InText)
{
	if (!ValidateFunction()) return;
	UFunction* func = GetFunction();
	FScopedTransaction Transaction(LOCTEXT("SetBlueprintFunctionMetaData", "Set Function MetaData"));
	FBlueprintEditorUtils::ModifyFunctionMetaData(GraphPtr);
	func->Modify();
	func->SetMetaData(FBlueprintMetadata::MD_FunctionKeywords, *InText.ToString());
	FElgKEWUtils::UpdatedNodeParameters(EntryNodePtr);
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
}


void UElgBESGraphFunction::SetCompactNodeTitle(const FText InText)
{
	if (!ValidateFunction()) return;
	UFunction* func = GetFunction();
	FScopedTransaction Transaction(LOCTEXT("SetBlueprintFunctionMetaData", "Set Function MetaData"));
	FBlueprintEditorUtils::ModifyFunctionMetaData(GraphPtr);
	func->Modify();
	func->SetMetaData(FBlueprintMetadata::MD_CompactNodeTitle, *InText.ToString());
	FElgKEWUtils::UpdatedNodeParameters(EntryNodePtr);
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
}

void UElgBESGraphFunction::SetDeprecationMessage(const FString InText)
{
	if (!ValidateFunction()) return;
	UFunction* func = GetFunction();
	FScopedTransaction Transaction(LOCTEXT("SetBlueprintFunctionMetaData", "Set Function MetaData"));
	FBlueprintEditorUtils::ModifyFunctionMetaData(GraphPtr);
	func->Modify();
	func->SetMetaData(FBlueprintMetadata::MD_DeprecationMessage, *InText);
	FElgKEWUtils::UpdatedNodeParameters(EntryNodePtr);
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
}

void UElgBESGraphFunction::SetIsDeprecated(const bool InState)
{
	if (!ValidateFunction()) return;
	UFunction* func = GetFunction();
	FScopedTransaction Transaction(LOCTEXT("SetBlueprintFunctionMetaData", "Set Function MetaData"));
	FBlueprintEditorUtils::ModifyFunctionMetaData(GraphPtr);
	func->Modify();
	FString depricated = InState ? "true" : "false";
	func->SetMetaData(FBlueprintMetadata::MD_DeprecatedFunction, *depricated);
	FElgKEWUtils::UpdatedNodeParameters(EntryNodePtr);
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
}

void UElgBESGraphFunction::SetCallInEditor(const bool InState)
{
	if (!ValidateFunction()) return;
	UFunction* func = GetFunction();
	FScopedTransaction Transaction(LOCTEXT("SetBlueprintFunctionMetaData", "Set Function MetaData"));
	FBlueprintEditorUtils::ModifyFunctionMetaData(GraphPtr);
	func->Modify();
	FString callInEditor = InState ? "true" : "false";
	func->SetMetaData(FBlueprintMetadata::MD_CallInEditor, *callInEditor);
	FElgKEWUtils::UpdatedNodeParameters(EntryNodePtr);
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
}

void UElgBESGraphFunction::SetFunctionName(FName InNewName)
{
	if (!ValidateFunction()) return;
	if (InNewName.IsNone()) return;
	UEdGraph* ExistingGraph = FindObject<UEdGraph>(GraphPtr->GetOuter(), *InNewName.ToString());
	if (ExistingGraph == nullptr || ExistingGraph == GraphPtr) {
		const FScopedTransaction Transaction(LOCTEXT("RenameFunction", "Rename Function"));
		GraphPtr->Modify();
		FBlueprintEditorUtils::RenameGraph(GraphPtr, InNewName.ToString());
	}
}


#pragma endregion


#pragma region Misc


void UElgBESGraphFunction::OpenFunctionGraph()
{
	if (!ValidateFunction()) return;
	FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(GraphPtr, false);
}


void UElgBESGraphFunction::Remove(const bool bShowDialog /*= false*/)
{
	if (!ValidateFunction()) return;
	if (GraphPtr && GraphPtr->bAllowDeletion)
	{

		if (bShowDialog && IsInUse()) {
			EAppReturnType::Type DialogReturn = FMessageDialog::Open(EAppMsgType::YesNo, NSLOCTEXT("UnrealEd", "Function", "The Function is referenced in the blueprint graph, are you sure you want remove it?"));
			if (DialogReturn == EAppReturnType::No) return;
		}

		const FScopedTransaction Transaction(LOCTEXT("RemoveFunction", "Remove Function"));
		GetBlueprint()->Modify();
		GraphPtr->Modify();

		UElgKEWEditorSubSystem* kewSS = GEditor->GetEditorSubsystem<UElgKEWEditorSubSystem>();
		if (UElgEditorContext_BlueprintEditor* context = kewSS->FindEditorContext(GetBlueprint())) {
			context->CloseTab(GraphPtr);
		}
		FBlueprintEditorUtils::RemoveGraph(GetBlueprint(), GraphPtr, EGraphRemoveFlags::Recompile);

		for (TObjectIterator<UK2Node_CreateDelegate> It(RF_ClassDefaultObject, /** bIncludeDerivedClasses */ true, /** InternalExcludeFlags */ EInternalObjectFlags::PendingKill); It; ++It)
		{
			if (It->GetGraph() != GraphPtr) {
				if (!It->IsPendingKill() && It->GetGraph() && !It->GetGraph()->IsPendingKill()) {
					It->HandleAnyChange();
				}
			}
		}
		
		// invalidate this object as the function is removed.
		GraphPtr = nullptr;
		Name = NAME_None;
		Guid.Invalidate();
	}
}


void UElgBESGraphFunction::Duplicate(UElgBESGraphFunction*& OutFunction)
{
	if (!ValidateFunction()) return;
	if (UEdGraph* newGraph= FElgKEWUtils::DuplcateGraph(GetBlueprint(), GraphPtr)) {
		OutFunction = UElgBESGraphFunction::MakeGraphFunctionObject(newGraph);
		return;
	}
	OutFunction = nullptr;
}


#pragma endregion


#pragma region FunctionFlags

bool UElgBESGraphFunction::HasFunctionFlag(EElgFunctionFlags InFlag)
{
	if (!ValidateFunction()) return false;
	return FElgKEWUtils::DoFunctionHasFlag(GetFunction(), InFlag);
}

bool UElgBESGraphFunction::HasFunctionFlags(TArray<EElgFunctionFlags> InFlags)
{
	if (!ValidateFunction()) return false;
	UFunction* func = GetFunction();
	for (EElgFunctionFlags flag : InFlags) {
		if (!FElgKEWUtils::DoFunctionHasFlag(func, flag)) return false;
	}
	return true;
}


#pragma endregion


#pragma region LocalVariables

TArray<UElgBESGraphVariableLocal*> UElgBESGraphFunction::GetFunctionLocalVariables()
{
	if (!ValidateFunction()) {
		TArray<UElgBESGraphVariableLocal*> vars;
		return vars;
	}
	return FElgKEWUtils::GetLocalVariables(GetBlueprint(), GraphPtr);

}


bool UElgBESGraphFunction::GetLocalVariableByName(const FName InName, UElgBESGraphVariableLocal*& OutVariable)
{
	for (UElgBESGraphVariableLocal* var : FElgKEWUtils::GetLocalVariables(GetBlueprint(), GraphPtr)) {
		if (var->VariableName == InName) {
			OutVariable = var;
			return true;
		}
	}
	OutVariable = nullptr;
	return false;
}

bool UElgBESGraphFunction::FunctionAddLocalVariable(const FName InVariableName, const FS_ElgGraphPinType InType, const FText InCategoryName, const FString InDefaultValue, UElgBESGraphVariableLocal*& OutVariable)
{
	OutVariable = nullptr;
	FString nameErrorMsg; 
	if (!FElgKEWUtils::NameValidator(GetBlueprint(), NAME_None, InVariableName, nameErrorMsg, GetFunction())) {
		UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("Failed to AddLocalVariable to function %s: %s"), *Name.ToString(), *nameErrorMsg);
		return false;
	}

	if (FBlueprintEditorUtils::AddLocalVariable(GetBlueprint(), GraphPtr, InVariableName, FElgKEWUtils::ConvertFromPinType(InType), InDefaultValue)) {
		for (UElgBESGraphVariableLocal* var : FElgKEWUtils::GetLocalVariables(GetBlueprint(), GraphPtr)) {
			if (var->VariableName == InVariableName) {
				OutVariable = var;
				OutVariable->SetVariableCategory(InCategoryName);
				return true;
			}
		}
		return true;
	}
	return false;
}

void UElgBESGraphFunction::FunctionRemoveLocalVariableByName(const FName InVariableName)
{
	for (UElgBESGraphVariableLocal* var : GetFunctionLocalVariables()) {
		if (var->VariableName == InVariableName) {
			var->RemoveVariable();
			return;
		}
	}
}

bool UElgBESGraphFunction::IsNameValid(const FName InName, EBPElgKEWWResult& Branch)
{
	FString message;
	if (FElgKEWUtils::NameValidator(GetBlueprint(), NAME_None, InName, message, GetFunction())) {
		Branch = EBPElgKEWWResult::outTrue;
		return true;
	}
	UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("NameValidation falied: %s"), *message);
	Branch = EBPElgKEWWResult::outFalse;
	return false;
}


#pragma endregion


#pragma region PinVariables


TArray<UElgBESGraphFunctionPin*> UElgBESGraphFunction::GetFunctionPinVariables()
{
	TArray<UElgBESGraphFunctionPin*> outVars;
	if (!ValidateFunction()) return outVars;
	outVars.Append(FElgKEWUtils::GetInputPinVariables(GetBlueprint(), GraphPtr));
	outVars.Append(FElgKEWUtils::GetOutputPinVariables(GetBlueprint(), GraphPtr));
	
	outVars.Sort([&](const UElgBESGraphFunctionPin& dataA, const UElgBESGraphFunctionPin& dataB) {
		return dataA.PinName.ToString() < dataB.PinName.ToString();
		});
	
	return outVars;
}


TArray<UElgBESGraphFunctionPin*> UElgBESGraphFunction::GetFunctionInputPinVariables()
{
	TArray<UElgBESGraphFunctionPin*> outVars;
	if (!ValidateFunction()) return outVars;
	outVars = FElgKEWUtils::GetInputPinVariables(GetBlueprint(), GraphPtr);
	outVars.Sort([&](const UElgBESGraphFunctionPin& dataA, const UElgBESGraphFunctionPin& dataB) {
		return dataA.PinName.ToString() < dataB.PinName.ToString();
		});
	return outVars;
}


bool UElgBESGraphFunction::GetFunctionInputPinVariable(const FName InName, UElgBESGraphFunctionPin*& OutPinVariable)
{
	if (!ValidateFunction()) return false;
	OutPinVariable = FElgKEWUtils::GetInputPinVariable(GetBlueprint(), GraphPtr, InName);
	if (OutPinVariable->PinName.IsNone()) return false;
	return true;
}


TArray<UElgBESGraphFunctionPin*> UElgBESGraphFunction::GetFunctionOutputPinVariables()
{
	TArray<UElgBESGraphFunctionPin*> outVars;
	if (!ValidateFunction()) return outVars;
	outVars = FElgKEWUtils::GetOutputPinVariables(GetBlueprint(), GraphPtr);
	outVars.Sort([&](const UElgBESGraphFunctionPin& dataA, const UElgBESGraphFunctionPin& dataB) {
		return dataA.PinName.ToString() < dataB.PinName.ToString();
		});
	return outVars;
}


bool UElgBESGraphFunction::GetFunctionOutputPinVariable(const FName InName, UElgBESGraphFunctionPin*& OutPinVariable)
{
	if (!ValidateFunction()) return false;
	OutPinVariable = FElgKEWUtils::GetInputPinVariable(GetBlueprint(), GraphPtr, InName);
	if (OutPinVariable->PinName.IsNone()) return false;
	return true;
}

UElgBESGraphFunctionPin* UElgBESGraphFunction::FunctionAddInputPin(FS_ElgGraphPinType InType, FName InPinName, const FString InDefaultValue)
{
	if (!ValidateFunction()) return nullptr;
	if (InPinName.IsNone()) {
		InPinName = FName(TEXT("l_newparam"));
	}	
	UElgBESGraphFunctionPin* newPin = nullptr;
	InPinName = FElgKEWUtils::AddEditablePin(GetBlueprint(), FElgKEWUtils::GetFunctionEntryNodeFromGraph(GraphPtr), InPinName, InType, InDefaultValue, EEdGraphPinDirection::EGPD_Output);
	GetFunctionInputPinVariable(InPinName, newPin);
	return newPin;
}

UElgBESGraphFunctionPin* UElgBESGraphFunction::FunctionAddOutputPin(FS_ElgGraphPinType InType, FName InPinName)
{
	if (!ValidateFunction()) return nullptr;
	if (InPinName.IsNone()) {
		InPinName = FName(TEXT("l_newparam"));
	}
	UElgBESGraphFunctionPin* newPin = nullptr;
	InPinName = FElgKEWUtils::AddEditablePin(GetBlueprint(), FElgKEWUtils::GetFunctionResultNodeFromGraph(GraphPtr), InPinName, InType, FString(), EEdGraphPinDirection::EGPD_Input);
	GetFunctionOutputPinVariable(InPinName, newPin);
	return newPin;
}

#pragma endregion


#pragma region FunctionGetters


FGuid UElgBESGraphFunction::GetGuid()
{
	if (!ValidateFunction()) return FGuid();
	Guid = GraphPtr->GraphGuid;
	return Guid;
}

bool UElgBESGraphFunction::GetIsPure()
{
	if (!ValidateFunction()) return false;
	return (GetFunction()->HasAnyFunctionFlags(EFunctionFlags::FUNC_BlueprintPure)) ? true : false;
}


FName UElgBESGraphFunction::GetBlueprintName()
{
	if (!ValidateFunction()) return FName();
	return GetBlueprint()->GetFName();
}

FText UElgBESGraphFunction::GetKeywords()
{
	if (!ValidateFunction()) return FText();
	return EntryNodePtr->MetaData.Keywords;
}

FText UElgBESGraphFunction::GetCompactNodeTitle()
{
	if (!ValidateFunction()) return FText();
	return EntryNodePtr->MetaData.CompactNodeTitle;
}

FString UElgBESGraphFunction::GetDeprecationMessage()
{
	if (!ValidateFunction()) return FString();
	return EntryNodePtr->MetaData.DeprecationMessage;
}

bool UElgBESGraphFunction::GetIsDeprecated()
{
	if (!ValidateFunction()) return false;
	return EntryNodePtr->MetaData.bIsDeprecated;
}

bool UElgBESGraphFunction::GetCallInEditor()
{
	if (!ValidateFunction()) return false;
	return EntryNodePtr->MetaData.bCallInEditor;
}

bool UElgBESGraphFunction::GetSupportDragDrop()
{
	if (!ValidateFunction()) return false;
	UFunction* func = GetFunction();
	bool isCodeEvent = (func->HasAnyFunctionFlags(FUNC_Event));
	return (func->HasAnyFunctionFlags((FUNC_BlueprintCallable | FUNC_BlueprintPure)) && !isCodeEvent);
}

int32 UElgBESGraphFunction::GetNumParamsp()
{
	if (!ValidateFunction()) return 0;
	return GetFunction()->NumParms;
}

bool UElgBESGraphFunction::GetHasReturnValue()
{
	if (!ValidateFunction()) return false;
	return (GetFunction()->GetReturnProperty()) ? true : false;
}

bool UElgBESGraphFunction::GetIsValid()
{
	if (GraphPtr && Guid.IsValid()) return true;
	return false;
}

void UElgBESGraphFunction::IsValidBranch(EBPElgKEWWResult& Branch)
{
	Branch = GetIsValid() ? EBPElgKEWWResult::outTrue : EBPElgKEWWResult::outFalse;
}


bool UElgBESGraphFunction::GetIsOverride()
{
	if (!ValidateFunction()) return false; 
	if (!GetBlueprint()->ParentClass) return false;
	if (UFunction* overrideFunc = FindUField<UFunction>(GetBlueprint()->ParentClass, Name)) {
		return true;
	}
	return false;
}

bool UElgBESGraphFunction::GetIsEditable()
{
	if (!ValidateFunction()) return false;
	bool res = FBlueprintEditorUtils::IsGraphReadOnly(GraphPtr);
	return !res;
}


void UElgBESGraphFunction::ShowInDetailsInternal()
{
	if (!ValidateFunction()) return;
	UElgKEWEditorSubSystem* kewSS = GEditor->GetEditorSubsystem<UElgKEWEditorSubSystem>();
	if (UElgEditorContext_BlueprintEditor* context = kewSS->FindEditorContext(GetBlueprint())) {
		context->ShowGraphInInspector(GraphPtr);
	}
}

FName UElgBESGraphFunction::GetNameInternal()
{
	if (!ValidateFunction()) return FName();
	Name = GraphPtr->GetFName();
	return Name;
}

FText UElgBESGraphFunction::GetToolTipInternal()
{
	if (!ValidateFunction()) return FText();
	return EntryNodePtr->MetaData.ToolTip;
}

FText UElgBESGraphFunction::GetCategoryInternal()
{
	if (!ValidateFunction()) return FText();
	return FObjectEditorUtils::GetCategoryText(GetFunction());
}

int32 UElgBESGraphFunction::GetCountInternal()
{
	int32 usage = 0;
	if (!ValidateFunction()) return usage;

	for (UEdGraph* graph : FElgKEWUtils::GetAllGraphs(GetBlueprint())) {
		TArray<UK2Node_CallFunction*> callNodes;
		graph->GetNodesOfClass(callNodes);
		for (UK2Node_CallFunction* node : callNodes) {
			if (FElgKEWUtils::GetCallfunctionMemberGuid(node) == Guid) {
				usage++;
			}
		}
		TArray<UK2Node_CreateDelegate*> delegateNodes;
		graph->GetNodesOfClass(delegateNodes);
		for (UK2Node_CreateDelegate* nodeDelegate : delegateNodes) {
			if (nodeDelegate->GetFunctionName() == Name) {
				usage++;
			}
		}
	}
	return usage;
}

void UElgBESGraphFunction::GetIconInternal(struct FSlateBrush& OutBrush)
{
	if (!ValidateFunction()) return;

	FLinearColor tint = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	if (GetIsPure()) {
		tint = GetDefault<UGraphEditorSettings>()->PureFunctionCallNodeTitleColor;
	} else {
		tint = GetDefault<UGraphEditorSettings>()->FunctionCallNodeTitleColor;
	}

	if (UFunction* overrideFunc = FindUField<UFunction>(GetBlueprint()->ParentClass, Name)) {
		const bool bIsPureFunction = overrideFunc && overrideFunc->HasAnyFunctionFlags(FUNC_BlueprintPure);
		OutBrush = *FEditorStyle::GetBrush(bIsPureFunction ? TEXT("GraphEditor.OverridePureFunction_16x") : TEXT("GraphEditor.OverrideFunction_16x"));
	}
	else {
		bool bIsPPure = GetIsPure();
		OutBrush = *FEditorStyle::GetBrush(bIsPPure ? TEXT("GraphEditor.PureFunction_16x") : TEXT("GraphEditor.Function_16x"));
	}
	OutBrush.TintColor = tint;
}

#pragma endregion


#pragma region ptrGetters

UFunction* UElgBESGraphFunction::GetFunction()
{
	return FElgKEWUtils::FindFunctionInBlueprint(GetBlueprint(), GraphPtr->GetFName());
}


UBlueprint* UElgBESGraphFunction::GetBlueprint()
{
	return Cast<UBlueprint>(GraphPtr->GetOuter());
}


UEdGraph* UElgBESGraphFunction::GetGraph()
{
	return GraphPtr;
}


UK2Node_FunctionEntry* UElgBESGraphFunction::GetFunctionEntryNode()
{
	return EntryNodePtr;
}


UK2Node_FunctionResult* UElgBESGraphFunction::GetFunctionResultNode()
{
	return ResultNodePtr;
}

#pragma endregion


void UElgBESGraphFunction::Setup(UEdGraph* InGraph)
{
	GraphPtr = InGraph;
	Name = InGraph->GetFName();
	BaseName = Name;
	Guid = InGraph->GraphGuid;
	EntryNodePtr = FElgKEWUtils::GetFunctionEntryNodeFromGraph(GraphPtr);
	ResultNodePtr = FElgKEWUtils::GetFunctionResultNodeFromGraph(GraphPtr);
}


bool UElgBESGraphFunction::ValidateFunction()
{
	if (GraphPtr && Guid.IsValid()) return true;
	UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("Function are no longer valid!"));
	return false;
}


UElgBESGraphFunction* UElgBESGraphFunction::MakeGraphFunctionObject(UEdGraph* InGraph)
{
	UElgBESGraphFunction* bpFunc = NewObject<UElgBESGraphFunction>();
	bpFunc->Setup(InGraph);
	return bpFunc;
}

#undef LOCTEXT_NAMESPACE