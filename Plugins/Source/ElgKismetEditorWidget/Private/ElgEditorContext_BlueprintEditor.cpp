// Copyright 2019-2021 ElgSoft. All rights reserved. 


#include "ElgEditorContext_BlueprintEditor.h"

#include <ElgKEWUtils.h>

#include <Engine/Blueprint.h>
#include <EdGraph/EdGraphNode.h>
#include <Logging/TokenizedMessage.h>
#include <BlueprintEditor.h>

#include <Widgets/SWindow.h>
#include <Framework/Application/SlateApplication.h>
#include <Layout/WidgetPath.h>
#include <SGraphPanel.h>
#include <Kismet2/BlueprintEditorUtils.h>
#include <Blueprint/WidgetLayoutLibrary.h>
#include <Input/Events.h>
#include <SKismetInspector.h>
#include <K2Node_Variable.h>
#include <ElgKEWDragDrop.h>
#include <ElgBESGraphNode.h>
#include <ElgBESGraphFunction.h>
#include <ObjectEditorUtils.h>
#include <Kismet2/KismetEditorUtilities.h>
#include <K2Node_Event.h>
#include <EdGraphSchema_K2_Actions.h>
#include <K2Node_CallFunction.h>
#include <ElgBESGraphVariableLocal.h>
#include <ElgBESGraphVariable.h>
#include <ElgBESGraphMacro.h>
#include <ElgBESGraphEventDispatcher.h>
#include <ElgBESGraphInterface.h>
#include <ElgBESGraphInterfaceFunction.h>
#include <AssetRegistry/AssetRegistryModule.h>
#include <ElgBESGraphWidgetVariable.h>
#include <Components/Widget.h>
#include <WidgetBlueprint.h>
#include <ElgBESGraphVariableInherited.h>
#include <Blueprint/WidgetTree.h>
#include <ElgBESGraphWidget.h>
#include <Components/NativeWidgetHost.h>
#include <KismetWidgets/Public/SPinTypeSelector.h>
#include <EdGraphSchema_K2.h>
#include <Widgets/SWidget.h>
#include <SGraphNode.h>
#include <K2Node_VariableGet.h>
#include <ElgBESGraphUber.h>
#include "ElgKEW_Log.h"


#define LOCTEXT_NAMESPACE "FElgKismetEditorWidgetModule"

#pragma region Setup

UElgEditorContext_BlueprintEditor::~UElgEditorContext_BlueprintEditor()
{
	FTSTicker::GetCoreTicker().RemoveTicker(myTickDelegateHandle);

	if (BlueprintEditorPtr.IsValid()) {
		BlueprintEditorPtr.Pin()->OnRefresh().Remove(EditorRefreshDelegate);
	}

	// remove any delegates
	OnNodeSelectionChanged.Clear();
	OnGraphCompiled.Clear();
	OnGraphChanged.Clear();
	OnFocusGraphChanged.Clear();

	for (auto& mapData : GraphChangedDelegatesMap) {
		if (IsValid(mapData.Key)) {
			mapData.Key->RemoveOnGraphChangedHandler(mapData.Value);
		}
	}
	GraphChangedDelegatesMap.Empty();

	SelectedNodeCount = 0;
	SelectedNodeSet.Reset();
	BlueprintEditorPtr = nullptr;
	BlueprintPtr = nullptr;

}

void UElgEditorContext_BlueprintEditor::Setup(TWeakPtr<class FBlueprintEditor> InEditor)
{
	BlueprintEditorPtr = InEditor;
	BlueprintPtr = InEditor.Pin()->GetBlueprintObj();
	EditorRefreshDelegate = BlueprintEditorPtr.Pin()->OnRefresh().AddUObject(this, &UElgEditorContext_BlueprintEditor::HandleOnEditorRefresh);

	bOnGraphChanged = false;

	// add delegates for its stuff.
	BlueprintPtr->OnCompiled().AddUObject(this, &UElgEditorContext_BlueprintEditor::HandleOnCompiled);
	BlueprintPtr->OnChanged().AddUObject(this, &UElgEditorContext_BlueprintEditor::HandleOnChanged);

	for (UEdGraph* graph : FElgKEWUtils::GetAllGraphs(BlueprintPtr)) {
		FDelegateHandle graphChangeDelegate = graph->AddOnGraphChangedHandler(
			FOnGraphChanged::FDelegate::CreateUObject(this, &UElgEditorContext_BlueprintEditor::HandleOnGraphChanged)
		);
		GraphChangedDelegatesMap.Add(graph, graphChangeDelegate);
		// graph->AddPropertyChangedNotifier // don't seem to work...
	}

	SetupTicker();
}


void UElgEditorContext_BlueprintEditor::Cleanup()
{
	bOnGraphChanged = false;
	FTSTicker::GetCoreTicker().RemoveTicker(myTickDelegateHandle);

	if (BlueprintEditorPtr.IsValid()) {
		BlueprintEditorPtr.Pin()->OnRefresh().Remove(EditorRefreshDelegate);
	}
	// remove any delegates
	OnNodeSelectionChanged.Clear();
	OnGraphCompiled.Clear();
	OnGraphChanged.Clear();
	OnFocusGraphChanged.Clear();

	if (IsValid(BlueprintPtr) && BlueprintPtr->GetFName().IsValid()) {
		BlueprintPtr->OnCompiled().RemoveAll(this);
		BlueprintPtr->OnChanged().RemoveAll(this);
	}

	for (auto& mapData : GraphChangedDelegatesMap) {
		if (IsValid(mapData.Key)) {
			mapData.Key->RemoveOnGraphChangedHandler(mapData.Value);
		}
	}
	GraphChangedDelegatesMap.Empty();

	SelectedNodeCount = 0;
	SelectedNodeSet.Reset();
	BlueprintEditorPtr = nullptr;
	BlueprintPtr = nullptr;
	CurrentGraphTab = nullptr;
}

void UElgEditorContext_BlueprintEditor::UpdateBlueprintEditor(TWeakPtr<class FBlueprintEditor> InEditor)
{
	if (!BlueprintEditorPtr.IsValid()) {
		Setup(InEditor);
	}
}

#pragma endregion


#pragma region Ticker

void UElgEditorContext_BlueprintEditor::SetupTicker()
{
	if (myTickDelegateHandle.IsValid()) FTSTicker::GetCoreTicker().RemoveTicker(myTickDelegateHandle);
	myTickDelegate =  FTickerDelegate::CreateUObject(this, &UElgEditorContext_BlueprintEditor::MyTick);
	myTickDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(myTickDelegate, myTickDelay);
}

bool UElgEditorContext_BlueprintEditor::MyTick(float DeltaSeconds)
{
	if (!BlueprintEditorPtr.IsValid()) {
		Cleanup();
		return true;
	}
	if (!IsValid(BlueprintPtr)) {
		Cleanup();
		return true;
	}

	if (KismetInspectorPropertyWrapper) {
		TSharedRef<SKismetInspector> inspector = BlueprintEditorPtr.Pin()->GetInspector();
		if (!inspector->IsSelected(KismetInspectorPropertyWrapper)) {
			KismetInspectorPropertyWrapper = nullptr;
		}
	}

	if (CurrentGraphTab && CurrentGraphTab != GetFocusGraph()) {
		CurrentGraphTab = GetFocusGraph();
		if (CurrentGraphTab) {  //
			HandleOnFocusGraphChanged();
		}
	} else if (CurrentGraphTab == nullptr && GetFocusGraph()) {
		CurrentGraphTab = GetFocusGraph();
		HandleOnFocusGraphChanged();
	}


	TSet<UObject*> selectedNodes = BlueprintEditorPtr.Pin()->GetSelectedNodes();
	if (selectedNodes.Num() != SelectedNodeCount) {
		SelectedNodeCount = selectedNodes.Num();
		HandleOnNodeSelectionChanged();
	}
	else if (SelectedNodeCount) {
		TSet<UObject*> nodeDiff = SelectedNodeSet.Difference(selectedNodes);
		int32 diffCount = nodeDiff.Num();
		if (diffCount) {
			HandleOnNodeSelectionChanged();
		}
	}
	SelectedNodeCount = selectedNodes.Num();
	SelectedNodeSet = selectedNodes;

	if (bOnGraphChanged) {
		//UE_LOG(LogElgKismetEditorWidget, Log, TEXT("OnGraphChanged.Broadcast()"));
		OnGraphChanged.Broadcast();
		bOnGraphChanged = false;
	}

	return true;
}

#pragma endregion


#pragma region Nodes


int32 UElgEditorContext_BlueprintEditor::NodeCount()
{
	return GetNodes().Num();
}


TArray<UElgBESGraphNode*> UElgEditorContext_BlueprintEditor::GetSelectedNodes()
{
	TArray<UElgBESGraphNode*> selectedGraphNodes;
	TSet<UObject*> selectedNodes = BlueprintEditorPtr.Pin()->GetSelectedNodes();
	if (selectedNodes.Num()) {
		for (FGraphPanelSelectionSet::TConstIterator It(selectedNodes); It; ++It) {
			if (UEdGraphNode* graphNode = Cast<UEdGraphNode>(*It)) {
				selectedGraphNodes.Add(UElgBESGraphNode::MakeGraphNodeObject(graphNode));
			}
		}
	}
	return selectedGraphNodes;
}


TArray<UElgBESGraphNode*> UElgEditorContext_BlueprintEditor::GetNodes()
{
	TArray<UElgBESGraphNode*> nodes;
	for (UEdGraph* graph : FElgKEWUtils::GetAllGraphs(BlueprintPtr)) {
		for (UEdGraphNode* node : graph->Nodes) {
			nodes.Add(UElgBESGraphNode::MakeGraphNodeObject(node));
		}
	}
	return nodes;
}


TArray<UElgBESGraphNode*> UElgEditorContext_BlueprintEditor::GetNodesInFunction(UElgBESGraphFunction* InFunction)
{	
	return GetNodesInGraph(InFunction->GraphPtr);
}


TArray<UElgBESGraphNode*> UElgEditorContext_BlueprintEditor::GetNodesInGraph(UEdGraph* InGraph)
{
	TArray<UElgBESGraphNode*> nodes;
	if (InGraph) {
		for (UEdGraphNode* node : InGraph->Nodes) {
			nodes.Add(UElgBESGraphNode::MakeGraphNodeObject(node));
		}
	}
	return nodes;
}


TArray<UElgBESGraphNode*> UElgEditorContext_BlueprintEditor::GetNodesCurrentGraph()
{	
	return GetNodesInGraph(GetFocusGraph());
}


bool UElgEditorContext_BlueprintEditor::GetNodeByGuid(FGuid InGuid, UElgBESGraphNode*& OutNode)
{
	for (UEdGraph* graph : FElgKEWUtils::GetAllGraphs(BlueprintPtr)) {
		for (UEdGraphNode* node : graph->Nodes) {
			if (node->NodeGuid == InGuid) {
				OutNode = UElgBESGraphNode::MakeGraphNodeObject(node);
				return true;
			}
		}
	}
	OutNode = nullptr;
	return false;

}

void UElgEditorContext_BlueprintEditor::AddNodeToSelection(UElgBESGraphNode* InNode)
{
	if (!InNode) return;
	if (BlueprintEditorPtr.IsValid()) {
		BlueprintEditorPtr.Pin()->AddToSelection(InNode->NodePtr);
	}
}


void UElgEditorContext_BlueprintEditor::AddNodesToSelection(TArray<UElgBESGraphNode*> InNodes)
{	
	if (BlueprintEditorPtr.IsValid()) {
		TSharedPtr<FBlueprintEditor> bpEditor = BlueprintEditorPtr.Pin();
		for (UElgBESGraphNode* node : InNodes) {
			bpEditor->AddToSelection(node->NodePtr);
		}		
	}
}

#pragma endregion


#pragma region Messages


void UElgEditorContext_BlueprintEditor::LogSimpleMessage(const FText InMessage)
{
	if (BlueprintEditorPtr.IsValid()) {
		BlueprintEditorPtr.Pin()->LogSimpleMessage(InMessage);
	}
}



void UElgEditorContext_BlueprintEditor::LogMessageToCompilerLog(const FText InMessage, EBPElgKEWMessageSeverity InSeverity, bool bForceMessageDisplay /*= true*/)
{
	if (BlueprintEditorPtr.IsValid()) {
		TArray<TSharedRef<FTokenizedMessage>> Messages;
		EMessageSeverity::Type severity;
		switch (InSeverity) {
		case EBPElgKEWMessageSeverity::CriticalError:		severity = EMessageSeverity::CriticalError; break;
		case EBPElgKEWMessageSeverity::Error:				severity = EMessageSeverity::Error; break;
		case EBPElgKEWMessageSeverity::Info:				severity = EMessageSeverity::Info; break;
		case EBPElgKEWMessageSeverity::PerformanceWarning:	severity = EMessageSeverity::PerformanceWarning; break;
		case EBPElgKEWMessageSeverity::Warning:				severity = EMessageSeverity::Warning; break;
		default: severity = EMessageSeverity::Info;
		}
		Messages.Add(FTokenizedMessage::Create(severity, InMessage));
		BlueprintEditorPtr.Pin()->DumpMessagesToCompilerLog(Messages, bForceMessageDisplay);
	}
}

#pragma endregion


#pragma region Misc

void UElgEditorContext_BlueprintEditor::JumpToObject(UObject* InObjectToJumpTo)
{
	if (BlueprintEditorPtr.IsValid()) {
		BlueprintEditorPtr.Pin()->JumpToHyperlink(InObjectToJumpTo, false);
	}
}

void UElgEditorContext_BlueprintEditor::CloseTab(UObject* InTabToClose)
{
	if (BlueprintEditorPtr.IsValid()) {
		BlueprintEditorPtr.Pin()->CloseDocumentTab(InTabToClose);
	}
}


UBlueprint* UElgEditorContext_BlueprintEditor::GetBlueprint()
{
	return BlueprintPtr;
}


UEdGraph* UElgEditorContext_BlueprintEditor::GetFocusGraph()
{
	if (BlueprintEditorPtr.IsValid()) {
		return BlueprintEditorPtr.Pin()->GetFocusedGraph();
	}
	return nullptr;
}




void UElgEditorContext_BlueprintEditor::SummonSearchUI(const FString InSearchTerms /*= FString()*/, const bool bInSelectFirstResult /*= false*/)
{
	if (BlueprintEditorPtr.IsValid()) {
		return BlueprintEditorPtr.Pin()->SummonSearchUI(true, InSearchTerms, bInSelectFirstResult);
	}
}


void UElgEditorContext_BlueprintEditor::SummonFindAndReplaceUI()
{
	TSharedPtr<FBlueprintEditor> PinnedEditor = BlueprintEditorPtr.Pin();
	if (PinnedEditor.IsValid()) {
		PinnedEditor->SummonFindAndReplaceUI();
	}
}

bool UElgEditorContext_BlueprintEditor::InDebuggingMode()
{
	return FSlateApplication::Get().InKismetDebuggingMode();
}

bool UElgEditorContext_BlueprintEditor::InEditingMode()
{
	if (BlueprintEditorPtr.IsValid()) {
		return BlueprintEditorPtr.Pin()->InEditingMode();
	}
	return false;
}

bool UElgEditorContext_BlueprintEditor::IsCompilingEnabled()
{
	if (BlueprintEditorPtr.IsValid()) {
		return BlueprintEditorPtr.Pin()->IsCompilingEnabled();
	}
	return false;
}


void UElgEditorContext_BlueprintEditor::GetViewLocation(FVector2D& OutLocation, float& OutZoomAmount)
{
	if (BlueprintEditorPtr.IsValid()) {
		return BlueprintEditorPtr.Pin()->GetViewLocation(OutLocation, OutZoomAmount);
	}
}

void UElgEditorContext_BlueprintEditor::SetViewLocation(const FVector2D InLocation, float InZoomAmount)
{
	if (BlueprintEditorPtr.IsValid()) {
		return BlueprintEditorPtr.Pin()->SetViewLocation(InLocation, InZoomAmount);
	}
}


bool UElgEditorContext_BlueprintEditor::IsParentClassABlueprint()
{
	if (BlueprintEditorPtr.IsValid()) {
		return BlueprintEditorPtr.Pin()->IsParentClassABlueprint();
	}
	return false;
}

bool UElgEditorContext_BlueprintEditor::IsParentClassNative()
{
	if (BlueprintEditorPtr.IsValid()) {
		return BlueprintEditorPtr.Pin()->IsParentClassNative();	
	}
	return false;
}


FText UElgEditorContext_BlueprintEditor::GetPIEStatus()
{
	if (BlueprintEditorPtr.IsValid()) {
		return BlueprintEditorPtr.Pin()->GetPIEStatus();
	}
	return FText();
}


void UElgEditorContext_BlueprintEditor::StartEditingDefaults(bool bInAutoFocus /*= true*/, bool bInForceRefresh /*= false*/)
{
	if (BlueprintEditorPtr.IsValid()) {
		BlueprintEditorPtr.Pin()->StartEditingDefaults(bInAutoFocus, bInForceRefresh);
	}
}


void UElgEditorContext_BlueprintEditor::GetUISelectionState(FName& OutCurrentState)
{
	if (BlueprintEditorPtr.IsValid()) {
		OutCurrentState = BlueprintEditorPtr.Pin()->GetUISelectionState();
	}
}

void UElgEditorContext_BlueprintEditor::SetUISelectionState(const FName InSelectionOwner)
{
	if (BlueprintEditorPtr.IsValid()) {
		BlueprintEditorPtr.Pin()->SetUISelectionState(InSelectionOwner);
		BlueprintEditorPtr.Pin()->SetCurrentMode(InSelectionOwner);
	}
}

// #UE5: #Remove NativeCodeGeneration has been removed.
bool UElgEditorContext_BlueprintEditor::CanGenerateNativeCode()
{
	UE_LOG(LogElgKismetEditorWidget, Error, TEXT("NativeCodeGeneration has been removed in UE5."));
	return false;
}

// #UE5: #Remove NativeCodeGeneration has been removed.
void UElgEditorContext_BlueprintEditor::OpenNativeCodeGenerationTool()
{
	UE_LOG(LogElgKismetEditorWidget, Error, TEXT("NativeCodeGeneration has been removed in UE5."));
}


void UElgEditorContext_BlueprintEditor::ToggleHideUnrelatedNodes()
{
	if (BlueprintEditorPtr.IsValid()) {
		BlueprintEditorPtr.Pin()->ToggleHideUnrelatedNodes();
	}
}


bool UElgEditorContext_BlueprintEditor::IsToggleHideUnrelatedNodesChecked()
{
	if (BlueprintEditorPtr.IsValid()) {
		return BlueprintEditorPtr.Pin()->IsToggleHideUnrelatedNodesChecked();
	}
	return false;
}

// #UE5 #Remove Remove next update
bool UElgEditorContext_BlueprintEditor::ShouldShowToggleHideUnrelatedNodes(bool bInIsToolbar)
{
	return false;
}


void UElgEditorContext_BlueprintEditor::GetCategoryTexts(bool bShowUserVarsOnly, TArray<FText>& OutCategory)
{
	OutCategory = FElgKEWUtils::GetCategoryTexts(BlueprintPtr, bShowUserVarsOnly);
}


void UElgEditorContext_BlueprintEditor::GetCategoryTextsAsString(bool bShowUserVarsOnly, TArray<FString>& OutCategory)
{
	OutCategory = FElgKEWUtils::GetCategoryTextsAsString(BlueprintPtr, bShowUserVarsOnly);
}

void UElgEditorContext_BlueprintEditor::HandleDragAndDrop(const FPointerEvent& PointerEvent, UDragDropOperation* Operation)
{
	if (!Operation) return;

	UElgKEWDragDropBase* dragDrop = Cast<UElgKEWDragDropBase>(Operation);
	if (!dragDrop) return;

	bool bInGraphPanel = false;
	FVector2D nodePosition;
	UK2Node_Variable* droppedOnVarNode = nullptr;
	UEdGraphPin* PinUnderCursor = nullptr;

	// check if the drop was on a Variable node.
	TArray<TSharedRef<SWidget>> widgets = FElgKEWUtils::GetSlateWidgetsUnderMouse();
	//UE_LOG(LogElgKismetEditorWidget, Log, TEXT("Widgets under mouse"));
	for (TSharedRef<SWidget> widgetRef : widgets) {
		FName widgetTypeName = widgetRef.Get().GetType();
		//UE_LOG(LogElgKismetEditorWidget, Log, TEXT("		[%s]"), *widgetRef.Get().GetType().ToString());
		if (widgetTypeName == FName("SGraphNodeK2Var")) {
			TSharedRef<SGraphNode> widgetVar = StaticCastSharedRef<SGraphNode>(widgetRef);
			const TSharedPtr<SGraphPin> HoveredPin = widgetVar->GetHoveredPin(widgetVar->GetCachedGeometry(), PointerEvent);
			if (HoveredPin.IsValid()) {
				PinUnderCursor = HoveredPin->GetPinObj();
				if (PinUnderCursor->bOrphanedPin) { // we do not want orphaned pins
					PinUnderCursor = nullptr;
				} else {
					//UE_LOG(LogElgKismetEditorWidget, Log, TEXT("Dropped on pin!"));
				}
			}
			//UE_LOG(LogElgKismetEditorWidget, Log, TEXT("Dropped on variable!"));
			droppedOnVarNode = Cast<UK2Node_Variable>(widgetVar->GetNodeObj());
		} 
		else if (widgetTypeName.ToString().StartsWith("SGraphNode")) 
		{
			TSharedRef<SGraphNode> widgetNode = StaticCastSharedRef<SGraphNode>(widgetRef);
			const TSharedPtr<SGraphPin> HoveredPin = widgetNode->GetHoveredPin(widgetNode->GetCachedGeometry(), PointerEvent);
			if (HoveredPin.IsValid()) {
				PinUnderCursor = HoveredPin->GetPinObj();
				if (PinUnderCursor->bOrphanedPin) { // we do not want orphaned pins
					PinUnderCursor = nullptr;
				} else {
					//UE_LOG(LogElgKismetEditorWidget, Log, TEXT("Dropped on pin!"));
				}					
			}
		} 
		else if (widgetTypeName == FName("SGraphPanel")) {
			TSharedPtr<SWidget> graphWidget = widgetRef;
			TSharedPtr<SGraphPanel> widgetPanel = StaticCastSharedPtr<SGraphPanel>(graphWidget);
			nodePosition = FElgKEWUtils::GetMousePositionInGraph(widgetPanel);
			bInGraphPanel = true;
			break;
		}
	}

	if (!bInGraphPanel) return;
	
	FName dragDropName = dragDrop->NameOfDragDropObject;

	if (dragDrop->IsA<UElgKEWDragDropVariable>())
	{
		if (droppedOnVarNode && !PinUnderCursor) {
			FElgKEWUtils::ChangeVariableNodeReferenceToMember(BlueprintPtr, droppedOnVarNode, dragDropName);
		} else {			
			bool isSetter = !PointerEvent.IsAltDown();
			FElgKEWUtils::SpawnVariableNodeInGraph(BlueprintPtr, GetFocusGraph(), dragDropName, nodePosition, isSetter, PinUnderCursor);
		}
	}
	else if (dragDrop->IsA<UElgKEWDragDropLocalVariable>())
	{
		if (droppedOnVarNode && !PinUnderCursor) {
			UFunction* func = FElgKEWUtils::FindFunctionInBlueprint(BlueprintPtr, GetFocusGraph()->GetFName());
			FElgKEWUtils::ChangeVariableNodeReferenceToLocal(BlueprintPtr, droppedOnVarNode, func, dragDropName);
		} else {
			bool isSetter = !PointerEvent.IsAltDown();
			FElgKEWUtils::SpawnLocalVariableNodeInGraph(BlueprintPtr, GetFocusGraph(), dragDropName, nodePosition, isSetter, PinUnderCursor);
		}
	}
	else if (dragDrop->IsA<UElgKEWDragDropVariablePin>())
	{
		if (droppedOnVarNode && !PinUnderCursor) {
			// we can only update GetNodes as you can not set input pin data.
			if (UK2Node_VariableGet* getVarNnode = Cast<UK2Node_VariableGet>(droppedOnVarNode)) {
				FElgKEWUtils::ChangeVariableNodeReferenceToPinVar(BlueprintPtr, GetFocusGraph(), getVarNnode, dragDropName);
			}			
		} else {			
			FElgKEWUtils::SpawnPinVariableNodeInGraph(BlueprintPtr, GetFocusGraph(), dragDropName, nodePosition, PinUnderCursor);
		}		
	}
	else if (dragDrop->IsA<UElgKEWDragDropFunction>())
	{
		FElgKEWUtils::SpawnFunctionNodeInGraph(BlueprintPtr, GetFocusGraph(), dragDropName, nodePosition, PinUnderCursor);
	}
	else if (dragDrop->IsA<UElgKEWDragDropMacro>())
	{
		FElgKEWUtils::SpawnMacroNodeInGraph(BlueprintPtr, GetFocusGraph(), dragDropName, nodePosition, PinUnderCursor);
	}
	else if (dragDrop->IsA<UElgKEWDragDropEventDispatcher>())
	{
		bool isSetter = !PointerEvent.IsAltDown();
		FElgKEWUtils::SpawnEventDispatcherNodeInGraph(BlueprintPtr, GetFocusGraph(), dragDropName, nodePosition, isSetter, PinUnderCursor);
	}
	else if (dragDrop->IsA<UElgKEWDragDropInterfaceFunction>())
	{
		FElgKEWUtils::SpawnFunctionNodeInGraph(BlueprintPtr, GetFocusGraph(), dragDropName, nodePosition, PinUnderCursor);
	}
	else if (dragDrop->IsA<UElgKEWDragDropWidgetVariable>())
	{
		if (droppedOnVarNode && !PinUnderCursor) {
			if (UK2Node_VariableGet* getVarNnode = Cast<UK2Node_VariableGet>(droppedOnVarNode)) {
				FElgKEWUtils::ChangeVariableNodeReferenceToMember(BlueprintPtr, droppedOnVarNode, dragDropName);
			}
		} else {
			// we can not set the Widget variable data so skip it if thats the case.
			if (PinUnderCursor && PinUnderCursor->Direction == EGPD_Output) return;
			FElgKEWUtils::SpawnVariableNodeInGraph(BlueprintPtr, GetFocusGraph(), dragDropName, nodePosition, true, PinUnderCursor);
		}
	}
}

TMap<FName, int32> UElgEditorContext_BlueprintEditor::GetVariableUsageMap()
{
	TMap<FName, int32> outMap;
	for (UEdGraph* graph : FElgKEWUtils::GetAllGraphs(BlueprintPtr)) {
		for (UEdGraphNode* node : graph->Nodes) {
			if (UK2Node_Variable* varNode = Cast<UK2Node_Variable>(node)) {
				FName name = varNode->GetPropertyForVariable()->GetFName();
				int32& count = outMap.FindOrAdd(name);
				count++;
			}
		}
	}
	return outMap;
}


bool UElgEditorContext_BlueprintEditor::IsNameValid(const FName InName, EBPElgKEWWResult& Branch)
{
	FString message;
	if (FElgKEWUtils::NameValidator(BlueprintPtr, NAME_None, InName, message)) {
		Branch = EBPElgKEWWResult::outTrue;
		return true;
	}
	//UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("NameValidation falied: %s"), *message);
	Branch = EBPElgKEWWResult::outFalse;
	return false;
}

void UElgEditorContext_BlueprintEditor::GetUniqueName(const FName InName, FName& OutName)
{
	OutName = FBlueprintEditorUtils::FindUniqueKismetName(BlueprintPtr, InName.ToString());
}

#pragma endregion 


#pragma region ClearSelection

void UElgEditorContext_BlueprintEditor::ClearSelectionGraph()
{
	if (BlueprintEditorPtr.IsValid()) {
		BlueprintEditorPtr.Pin()->ClearSelectionStateFor(TEXT("Graph"));
	}
}

void UElgEditorContext_BlueprintEditor::ClearSelectionMyBlueprint()
{
	if (BlueprintEditorPtr.IsValid()) {
		BlueprintEditorPtr.Pin()->ClearSelectionStateFor(TEXT("MyBlueprint"));
	}	
}

void UElgEditorContext_BlueprintEditor::ClearSelectionComponents()
{
	if (BlueprintEditorPtr.IsValid()) {
		BlueprintEditorPtr.Pin()->ClearSelectionStateFor(TEXT("Components"));
	}
}


void UElgEditorContext_BlueprintEditor::ClearSelectionAll()
{
	ClearSelectionGraph();
	ClearSelectionMyBlueprint();
	ClearSelectionComponents();
}

#pragma endregion 


#pragma region KismetInspector

void UElgEditorContext_BlueprintEditor::ShowPropertyInInspector(FProperty* InProperty)
{	
	TSharedRef<SKismetInspector> inspector = BlueprintEditorPtr.Pin()->GetInspector();
	if (InProperty) {
		KismetInspectorPropertyName = InProperty->GetFName();
		ClearSelectionAll();
		SKismetInspector::FShowDetailsOptions Options(FText::FromName(InProperty->GetFName()));
		Options.bForceRefresh = true;		
		KismetInspectorPropertyWrapper = InProperty->GetUPropertyWrapper();
		inspector->ShowDetailsForSingleObject(KismetInspectorPropertyWrapper, Options);
		BlueprintEditorPtr.Pin()->SetUISelectionState(FBlueprintEditor::SelectionState_MyBlueprint);
	} else {
		// deselect anything selected
		inspector->ShowDetailsForSingleObject(nullptr);
		KismetInspectorPropertyWrapper = nullptr;
	}	
}


void UElgEditorContext_BlueprintEditor::ShowGraphInInspector(UEdGraph* InGraph)
{
	TSharedRef<SKismetInspector> inspector = BlueprintEditorPtr.Pin()->GetInspector();
	if (InGraph) {
		ClearSelectionAll();
		FGraphDisplayInfo DisplayInfo;
		InGraph->GetSchema()->GetGraphDisplayInformation(*InGraph, DisplayInfo);
		inspector->ShowDetailsForSingleObject(InGraph, SKismetInspector::FShowDetailsOptions(DisplayInfo.PlainName));
	} else {
		// deselect anything selected
		inspector->ShowDetailsForSingleObject(nullptr);
		KismetInspectorPropertyWrapper = nullptr;
	}
}


void UElgEditorContext_BlueprintEditor::ShowDelegateInInspector(UEdGraph* InGraph, UK2Node_EditablePinBase* InNode)
{
	TSharedRef<SKismetInspector> inspector = BlueprintEditorPtr.Pin()->GetInspector();
	if (InGraph) {
		ClearSelectionAll();
		FGraphDisplayInfo DisplayInfo;
		InGraph->GetSchema()->GetGraphDisplayInformation(*InGraph, DisplayInfo);
		inspector->ShowDetailsForSingleObject(InNode, SKismetInspector::FShowDetailsOptions(DisplayInfo.PlainName));
	}
	else {
		// deselect anything selected
		inspector->ShowDetailsForSingleObject(nullptr);
		KismetInspectorPropertyWrapper = nullptr;
	}
}

#pragma endregion 


#pragma region MyBlueprint

void UElgEditorContext_BlueprintEditor::SelectItemInMyBlueprint(FName InName)
{
	if (BlueprintEditorPtr.IsValid()) {
		BlueprintEditorPtr.Pin()->SelectGraphActionItemByName(InName, ESelectInfo::OnMouseClick);
	}
}

#pragma endregion 


#pragma region Function 

TArray<UElgBESGraphFunction*> UElgEditorContext_BlueprintEditor::GetFunctions()
{
	TArray<UElgBESGraphFunction*> functions;
	for (UEdGraph* graph : BlueprintPtr->FunctionGraphs) {
		functions.Add(UElgBESGraphFunction::MakeGraphFunctionObject(graph));
	}

	functions.Sort([&](const UElgBESGraphFunction& dataA, const UElgBESGraphFunction& dataB) {
		return dataA.Name.ToString() < dataB.Name.ToString();
		});

	return functions;
}



UElgBESGraphFunction* UElgEditorContext_BlueprintEditor::GetFunctionByGuid(FGuid InGuid, bool& OutResult)
{
	OutResult = false;
	if (!InGuid.IsValid()) {
		UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("GetFunctionByGuid: The InGuid is invalid!"));
		return nullptr;
	}

	for (UEdGraph* graph : BlueprintPtr->FunctionGraphs) {
		if (graph->GraphGuid == InGuid) {
			OutResult = true;
			return UElgBESGraphFunction::MakeGraphFunctionObject(graph);
		}
	}

	UE_LOG(LogElgKismetEditorWidget, Log, TEXT("GetFunctionByGuid: failed to find any function with Guid[%s]"), *InGuid.ToString());
	return nullptr;
}


UElgBESGraphFunction* UElgEditorContext_BlueprintEditor::GetFunctionByName(FName InName, bool& OutResult)
{
	OutResult = false;
	if (!InName.IsValid()) {
		UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("GetFunctionByName: The InName is invalid!"));
		return nullptr;
	}

	for (UEdGraph* graph : BlueprintPtr->FunctionGraphs) {
		if (graph->GetFName() == InName) {
			OutResult = true;
			return UElgBESGraphFunction::MakeGraphFunctionObject(graph);
		}
	}
	UE_LOG(LogElgKismetEditorWidget, Log, TEXT("GetFunctionByName: Failed to find any function with name [%s]"), *InName.ToString());
	return nullptr;
}


UElgBESGraphFunction* UElgEditorContext_BlueprintEditor::GetCurrentFunction(bool& OutResult)
{
	OutResult = false;
	if (UEdGraph* graph = GetFocusGraph()) {
		if (FElgKEWUtils::IsGraphAFunction(graph)) {
			return GetFunctionByName(graph->GetFName(), OutResult);
		}
	}
	return nullptr;		 
}


UElgBESGraphFunction* UElgEditorContext_BlueprintEditor::CreateNewFunction(FName InName, FText InCategory)
{	
	FString message;
	if (InName.IsNone()) {
		InName = FName("NewFunction");
	}
	if (!FElgKEWUtils::NameValidator(BlueprintPtr, NAME_None, InName, message)) {
		InName = FBlueprintEditorUtils::FindUniqueKismetName(BlueprintPtr, InName.ToString());
	}
	const FScopedTransaction Transaction(LOCTEXT("AddNewFunction", "Add New Function"));
	BlueprintPtr->Modify();
	UEdGraph* newGraph = FBlueprintEditorUtils::CreateNewGraph(BlueprintPtr, InName, UEdGraph::StaticClass(), UEdGraphSchema_K2::StaticClass());
	FBlueprintEditorUtils::AddFunctionGraph<UClass>(BlueprintPtr, newGraph, /*bIsUserCreated=*/ true, NULL);
	UElgBESGraphFunction* newFunc = UElgBESGraphFunction::MakeGraphFunctionObject(newGraph);
	if (InCategory.IsEmpty()) {
		InCategory = FText::FromString(TEXT("Default"));
	}
	newFunc->SetCategory(InCategory);
	return newFunc;
}


TArray<UElgBESGraphFunction*> UElgEditorContext_BlueprintEditor::GetOverrideFunctions()
{
	TArray<UElgBESGraphFunction*> functions;
	if (!GetBlueprint()->ParentClass) return functions;

	for (UEdGraph* graph : BlueprintPtr->FunctionGraphs) {
		if (FindUField<UFunction>(GetBlueprint()->ParentClass, graph->GetFName())) {
			functions.Add(UElgBESGraphFunction::MakeGraphFunctionObject(graph));
		}		
	}

	if (functions.Num()) {
		functions.Sort([&](const UElgBESGraphFunction& dataA, const UElgBESGraphFunction& dataB) {
			return dataA.Name.ToString() < dataB.Name.ToString();
			});
	}

	return functions;
}


void UElgEditorContext_BlueprintEditor::GetOverridableFunctions(TArray<FS_ElgFunctionInfo>& OutInfo)
{
	UClass* ParentClass = BlueprintPtr->SkeletonGeneratedClass ? BlueprintPtr->SkeletonGeneratedClass->GetSuperClass() : *BlueprintPtr->ParentClass;
	for (TFieldIterator<UFunction> FunctionIt(ParentClass, EFieldIteratorFlags::IncludeSuper); FunctionIt; ++FunctionIt) {
		const UFunction* Function = *FunctionIt;
		const FName FunctionName = Function->GetFName();
		if (UEdGraphSchema_K2::CanKismetOverrideFunction(Function)
			&& !FObjectEditorUtils::IsFunctionHiddenFromClass(Function, ParentClass)
			&& !FBlueprintEditorUtils::FindOverrideForFunction(BlueprintPtr, CastChecked<UClass>(Function->GetOuter()), Function->GetFName()))
		{
			OutInfo.Add(FElgKEWUtils::CreateFunctionInfo(Function));
		}
	}
	OutInfo.Sort([](const FS_ElgFunctionInfo& varA, const FS_ElgFunctionInfo& varB) {
		return varA.FunctionName.ToString() < varB.FunctionName.ToString();
		});
}


UElgBESGraphFunction* UElgEditorContext_BlueprintEditor::AddOverridableFunction(const FName InName)
{
	if (FElgKEWUtils::AddOverridableFunction(BlueprintPtr, InName)) {
		bool temp;
		return GetFunctionByName(InName, temp);
	}
	return nullptr;
}

#pragma endregion


#pragma region Macro

TArray<UElgBESGraphMacro*> UElgEditorContext_BlueprintEditor::GetMacros()
{
	TArray<UElgBESGraphMacro*> macros;
	for (int32 i = 0; i < BlueprintPtr->MacroGraphs.Num(); i++) {
		UEdGraph* graph = BlueprintPtr->MacroGraphs[i];
		macros.Add(UElgBESGraphMacro::MakeGraphMacroObject(graph));
	}
	return macros;
}

UElgBESGraphMacro* UElgEditorContext_BlueprintEditor::GetMacroByName(FName InName, bool& OutResult)
{
	TArray<UElgBESGraphMacro*> macros;
	for (int32 i = 0; i < BlueprintPtr->MacroGraphs.Num(); i++) {
		UEdGraph* graph = BlueprintPtr->MacroGraphs[i];
		if (graph->GetFName() == InName) {
			OutResult = true;
			return UElgBESGraphMacro::MakeGraphMacroObject(graph);
		}
	}
	OutResult = false;
	return nullptr;
}

UElgBESGraphMacro* UElgEditorContext_BlueprintEditor::CreateNewMacro(FName InName, FText InCategory)
{
	FString message;
	if (InName.IsNone()) {
		InName = FName("NewMacro");
	} 
	if (!FElgKEWUtils::NameValidator(BlueprintPtr, NAME_None, InName, message)) {
		InName = FBlueprintEditorUtils::FindUniqueKismetName(BlueprintPtr, InName.ToString());
	}

	const FScopedTransaction Transaction(LOCTEXT("AddNewMacro", "Add New Macro"));
	BlueprintPtr->Modify();
	UEdGraph* NewGraph = FBlueprintEditorUtils::CreateNewGraph(BlueprintPtr, InName, UEdGraph::StaticClass(), UEdGraphSchema_K2::StaticClass());
	FBlueprintEditorUtils::AddMacroGraph(BlueprintPtr, NewGraph, /*bIsUserCreated=*/ true, NULL);

	if (InCategory.IsEmpty()) {
		InCategory = FText::FromString(TEXT("Default"));
	}
	UElgBESGraphMacro* newMacro = UElgBESGraphMacro::MakeGraphMacroObject(NewGraph);
	newMacro->SetCategory(InCategory);
	return newMacro;
}

#pragma endregion


#pragma region Variables


bool UElgEditorContext_BlueprintEditor::CreateVariable(const FName InName, const FS_ElgGraphPinType InType, const FText InCategoryName, const FString InDefaultValue, UElgBESGraphVariable*& OutVariable)
{
	FEdGraphPinType NewVarType = FElgKEWUtils::ConvertFromPinType(InType);
	if (FBlueprintEditorUtils::AddMemberVariable(BlueprintPtr, InName, NewVarType, InDefaultValue)) {
		HandleOnChanged(BlueprintPtr);
		SelectItemInMyBlueprint(InName);
		GetVariableByName(InName, OutVariable); // TODO: This could fail to find the variable? or would it just fail to add it?
		OutVariable->SetVariableCategory(InCategoryName);
		return true;
	}
	return false;
}


void UElgEditorContext_BlueprintEditor::RemoveVariable(const FName InName)
{
	FElgKEWUtils::RemoveVariable(BlueprintPtr, InName);
}

TArray<UElgBESGraphVariable*> UElgEditorContext_BlueprintEditor::GetVariables()
{		
	TArray<UElgBESGraphVariable*> outVars;

	for (FProperty* prop : FElgKEWUtils::GetVariableProperties(BlueprintPtr, false, true)) {
		outVars.Add(UElgBESGraphVariable::MakeGraphVariableObject(BlueprintPtr, prop));
	}

	outVars.Sort([&](const UElgBESGraphVariable& dataA, const UElgBESGraphVariable& dataB) {
		return dataA.VariableName.ToString() < dataB.VariableName.ToString();
		});

	return outVars;
}


bool UElgEditorContext_BlueprintEditor::GetVariableByName(const FName InName, UElgBESGraphVariable*& OutVariable)
{
	for (FProperty* prop : FElgKEWUtils::GetVariableProperties(BlueprintPtr, false, true)) {
		if (prop->GetFName() == InName) {
			OutVariable = UElgBESGraphVariable::MakeGraphVariableObject(BlueprintPtr, prop);
			return true;
		}
	}
	return false;
}

bool UElgEditorContext_BlueprintEditor::GetVariableByGuid(const FGuid InGuid, UElgBESGraphVariable*& OutVariable)
{
	for (FBPVariableDescription var : BlueprintPtr->NewVariables) {
		if (var.VarGuid == InGuid) {
			OutVariable = UElgBESGraphVariable::MakeGraphVariableObject(BlueprintPtr, var);
			return true;
		}
	}
	return false;
}


TArray<UElgBESGraphVariableInherited*> UElgEditorContext_BlueprintEditor::GetInheritVariables()
{
	TArray<UElgBESGraphVariableInherited*> outVars;
	TSet<FName> localVars;
	// include any Widget and Component variables other wise they will also show up here
	for (FProperty* prop : FElgKEWUtils::GetVariableProperties(BlueprintPtr, true, true)) {
		localVars.Add(prop->GetFName());
	}

	for (TFieldIterator<FProperty> PropertyIt(BlueprintPtr->SkeletonGeneratedClass, EFieldIteratorFlags::IncludeSuper); PropertyIt; ++PropertyIt)
	{
		FProperty* Property = *PropertyIt;

		const bool bMulticastDelegateProp = Property->IsA(FMulticastDelegateProperty::StaticClass());
		const bool bDelegateProp = (Property->IsA(FDelegateProperty::StaticClass()) || bMulticastDelegateProp);
		const bool bShouldShowAsVar = (!Property->HasAnyPropertyFlags(CPF_Parm) && Property->HasAllPropertyFlags(CPF_BlueprintVisible)) && !bDelegateProp;
		if (!bShouldShowAsVar) continue;
		if (localVars.Contains(Property->GetFName())) continue;
		outVars.Add(UElgBESGraphVariableInherited::MakeGraphVariable(BlueprintPtr, Property));
	}

	outVars.Sort([&](const UElgBESGraphVariableInherited& dataA, const UElgBESGraphVariableInherited& dataB) {
		return dataA.VariableName.ToString() < dataB.VariableName.ToString();
		});

	return outVars;

}

#pragma endregion


#pragma region FunctionPinVariables

TArray<UElgBESGraphFunctionPin*> UElgEditorContext_BlueprintEditor::GetFunctionInputPinVariablesInGraph(UEdGraph* InGraph)
{
	TArray<UElgBESGraphFunctionPin*> outPins;
	if (InGraph) {
		if (FElgKEWUtils::IsGraphAFunction(InGraph)) {
			FName graphName = InGraph->GetFName();
			bool res;
			if (UElgBESGraphFunction* func = GetFunctionByName(graphName, res)) {
				return func->GetFunctionInputPinVariables();
			}

			// not a function so could be a interface function.
			for (UElgBESGraphInterface* impInterface : UElgBESGraphInterface::GetBlueprintInterfaces(BlueprintPtr, true)) {
				for (UElgBESGraphInterfaceFunction* inFunc : impInterface->GetInterfaceFunctions()) {
					if (inFunc->FunctionName == graphName) {
						return inFunc->GetFunctionInputPinVariables();
					}
				}
			}
		}
	}
	return outPins;



}


TArray<UElgBESGraphFunctionPin*> UElgEditorContext_BlueprintEditor::GetFunctionInputPinVariablesInFocusGraph()
{
	return GetFunctionInputPinVariablesInGraph(GetFocusGraph());
}

#pragma endregion


#pragma region WidgetVariables

TArray<UElgBESGraphWidgetVariable*> UElgEditorContext_BlueprintEditor::GetWidgetVariables()
{
	TArray<UElgBESGraphWidgetVariable*> outVars;
	// check if this is an widget blueprint graph
	UWidgetBlueprint* widgetBP = Cast<UWidgetBlueprint>(BlueprintPtr);
	if ( widgetBP == nullptr) return outVars;

	// get the name for all the widgets that exist in widget tree as its only those variables we are after.
	TArray<FName> widgetNames = FElgKEWUtils::GetWidgetTreeNames(BlueprintPtr);

	for (TFieldIterator<FProperty> PropertyIt(BlueprintPtr->SkeletonGeneratedClass, EFieldIteratorFlags::ExcludeSuper); PropertyIt; ++PropertyIt)
	{
		FProperty* Property = *PropertyIt;
		
		const bool bMulticastDelegateProp = Property->IsA(FMulticastDelegateProperty::StaticClass());
		const bool bDelegateProp = (Property->IsA(FDelegateProperty::StaticClass()) || bMulticastDelegateProp);
		const bool bShouldShowAsVar = (!Property->HasAnyPropertyFlags(CPF_Parm) && Property->HasAllPropertyFlags(CPF_BlueprintVisible)) && !bDelegateProp;
		
		FObjectPropertyBase* Obj = CastField<FObjectPropertyBase>(Property);		
		if (!bShouldShowAsVar || Obj == nullptr) continue; 
		if (!Obj->PropertyClass->IsChildOf(UWidget::StaticClass())) continue;
		if (!widgetNames.Contains(Property->GetFName())) continue;

		outVars.Add(UElgBESGraphWidgetVariable::MakeGraphWidgetVariable(BlueprintPtr, Property));
	}
	outVars.Sort([&](const UElgBESGraphWidgetVariable& dataA, const UElgBESGraphWidgetVariable& dataB) {
		return dataA.VariableName.ToString() < dataB.VariableName.ToString();
		});
	return outVars;
}


TArray<UElgBESGraphWidget*> UElgEditorContext_BlueprintEditor::GetWidgetTreeWidgets()
{
	TArray<UElgBESGraphWidget*> outVars;
	// check if this is an widget blueprint graph
	if (UWidgetBlueprint* widgetBP = Cast<UWidgetBlueprint>(BlueprintPtr)) {
		TArray<UWidget*> allWidgets;
		widgetBP->WidgetTree->GetAllWidgets(allWidgets);
		for (UWidget* widget : allWidgets) {
			outVars.Add(UElgBESGraphWidget::MakeGraphWidget(BlueprintPtr, widget));
		}
	}
	outVars.Sort([&](const UElgBESGraphWidget& dataA, const UElgBESGraphWidget& dataB) {
		return dataA.WidgetName.ToString() < dataB.WidgetName.ToString();
		});
	return outVars;		 
}

#pragma endregion


#pragma region LocalVariables

TArray<UElgBESGraphVariableLocal*> UElgEditorContext_BlueprintEditor::GetLocalVariablesInFocusGraph()
{	
	UEdGraph* graph = GetFocusGraph();
	if (!FElgKEWUtils::IsGraphAFunction(graph)) {
		TArray<UElgBESGraphVariableLocal*> vars;
		return vars;
	}

	return FElgKEWUtils::GetLocalVariables(BlueprintPtr, graph);
}
#pragma endregion


#pragma region EventDispatchers

bool UElgEditorContext_BlueprintEditor::CreateEventDispatcher(FName InName, FText InCategory, UElgBESGraphEventDispatcher*& OutEventDispatcher)
{
	if (InName.IsNone()) {
		InName = FBlueprintEditorUtils::FindUniqueKismetName(BlueprintPtr, InName.ToString());
	}

	if (FElgKEWUtils::AddEventDispatcher(BlueprintPtr, InName)) {
		OutEventDispatcher = GetEventDispatcherByName(InName);
		if (OutEventDispatcher) {
			if (InCategory.IsEmpty()) {
				InCategory = FText::FromString("Default");
			}
			OutEventDispatcher->SetVariableCategory(InCategory);
			return true;
		}		
	}
	return false;
}


TArray<UElgBESGraphEventDispatcher*> UElgEditorContext_BlueprintEditor::GetEventDispatchers()
{
	TArray<UElgBESGraphEventDispatcher*> out;
	for (UEdGraph* delegateGraph : BlueprintPtr->DelegateSignatureGraphs) {
		FName name = delegateGraph->GetFName();
		out.Add(UElgBESGraphEventDispatcher::MakeGraphEventDispatcherObject(BlueprintPtr, delegateGraph));
	}
	return out;
}


UElgBESGraphEventDispatcher* UElgEditorContext_BlueprintEditor::GetEventDispatcherByName(const FName InName)
{
	for (UEdGraph* delegateGraph : BlueprintPtr->DelegateSignatureGraphs) {
		if (InName == delegateGraph->GetFName()) {
			return UElgBESGraphEventDispatcher::MakeGraphEventDispatcherObject(BlueprintPtr, delegateGraph);
		}		
	}
	return nullptr;
}


TArray<FName> UElgEditorContext_BlueprintEditor::GetEventDispatcherSignaturesToCopy()
{
	return UElgBESGraphEventDispatcher::GetFunctionsSignature(BlueprintPtr);
}

#pragma endregion


#pragma region Interface

TArray<UElgBESGraphInterface*> UElgEditorContext_BlueprintEditor::GetInterfaceImplemented(const bool bIncludeInherent/* = false*/)
{
	return UElgBESGraphInterface::GetBlueprintInterfaces(BlueprintPtr, bIncludeInherent);
}


UElgBESGraphInterface* UElgEditorContext_BlueprintEditor::GetInterfaceByName(const FName InName)
{
	return UElgBESGraphInterface::GetBlueprintInterface(BlueprintPtr, InName);
}

TArray<UElgBESGraphInterface*> UElgEditorContext_BlueprintEditor::GetInterfaceNativelyImplemented()
{
	return UElgBESGraphInterface::GetBlueprintInterfaces(BlueprintPtr);
}


TArray<FName> UElgEditorContext_BlueprintEditor::GetInterfaceNames()
{
	return UElgBESGraphInterface::GetInterfaces(BlueprintPtr);
}


void UElgEditorContext_BlueprintEditor::AddInterface(const FName InterfaceClass)
{	
	if (UClass* InterfaceClassPtr = (UClass*)StaticFindObject(UClass::StaticClass(), ANY_PACKAGE, *InterfaceClass.ToString())) {
		FBlueprintEditorUtils::ImplementNewInterface(BlueprintPtr, InterfaceClass);
	} else { // might be a BP that is not loaded yet so try to find the asset and load it before trying the interface again.
		FName bpName = InterfaceClass;
		FString tempName = InterfaceClass.ToString();
		if (!tempName.EndsWith(TEXT("_C"))) {
			tempName += TEXT("_C");
			bpName = FName(*tempName);
		}

		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
		IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
		FARFilter Filter;
		Filter.ClassNames.Add(UBlueprint::StaticClass()->GetFName());
		Filter.TagsAndValues.Add(FName(TEXT("BlueprintType")), FString(TEXT("BPTYPE_Interface")));
		TArray<FAssetData> AssetList;
		AssetRegistry.GetAssets(Filter, AssetList);
		if (AssetList.Num()) {
			for (auto const& Asset : AssetList) {
				FAssetDataTagMapSharedView::FFindTagResult Result = Asset.TagsAndValues.FindTag(TEXT("GeneratedClass"));
				if (Result.IsSet()) {
					// Convert path to just the name part
					const FString& GeneratedClassPathPtr = Result.GetValue();
					const FString ClassObjectPath = FPackageName::ExportTextPathToObjectPath(*GeneratedClassPathPtr);
					const FString ClassName = FPackageName::ObjectPathToObjectName(ClassObjectPath);
					if (ClassName == tempName) {
						Asset.GetAsset();
						FBlueprintEditorUtils::ImplementNewInterface(BlueprintPtr, bpName);
					}
				}
			}
		}
	}
}


void UElgEditorContext_BlueprintEditor::RemoveInterface(const FName InterfaceClass, const bool InShowDialog /*= true*/)
{
	if (InterfaceClass.IsNone()) return;
	FName className = InterfaceClass;
	if (UElgBESGraphInterface* inter = GetInterfaceByName(InterfaceClass)) {
		className = inter->InterfaceClassName;
	}

	EAppReturnType::Type DialogReturn = EAppReturnType::No;
	if (InShowDialog) {
		DialogReturn = FMessageDialog::Open(EAppMsgType::YesNoCancel, NSLOCTEXT("UnrealEd", "TransferInterfaceFunctionsToBlueprint", "Would you like to transfer the interface functions to be part of your blueprint?"));
		if (DialogReturn == EAppReturnType::Cancel) return;
	}
	
	TArray<UEdGraph*> Graphs;
	FBlueprintEditorUtils::GetInterfaceGraphs(BlueprintPtr, className, Graphs);
	for( TArray<UEdGraph*>::TIterator GraphIt(Graphs); GraphIt; ++GraphIt ) {
		CloseTab(*GraphIt);
	}

	FBlueprintEditorUtils::RemoveInterface(BlueprintPtr, className, DialogReturn == EAppReturnType::Yes);
}

void UElgEditorContext_BlueprintEditor::RemoveGraphByName(const FName InName)
{
	TArray<UEdGraph*> graphList;
	BlueprintPtr->GetAllGraphs(graphList);

	UEdGraph* graphToRemove = nullptr;
	for (int32 GraphIdx = 0; GraphIdx < graphList.Num(); ++GraphIdx)
	{
		if (InName == graphList[GraphIdx]->GetFName()) {
			graphToRemove = graphList[GraphIdx];
			break;
		}
	}

	if (graphToRemove) {
		const FScopedTransaction Transaction(LOCTEXT("RemoveGraph", "Removed graph"));
		BlueprintPtr->Modify();
		FBlueprintEditorUtils::RemoveGraph(BlueprintPtr, graphToRemove);
	}	
}


void UElgEditorContext_BlueprintEditor::CreateUberGraph(FName InName, UElgBESGraphUber*& OutUberGraph)
{
	InName = FBlueprintEditorUtils::FindUniqueKismetName(BlueprintPtr, InName.ToString());
	const FScopedTransaction Transaction(LOCTEXT("AddNewGraph", "Add New UberGraph"));
	BlueprintPtr->Modify();
	UEdGraph* NewGraph = FBlueprintEditorUtils::CreateNewGraph(BlueprintPtr, InName, UEdGraph::StaticClass(), UEdGraphSchema_K2::StaticClass());
	FBlueprintEditorUtils::AddUbergraphPage(BlueprintPtr, NewGraph);
	FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(NewGraph, false);
	OutUberGraph = UElgBESGraphUber::MakeGraphUberObject(NewGraph);
}


TArray<UElgBESGraphUber*> UElgEditorContext_BlueprintEditor::GetUberGraphs()
{
	TArray<UElgBESGraphUber*> graphs;
	for (UEdGraph* graph : BlueprintPtr->UbergraphPages) {
		graphs.Add(UElgBESGraphUber::MakeGraphUberObject(graph));
	}

	if (graphs.Num() > 1) {
		graphs.Sort([&](const UElgBESGraphUber& dataA, const UElgBESGraphUber& dataB) {
			return dataA.GraphName < dataB.GraphName;
			});
	}

	return graphs;
}


UElgBESGraphUber* UElgEditorContext_BlueprintEditor::GetUberGraphByName(const FName InName)
{
	if (InName.IsNone()) return nullptr;

	for (UEdGraph* graph : BlueprintPtr->UbergraphPages) {
		if (graph->GetFName() == InName) {
			return UElgBESGraphUber::MakeGraphUberObject(graph);
		}
	}
	return nullptr;
}


bool UElgEditorContext_BlueprintEditor::HasExtraUberGraphs()
{
	if (BlueprintPtr->UbergraphPages.Num() == 1) return false;
	return true;
}

UElgBESGraphUber* UElgEditorContext_BlueprintEditor::GetEventGraph()
{
	FName eventGraph = FName(TEXT("EventGraph"));
	for (UEdGraph* graph : BlueprintPtr->UbergraphPages) {
		if (graph->GetFName() == eventGraph) {
			return UElgBESGraphUber::MakeGraphUberObject(graph);
		}
	}
	return nullptr;
}

#pragma endregion



#pragma region Delegates

void UElgEditorContext_BlueprintEditor::HandleOnCompiled(class UBlueprint* InBlueprint)
{
	OnGraphCompiled.Broadcast();
}


void UElgEditorContext_BlueprintEditor::HandleOnChanged(class UBlueprint* InBlueprint)
{
	//UE_LOG(LogElgKismetEditorWidget, Log, TEXT("HandleOnChanged::"));
	bOnGraphChanged = true;
}


void UElgEditorContext_BlueprintEditor::HandleOnGraphChanged(const FEdGraphEditAction& InAction)
{
	//UE_LOG(LogElgKismetEditorWidget, Log, TEXT("HandleOnGraphChanged::"));
	bOnGraphChanged = true;
}


void UElgEditorContext_BlueprintEditor::HandleOnNodeSelectionChanged()
{
	//UE_LOG(LogElgKismetEditorWidget, Log, TEXT("HandleOnNodeSelectionChanged::"));
	OnNodeSelectionChanged.Broadcast();
}


void UElgEditorContext_BlueprintEditor::HandleOnFocusGraphChanged()
{
	//UE_LOG(LogElgKismetEditorWidget, Log, TEXT("HandleOnFocusGraphChanged::"));
	OnFocusGraphChanged.Broadcast();
}


void UElgEditorContext_BlueprintEditor::HandleOnEditorRefresh()
{
	if (KismetInspectorPropertyWrapper) {
		if (FProperty* variableProperty = FindFProperty<FProperty>(BlueprintPtr->SkeletonGeneratedClass, KismetInspectorPropertyName)) {
			ShowPropertyInInspector(variableProperty);
		}
	}
	OnEditorRefresh.Broadcast();
}

#pragma endregion


#undef LOCTEXT_NAMESPACE