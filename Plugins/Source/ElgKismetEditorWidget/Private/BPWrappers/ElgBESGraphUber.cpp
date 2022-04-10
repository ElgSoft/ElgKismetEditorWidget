// Copyright 2019-2021 ElgSoft. All rights reserved. 


#include "BPWrappers/ElgBESGraphUber.h"
#include <ElgKEWEditorSubSystem.h>
#include <ElgEditorContext_BlueprintEditor.h>
#include <Kismet2/BlueprintEditorUtils.h>
#include <K2Node_CreateDelegate.h>
#include <Kismet2/KismetEditorUtilities.h>
#include <ElgKEWUtils.h>
#include <ElgBESGraphNode.h>
#include <EdGraph/EdGraphNode.h>
#include <K2Node_Event.h>
#include "ElgKEW_Log.h"


#define LOCTEXT_NAMESPACE "FElgKismetEditorWidgetModule"

void UElgBESGraphUber::Remove(const bool bShowDialog/* = false*/)
{
	if (!ValidateUber()) return;
	if (!GraphPtr->bAllowDeletion) {
		UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("Can't remove Uber graph [%s]"), *GraphPtr->GetFName().ToString());
		return;
	}

	if (bShowDialog && IsInUse()) {
		EAppReturnType::Type DialogReturn = FMessageDialog::Open(EAppMsgType::YesNo, NSLOCTEXT("UnrealEd", "Graph", "The UberGraph has nodes in it, are you sure you want remove it?"));
		if (DialogReturn == EAppReturnType::No) return;
	}


	const FScopedTransaction Transaction(LOCTEXT("RemoveGraph", "Remove Uber Graph"));
	BlueprintPtr->Modify();
	GraphPtr->Modify();

	UElgKEWEditorSubSystem* kewSS = GEditor->GetEditorSubsystem<UElgKEWEditorSubSystem>();
	if (UElgEditorContext_BlueprintEditor* context = kewSS->FindEditorContext(BlueprintPtr)) {
		context->CloseTab(GraphPtr);
	}

	FBlueprintEditorUtils::RemoveGraph(BlueprintPtr, GraphPtr, EGraphRemoveFlags::Recompile);
	for (TObjectIterator<UK2Node_CreateDelegate> It(RF_ClassDefaultObject, /** bIncludeDerivedClasses */ true, /** InternalExcludeFlags */ EInternalObjectFlags::Garbage); It; ++It)
	{
		if (IsValid(*It) && It->GetGraph() && IsValid(It->GetGraph()) && It->GetGraph() != GraphPtr) {
			It->HandleAnyChange();
		}
	}

	GraphPtr = NULL;
	BaseName = NAME_None;
}


void UElgBESGraphUber::Open()
{
	if (!ValidateUber()) return;
	//FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(GraphPtr, false);
	UElgKEWEditorSubSystem* kewSS = GEditor->GetEditorSubsystem<UElgKEWEditorSubSystem>();
	if (UElgEditorContext_BlueprintEditor* context = kewSS->FindEditorContext(BlueprintPtr)) {
		context->BlueprintEditorPtr.Pin()->OpenDocument(GraphPtr, FDocumentTracker::OpenNewDocument);
	}
}


void UElgBESGraphUber::SetName(FName InName)
{
	if (!ValidateUber()) return;
	if (!GraphPtr->bAllowDeletion) {
		UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("Can't rename Uber graph [%s]"), *GraphPtr->GetFName().ToString());
		return;
	}

	FString message;
	if (!FElgKEWUtils::NameValidator(BlueprintPtr, NAME_None, InName, message)) {
		UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("Can't rename Uber graph [%s]"), *GraphPtr->GetFName().ToString());
		return;
	}

	const FScopedTransaction Transaction(LOCTEXT("RenameGraph", "Rename Uber Graph"));
	FBlueprintEditorUtils::RenameGraph(GraphPtr, InName.ToString());
}


UEdGraph* UElgBESGraphUber::GetGraph()
{
	if (!ValidateUber()) return nullptr;
	return GraphPtr;
}


TArray<UElgBESGraphNode*> UElgBESGraphUber::GetNodes()
{
	TArray<UElgBESGraphNode*> nodes;
	if (!ValidateUber()) return nodes;
	for (UEdGraphNode* node : GraphPtr->Nodes) {
		nodes.Add(UElgBESGraphNode::MakeGraphNodeObject(node));
	}
	return nodes;
}


TArray<UElgBESGraphNode*> UElgBESGraphUber::GetEventNodes()
{
	TArray<UElgBESGraphNode*> nodes;
	if (!ValidateUber()) return nodes;
	for (UEdGraphNode* node : GraphPtr->Nodes) {
		if (node->IsA<UK2Node_Event>()) {
			nodes.Add(UElgBESGraphNode::MakeGraphNodeObject(node));
		}
	}
	return nodes;

}


bool UElgBESGraphUber::DoAllowDeletion()
{
	if (!ValidateUber()) return false;
	return GraphPtr->bAllowDeletion;
}


void UElgBESGraphUber::ShowInDetailsInternal()
{
	if (!ValidateUber()) return;
	UElgKEWEditorSubSystem* kewSS = GEditor->GetEditorSubsystem<UElgKEWEditorSubSystem>();
	if (UElgEditorContext_BlueprintEditor* context = kewSS->FindEditorContext(BlueprintPtr)) {
		context->ShowGraphInInspector(GraphPtr);
	}
}


void UElgBESGraphUber::SelectInMyBlueprintInternal()
{
	if (!ValidateUber()) return;
	UElgKEWEditorSubSystem* kewSS = GEditor->GetEditorSubsystem<UElgKEWEditorSubSystem>();
	if (UElgEditorContext_BlueprintEditor* context = kewSS->FindEditorContext(BlueprintPtr)) {
		context->ClearSelectionAll();
		context->SelectItemInMyBlueprint(GraphPtr->GetFName());
	}
}


FName UElgBESGraphUber::GetNameInternal()
{
	if (!ValidateUber()) return NAME_None;
	return GraphPtr->GetFName();
}


void UElgBESGraphUber::GetIconInternal(struct FSlateBrush& OutBrush)
{
	FLinearColor tint = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	OutBrush = *FEditorStyle::GetBrush(TEXT("GraphEditor.EventGraph_16x"));
	OutBrush.TintColor = tint;
}


int32 UElgBESGraphUber::GetCountInternal()
{
	if (!ValidateUber()) return 0;
	return GraphPtr->Nodes.Num();
}


void UElgBESGraphUber::Setup(UEdGraph* InGraph)
{
	GraphPtr = InGraph;
	GraphName = InGraph->GetFName().ToString();
	BlueprintPtr = Cast<UBlueprint>(GraphPtr->GetOuter());
}


bool UElgBESGraphUber::ValidateUber()
{
	if (GraphPtr) return true;
	UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("UberGraph are no longer valid!"));
	return false;
}


UElgBESGraphUber* UElgBESGraphUber::MakeGraphUberObject(UEdGraph* InGraph)
{
	UElgBESGraphUber* bpUber = NewObject<UElgBESGraphUber>();
	bpUber->Setup(InGraph);
	return bpUber;
}

#undef LOCTEXT_NAMESPACE