// Copyright 2019-2021 ElgSoft. All rights reserved. 


#include "ElgKEWUtils.h"
#include <ElgKEWStructs.h>
#include <Kismet2/BlueprintEditorUtils.h>
#include <K2Node_FunctionEntry.h>
#include <SGraphPanel.h>
#include <Engine/GameViewportClient.h>
#include <Slate/SGameLayerManager.h>
#include <BlueprintFunctionNodeSpawner.h>
#include <EditorUtilityWidget.h>
#include <K2Node_CallFunction.h>
#include <EdGraph/EdGraphPin.h>
#include <K2Node_Variable.h>
#include <Kismet2/Kismet2NameValidators.h>
#include <K2Node_VariableGet.h>
#include <EdGraphSchema_K2_Actions.h>
#include <K2Node_FunctionResult.h>
#include <BPWrappers/ElgBESGraphPin.h>
#include <ElgBESGraphVariableLocal.h>
#include <K2Node_MacroInstance.h>
#include <K2Node_CallDelegate.h>
#include <K2Node_AddDelegate.h>
#include "ObjectEditorUtils.h"
#include <K2Node_Event.h>
#include <Kismet2/KismetEditorUtilities.h>
#include <WidgetBlueprint.h>
#include <Blueprint/WidgetTree.h>
#include <K2Node_AddComponent.h>
#include <Classes/EditorStyleSettings.h>
#include <K2Node_VariableSet.h>
#include "ElgKEW_Log.h"


#define LOCTEXT_NAMESPACE "FElgKismetEditorWidgetModule"

TArray<UEdGraph*> FElgKEWUtils::GetAllGraphs(UBlueprint* InBlueprint)
{
	TArray<UEdGraph*> graphs;
	InBlueprint->GetAllGraphs(graphs);
	return graphs;
}


UFunction* FElgKEWUtils::FindFunction(UBlueprint* InBlueprint, FName InFunctionName)
{
	for (TFieldIterator<UFunction> FunctionIt(InBlueprint->GeneratedClass, EFieldIteratorFlags::ExcludeSuper); FunctionIt; ++FunctionIt) {
		UFunction* function = *FunctionIt;
		if (function->GetFName() == InFunctionName) {
			return function;
		}
	}
	return nullptr;
}


int32 FElgKEWUtils::NodeCount(UBlueprint* InBlueprint)
{
	return GetAllGraphs(InBlueprint).Num();
}


TArray<UFunction*> FElgKEWUtils::GetAllFunctions(UBlueprint* InBlueprint)
{
	TArray<UFunction*> functions;
	for (UEdGraph* graph : InBlueprint->FunctionGraphs) {
		functions.Add(FindFunction(InBlueprint, graph->GetFName()));
	}
	return functions;
}


bool FElgKEWUtils::DoFunctionHasFlag(UFunction* InFunction, EElgFunctionFlags InFlag)
{
	EFunctionFlags flag = EFunctionFlags::FUNC_None;
	switch (InFlag) {
	case EElgFunctionFlags::FUNC_None:					flag = EFunctionFlags::FUNC_None; break;
	case EElgFunctionFlags::FUNC_Final:					flag = EFunctionFlags::FUNC_Final; break;
	case EElgFunctionFlags::FUNC_RequiredAPI:			flag = EFunctionFlags::FUNC_RequiredAPI; break;
	case EElgFunctionFlags::FUNC_BlueprintAuthorityOnly:flag = EFunctionFlags::FUNC_BlueprintAuthorityOnly; break;
	case EElgFunctionFlags::FUNC_BlueprintCosmetic:		flag = EFunctionFlags::FUNC_BlueprintCosmetic; break;
	case EElgFunctionFlags::FUNC_Net:					flag = EFunctionFlags::FUNC_Net; break;
	case EElgFunctionFlags::FUNC_NetReliable:			flag = EFunctionFlags::FUNC_NetReliable; break;
	case EElgFunctionFlags::FUNC_NetRequest:			flag = EFunctionFlags::FUNC_NetRequest; break;
	case EElgFunctionFlags::FUNC_Exec:					flag = EFunctionFlags::FUNC_Exec; break;
	case EElgFunctionFlags::FUNC_Native:				flag = EFunctionFlags::FUNC_Native; break;
	case EElgFunctionFlags::FUNC_Event:					flag = EFunctionFlags::FUNC_Event; break;
	case EElgFunctionFlags::FUNC_NetResponse:			flag = EFunctionFlags::FUNC_NetResponse; break;
	case EElgFunctionFlags::FUNC_Static:				flag = EFunctionFlags::FUNC_Static; break;
	case EElgFunctionFlags::FUNC_NetMulticast:			flag = EFunctionFlags::FUNC_NetMulticast; break;
	case EElgFunctionFlags::FUNC_UbergraphFunction:		flag = EFunctionFlags::FUNC_UbergraphFunction; break;
	case EElgFunctionFlags::FUNC_MulticastDelegate:		flag = EFunctionFlags::FUNC_MulticastDelegate; break;
	case EElgFunctionFlags::FUNC_Public:				flag = EFunctionFlags::FUNC_Public; break;
	case EElgFunctionFlags::FUNC_Private:				flag = EFunctionFlags::FUNC_Private; break;
	case EElgFunctionFlags::FUNC_Protected:				flag = EFunctionFlags::FUNC_Protected; break;
	case EElgFunctionFlags::FUNC_Delegate:				flag = EFunctionFlags::FUNC_Delegate; break;
	case EElgFunctionFlags::FUNC_NetServer:				flag = EFunctionFlags::FUNC_NetServer; break;
	case EElgFunctionFlags::FUNC_HasOutParms:			flag = EFunctionFlags::FUNC_HasOutParms; break;
	case EElgFunctionFlags::FUNC_HasDefaults:			flag = EFunctionFlags::FUNC_HasDefaults; break;
	case EElgFunctionFlags::FUNC_NetClient:				flag = EFunctionFlags::FUNC_NetClient; break;
	case EElgFunctionFlags::FUNC_DLLImport:				flag = EFunctionFlags::FUNC_DLLImport; break;
	case EElgFunctionFlags::FUNC_BlueprintCallable:		flag = EFunctionFlags::FUNC_BlueprintCallable; break;
	case EElgFunctionFlags::FUNC_BlueprintEvent:		flag = EFunctionFlags::FUNC_BlueprintEvent; break;
	case EElgFunctionFlags::FUNC_BlueprintPure:			flag = EFunctionFlags::FUNC_BlueprintPure; break;
	case EElgFunctionFlags::FUNC_EditorOnly:			flag = EFunctionFlags::FUNC_EditorOnly; break;
	case EElgFunctionFlags::FUNC_Const:					flag = EFunctionFlags::FUNC_Const; break;
	case EElgFunctionFlags::FUNC_NetValidate:			flag = EFunctionFlags::FUNC_NetValidate; break;
	case EElgFunctionFlags::FUNC_AllFlags:				flag = EFunctionFlags::FUNC_AllFlags; break;
	}

	return InFunction->HasAnyFunctionFlags(flag);
}


bool FElgKEWUtils::IsInvalid(UObject* InObject, const FString InMessage)
{
	if (!IsValid(InObject)) {
		UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("%s: Invalid object"), *InMessage)
			return true;

	}
	return false;
}


UEdGraph* FElgKEWUtils::GetFunctionGraph(UBlueprint* InBlueprint, UFunction* InFunction)
{
	for (UEdGraph* graph : InBlueprint->FunctionGraphs) {
		if (graph->GetFName() == InFunction->GetFName()) return graph;
	}
	return nullptr;
}


UEdGraph* FElgKEWUtils::GetMacroGraph(UBlueprint* InBlueprint, const FName InName)
{
	for (UEdGraph* graph : InBlueprint->MacroGraphs) {
		if (graph->GetFName() == InName) return graph;
	}
	return nullptr;

}


UK2Node_FunctionEntry* FElgKEWUtils::GetFunctionEntryNodeFromGraph(UEdGraph* InGraph)
{
	TArray<UK2Node_FunctionEntry*> EntryNodes;
	InGraph->GetNodesOfClass(EntryNodes);
	if (EntryNodes.Num() == 1) {
		return EntryNodes[0];
	}
	return nullptr;
}


UK2Node_FunctionEntry* FElgKEWUtils::GetFunctionEntryNode(UBlueprint* InBlueprint, UFunction* InFunction)
{
	if (UEdGraph* graph = GetFunctionGraph(InBlueprint, InFunction)) {
		return GetFunctionEntryNodeFromGraph(graph);
	}
	return nullptr;
}



UK2Node_FunctionResult* FElgKEWUtils::GetFunctionResultNodeFromGraph(UEdGraph* InGraph)
{
	TArray<UK2Node_FunctionResult*> nodes;
	InGraph->GetNodesOfClass(nodes);
	if (nodes.Num() == 1) {
		return nodes[0];
	}
	return nullptr;
}


void FElgKEWUtils::UpdatedNodeParameters(UK2Node_EditablePinBase* InNode)
{
	if (InNode) {
		const bool bCurDisableOrphanSaving = InNode->bDisableOrphanPinSaving;
		InNode->bDisableOrphanPinSaving = true;
		InNode->ReconstructNode();
		InNode->bDisableOrphanPinSaving = bCurDisableOrphanSaving;
		GetDefault<UEdGraphSchema_K2>()->HandleParameterDefaultValueChanged(InNode);
	}
}


bool FElgKEWUtils::IsFunctionPure(UFunction* InFunction)
{
	return InFunction->HasAnyFunctionFlags(EFunctionFlags::FUNC_BlueprintPure);
}


bool FElgKEWUtils::SpawnFunctionNodeInGraph(UBlueprint* InBlueprint, UEdGraph* InGraph, const FName InFunctionName, const FVector2D InNodePosition, UEdGraphPin* InPin/* = nullptr*/)
{
	if (!InBlueprint || !InGraph || !InFunctionName.IsValid()) return false;

	UBlueprintFunctionNodeSpawner* functionNodeSpawner;

	// do we need this???
	FGraphActionListBuilderBase tempListBuilder;
	tempListBuilder.OwnerOfTemporaries = NewObject<UEdGraph>(InBlueprint);
	tempListBuilder.OwnerOfTemporaries->SetFlags(RF_Transient);

	UFunction* functionToSpawn = FindUField<UFunction>(InBlueprint->SkeletonGeneratedClass, InFunctionName);
	if (!functionToSpawn) {
		UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("Failed to find any function named [%s] in [%s]" ), *InFunctionName.ToString(), *InBlueprint->GetName())
		return false;
	}
	if (!functionToSpawn->HasAnyFunctionFlags((FUNC_BlueprintCallable | FUNC_BlueprintPure))) {
		UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("Function [%s] in [%s] is not Callable or Pure"), *InFunctionName.ToString(), *InBlueprint->GetName())
		return false;
	}

	functionNodeSpawner = UBlueprintFunctionNodeSpawner::Create(functionToSpawn);

	const FScopedTransaction Transaction(LOCTEXT("AddedFunction", "Function added to Graph"));

	IBlueprintNodeBinder::FBindingSet bindings;
	UEdGraphNode* resultNode = functionNodeSpawner->Invoke(InGraph, bindings, InNodePosition);
	
	if (InPin) {
		FElgKEWUtils::DropNodeOnPin(Cast<UK2Node>(resultNode), InPin);
	}

	FBlueprintEditorUtils::MarkBlueprintAsModified(InBlueprint);
	return true;
}


bool FElgKEWUtils::SpawnMacroNodeInGraph(UBlueprint* InBlueprint, UEdGraph* InGraph, const FName InMacroName, const FVector2D InNodePosition, UEdGraphPin* InPin/* = nullptr*/)
{
	if (!InBlueprint || !InGraph || !InMacroName.IsValid()) return false;

	if (UEdGraph* macroGraph = GetMacroGraph(InBlueprint, InMacroName)) {
		const FScopedTransaction Transaction(LOCTEXT("AddedMacro", "Macro node added to Graph"));
		InGraph->Modify();

		UK2Node_MacroInstance* resultNode = FEdGraphSchemaAction_K2NewNode::SpawnNode<UK2Node_MacroInstance>(
			InGraph,
			InNodePosition,
			EK2NewNodeFlags::SelectNewNode,
			[macroGraph](UK2Node_MacroInstance* NewMacroInastance) {
				NewMacroInastance->SetMacroGraph(macroGraph);
			}
		);
		if (InPin) {
			FElgKEWUtils::DropNodeOnPin(resultNode, InPin);
		}		FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(InBlueprint);
		return true;
	}
	return false;
}

bool FElgKEWUtils::SpawnVariableNodeInGraph(UBlueprint* InBlueprint, UEdGraph* InGraph, const FName InVariableName, const FVector2D InNodePosition, bool bGetter, UEdGraphPin* InPin/* = nullptr*/)
{
	if (FProperty* variableProperty = FindFProperty<FProperty>(InBlueprint->SkeletonGeneratedClass, InVariableName)) {
		UStruct* Outer = variableProperty->GetOwnerChecked<UStruct>();
		const UEdGraphSchema_K2* K2_Schema = Cast<const UEdGraphSchema_K2>(InGraph->GetSchema());
		if (K2_Schema) {

			if (InPin && !CanDropPropertyOnPin(variableProperty, InPin)) return false;

			const FScopedTransaction Transaction(LOCTEXT("AddedVariable", "Variable node added to Graph"));
			InGraph->Modify();
			InBlueprint->Modify();
			UK2Node_Variable* node;

			if (InPin) {
				bGetter = (InPin->Direction == EGPD_Output) ? false : true;
			}

			if (bGetter) {
				node = K2_Schema->SpawnVariableGetNode(InNodePosition, InGraph, InVariableName, Outer);
			} else {
				node = K2_Schema->SpawnVariableSetNode(InNodePosition, InGraph, InVariableName, Outer);
			}
			if (InPin) {
				DropVariableOnPin(node, InPin);
			}
			FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(InBlueprint);
			return true;
		}
	}
	return false;
}


bool FElgKEWUtils::SpawnLocalVariableNodeInGraph(UBlueprint* InBlueprint, UEdGraph* InGraph, const FName InVariableName, const FVector2D InNodePosition, bool bGetter, UEdGraphPin* InPin/* = nullptr*/)
{
	if (!IsGraphAFunction(InGraph)) return false;

	if (UFunction* func = FindFunctionInBlueprint(InBlueprint, InGraph->GetFName())) {
		if (UFunction* Outer = GetLocalVaribleScope(func, InVariableName))  {
			const UEdGraphSchema_K2* K2_Schema = Cast<const UEdGraphSchema_K2>(InGraph->GetSchema());
			if (K2_Schema) {

				// override what type of node to spawn if it's dropped on a Pin
				if (InPin) {
					FProperty* variableProperty = GetFunctionProperty(func, InVariableName);
					if (!CanDropPropertyOnPin(variableProperty, InPin)) return false;
					bGetter = (InPin->Direction == EGPD_Output) ? false : true;
				}
				const FScopedTransaction Transaction(LOCTEXT("AddedLocalVariable", "Local Variable node added to Graph"));
				InBlueprint->Modify();
				InGraph->Modify();
				UK2Node_Variable* node;
				if (bGetter) {
					node = K2_Schema->SpawnVariableGetNode(InNodePosition, InGraph, InVariableName, Outer);
				} else {
					node = K2_Schema->SpawnVariableSetNode(InNodePosition, InGraph, InVariableName, Outer);
				}
				if (InPin) {
					DropVariableOnPin(node, InPin);
				}
				FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(InBlueprint);
				return true;
			}
		}
	}

	return false;
}


bool FElgKEWUtils::SpawnPinVariableNodeInGraph(UBlueprint* InBlueprint, UEdGraph* InGraph, const FName InVariableName, const FVector2D InNodePosition, UEdGraphPin* InPin/* = nullptr*/)
{
	if (!IsGraphAFunction(InGraph)) return false;
	bool bHasInputPin = false;
	if (UK2Node_FunctionEntry* entryNode = GetFunctionEntryNodeFromGraph(InGraph)) {		
		for (UEdGraphPin* pin : entryNode->Pins) {
			if (pin->PinName == InVariableName) {
				bHasInputPin = true;
				break;
			}
		}		
	}
	if (!bHasInputPin) return false;
	// You can not set the data in the PinVariables so if dropped on a Output pin we skip spawning the node.
	if (InPin && InPin->Direction == EGPD_Output) return false; 


	if (UFunction* func = FindFunctionInBlueprint(InBlueprint, InGraph->GetFName())) {
		if (UStruct* owningScope = Cast<UStruct>(func)) {
			const UEdGraphSchema_K2* K2_Schema = Cast<const UEdGraphSchema_K2>(InGraph->GetSchema());
			if (K2_Schema) {
				if (InPin) {
					FProperty* variableProperty = GetFunctionProperty(func, InVariableName);
					if (!CanDropPropertyOnPin(variableProperty, InPin)) return false;
				}
				const FScopedTransaction Transaction(LOCTEXT("AddedLocalVariable", "Local Variable node added to Graph"));
				UK2Node_VariableGet* node = K2_Schema->SpawnVariableGetNode(InNodePosition, InGraph, InVariableName, owningScope);
				FGuid nodeGuid;
				nodeGuid.Invalidate();
				node->VariableReference.SetLocalMember(InVariableName, InGraph->GetName(), nodeGuid);
				node->AllocateDefaultPins();
				
				if (InPin) {
					DropVariableOnPin(node, InPin);
				}

				FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(InBlueprint);
				return true;
			}
		}
	}
	return false;
}


bool FElgKEWUtils::SpawnEventDispatcherNodeInGraph(UBlueprint* InBlueprint, UEdGraph* InGraph, const FName InVariableName, const FVector2D InNodePosition, const bool bInCall, UEdGraphPin* InPin/* = nullptr*/)
{
	FMulticastDelegateProperty* propertyDelegate = FindFProperty<FMulticastDelegateProperty>(InBlueprint->SkeletonGeneratedClass, InVariableName);
	if (!propertyDelegate) return false;

	const UClass* VariableSourceClass = propertyDelegate->GetOwnerChecked<UClass>();
	const UBlueprint* DropOnBlueprint = FBlueprintEditorUtils::FindBlueprintForGraph(InGraph);
	bool bSelfContext = VariableSourceClass == NULL || DropOnBlueprint->SkeletonGeneratedClass->IsChildOf(VariableSourceClass);

	if (bInCall) {
		UK2Node* resultNode = FEdGraphSchemaAction_K2NewNode::SpawnNode<UK2Node_CallDelegate>(
			InGraph,
			InNodePosition,
			EK2NewNodeFlags::SelectNewNode,
			[&propertyDelegate, bSelfContext](UK2Node_CallDelegate* NewInstance)
			{
				NewInstance->SetFromProperty(propertyDelegate, bSelfContext, propertyDelegate->GetOwnerClass());
			}
		);

		if (InPin) {
			FElgKEWUtils::DropNodeOnPin(resultNode, InPin);
		}
		return true;
	} else {		
		UK2Node* resultNode = FEdGraphSchemaAction_K2NewNode::SpawnNode<UK2Node_AddDelegate>(
			InGraph,
			InNodePosition,
			EK2NewNodeFlags::SelectNewNode,
			[&propertyDelegate, bSelfContext](UK2Node_AddDelegate* NewInstance)
			{
				NewInstance->SetFromProperty(propertyDelegate, bSelfContext, propertyDelegate->GetOwnerClass());
			}
		);
		if (InPin) {
			FElgKEWUtils::DropNodeOnPin(resultNode, InPin);
		}
		return true;
	}
	return false;
}


bool FElgKEWUtils::SpawnEventNodeInGraph(UBlueprint* InBlueprint, const FName InEventName, const FVector2D InNodePosition)
{
	UFunction* OverrideFunc = nullptr;
	UClass* const OverrideFuncClass = FBlueprintEditorUtils::GetOverrideFunctionClass(InBlueprint, InEventName, &OverrideFunc);
	UEdGraph* EventGraph = FBlueprintEditorUtils::FindEventGraph(InBlueprint);
	FVector2D nodePos = (InNodePosition.IsNearlyZero()) ? EventGraph->GetGoodPlaceForNewNode() : InNodePosition;
	UK2Node_Event* NewEventNode = FEdGraphSchemaAction_K2NewNode::SpawnNode<UK2Node_Event>(
		EventGraph,
		nodePos,
		EK2NewNodeFlags::SelectNewNode,
		[InEventName, OverrideFuncClass](UK2Node_Event* NewInstance)
		{
			NewInstance->EventReference.SetExternalMember(InEventName, OverrideFuncClass);
			NewInstance->bOverrideFunction = true;
		}
	);

	if (NewEventNode) {
		FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(NewEventNode);
		return true;
	}
	return false;
}


bool FElgKEWUtils::IsMouseOverGraphEditor()
{
	TSharedPtr<SWidget> widget = GetSlateWidgetUnderMouse();
	if (widget.IsValid()) {
		return IsWidgetSGraphPanel(widget);
	}
	return false;
}


TSharedPtr<SWidget> FElgKEWUtils::GetSlateWidgetUnderMouse()
{
	TArray<TSharedRef<SWindow>> Windows;
	FSlateApplication::Get().GetAllVisibleWindowsOrdered(Windows);
	FWidgetPath WidgetPath = FSlateApplication::Get().LocateWindowUnderMouse(FSlateApplication::Get().GetCursorPos(), Windows);
	if (WidgetPath.IsValid())
	{
		TSharedPtr<SWidget> widget = WidgetPath.GetLastWidget();
		return widget;
	}
	return nullptr;
}

TArray<TSharedRef<SWidget>> FElgKEWUtils::GetSlateWidgetsUnderMouse()
{
	TArray<TSharedRef<SWidget>> widgets;
	TArray<TSharedRef<SWindow>> Windows;
	FSlateApplication::Get().GetAllVisibleWindowsOrdered(Windows);
	FWidgetPath WidgetPath = FSlateApplication::Get().LocateWindowUnderMouse(FSlateApplication::Get().GetCursorPos(), Windows);
	if (WidgetPath.IsValid())
	{
		WidgetPath.Widgets.Reverse();
		for (int32 Idx = 0; Idx < WidgetPath.Widgets.Num(); ++Idx) {
			FArrangedWidget& Widget = WidgetPath.Widgets[Idx];
			widgets.Add(Widget.Widget);
		}
	}
	return widgets;
}

FVector2D FElgKEWUtils::GetMousePosition()
{
	return FSlateApplication::Get().GetCursorPos();
}


FVector2D FElgKEWUtils::GetMouseViewportPosition()
{
	if (!GEditor) return FVector2D(0, 0);
	FGeometry viewportGeometry;
	FVector2D mousePosition = FSlateApplication::Get().GetCursorPos();
	if (UWorld* world = GEditor->GetWorld()) {
		if (UGameViewportClient* viewportClient = world->GetGameViewport()) {
			TSharedPtr<IGameLayerManager> layerManager = viewportClient->GetGameLayerManager();
			if (layerManager.IsValid()) {
				viewportGeometry = layerManager->GetViewportWidgetHostGeometry();				
			}
		}
	}
	return viewportGeometry.AbsoluteToLocal(mousePosition);
}


FVector2D FElgKEWUtils::GetMousePositionInGraph(TSharedPtr<SGraphPanel> InGraphPanelWidget)
{
	const FGeometry& testGeo = InGraphPanelWidget->GetTickSpaceGeometry();
	const FVector2D nodePosition = InGraphPanelWidget->PanelCoordToGraphCoord(testGeo.AbsoluteToLocal(FElgKEWUtils::GetMouseViewportPosition()));	
	return nodePosition;
}

bool FElgKEWUtils::IsWidgetSGraphPanel(TSharedPtr<SWidget> InWidget)
{
	if (InWidget->GetType() == FName("SGraphPanel")) return true;
	return false;
}


UObject* FElgKEWUtils::GetEditorWidgetParent(UObject* InObject)
{
	UObject* outer = nullptr;
	if (!InObject) return outer;
	outer = InObject->GetOuter();
	if (outer == nullptr) return outer;
	if (outer->IsA(UEditorUtilityWidget::StaticClass())) {
		return outer;
	} else {
		outer = GetEditorWidgetParent(outer);
	}
	return outer;
}



void FElgKEWUtils::GetNodeMemberName(UEdGraphNode* InNode, FString& OutName, FString& OutParentClass, FString& OutParentPath) 
 {
	if (!IsValid(InNode)) return ;

	if (UK2Node_CallFunction* nodeType = Cast<UK2Node_CallFunction>(InNode)) {
		GetCallFunctionMemberName(nodeType, OutName, OutParentClass, OutParentPath);
	}
}


void FElgKEWUtils::GetCallFunctionMemberName(UEdGraphNode* InNode, FString& OutName, FString& OutParentClass, FString& OutParentPath)
{	
	if (UK2Node_CallFunction* nodeType = Cast<UK2Node_CallFunction>(InNode)) {
		for (TFieldIterator<FProperty> It(InNode->GetClass(), EFieldIteratorFlags::IncludeSuper); It; ++It) {
			FProperty* currentProperty = *It;
			FName propertyName = currentProperty->GetFName();
			if (propertyName == "FunctionReference") {
				if (FStructProperty* structProperty = CastField<FStructProperty>(currentProperty)) {					
					FMemberReference* theStructValue = structProperty->ContainerPtrToValuePtr<FMemberReference>(InNode);
					UClass* parentClass = theStructValue->GetMemberParentClass();

					if (parentClass) {
						OutName = theStructValue->GetMemberName().ToString();
						OutParentClass = theStructValue->GetMemberParentClass()->GetName();
						OutParentPath = theStructValue->GetMemberParentClass()->GetPathName();
					} else {
						OutName = FString::Printf(TEXT("this.%s"), *theStructValue->GetMemberName().ToString());
					}
				}
				break;
			}
		}
	}
}


FGuid FElgKEWUtils::GetCallfunctionMemberGuid(UEdGraphNode* InNode)
{
	if (UK2Node_CallFunction* nodeType = Cast<UK2Node_CallFunction>(InNode)) {
		for (TFieldIterator<FProperty> It(InNode->GetClass(), EFieldIteratorFlags::IncludeSuper); It; ++It) {
			FProperty* currentProperty = *It;
			FName propertyName = currentProperty->GetFName();
			if (propertyName == "FunctionReference") {
				if (FStructProperty* structProperty = CastField<FStructProperty>(currentProperty)) {
					FMemberReference* theStructValue = structProperty->ContainerPtrToValuePtr<FMemberReference>(InNode);
					return theStructValue->GetMemberGuid();
				}
			}
		}
	}
	FGuid guid;
	guid.Invalidate();
	return guid;
}



FName FElgKEWUtils::PinCategoryEnumToName(EBPElgKEWPinCategory InPinCategory)
{
	switch (InPinCategory) {
	case EBPElgKEWPinCategory::PC_Exec:			return FName(TEXT("exec"));
	case EBPElgKEWPinCategory::PC_Boolean:		return FName(TEXT("bool"));
	case EBPElgKEWPinCategory::PC_Byte:			return FName(TEXT("byte"));
	case EBPElgKEWPinCategory::PC_Class:		return FName(TEXT("class"));
	case EBPElgKEWPinCategory::PC_SoftClass:	return FName(TEXT("softclass"));
	case EBPElgKEWPinCategory::PC_Int:			return FName(TEXT("int"));
	case EBPElgKEWPinCategory::PC_Int64:		return FName(TEXT("int64"));
	case EBPElgKEWPinCategory::PC_Float:		return FName(TEXT("float"));
	case EBPElgKEWPinCategory::PC_Name:			return FName(TEXT("name"));
	case EBPElgKEWPinCategory::PC_Delegate:		return FName(TEXT("delegate"));
	case EBPElgKEWPinCategory::PC_MCDelegate:	return FName(TEXT("mcdelegate"));
	case EBPElgKEWPinCategory::PC_Object:		return FName(TEXT("object"));
	case EBPElgKEWPinCategory::PC_Interface:	return FName(TEXT("interface"));
	case EBPElgKEWPinCategory::PC_SoftObject:	return FName(TEXT("softobject"));
	case EBPElgKEWPinCategory::PC_String:		return FName(TEXT("string"));
	case EBPElgKEWPinCategory::PC_Text:			return FName(TEXT("text"));
	case EBPElgKEWPinCategory::PC_Struct:		return FName(TEXT("struct"));
	case EBPElgKEWPinCategory::PC_Wildcard:		return FName(TEXT("wildcard"));
	case EBPElgKEWPinCategory::PC_Enum:			return FName(TEXT("enum"));
	case EBPElgKEWPinCategory::PC_FieldPath:	return FName(TEXT("fieldpath"));
	case EBPElgKEWPinCategory::PSC_Self:		return FName(TEXT("self"));
	case EBPElgKEWPinCategory::PSC_Index:		return FName(TEXT("index"));
	case EBPElgKEWPinCategory::PSC_Bitmask:		return FName(TEXT("bitmask"));
	}
	return FName();
}

EBPElgKEWPinCategory FElgKEWUtils::NameToPinCategoryEnum(FName InPinCategory)
{
	
	if (InPinCategory == FName(TEXT("exec"))) {
		return EBPElgKEWPinCategory::PC_Exec;
	} else if (InPinCategory == FName(TEXT("bool"))) {
		return EBPElgKEWPinCategory::PC_Boolean;
	} else if (InPinCategory == FName(TEXT("byte"))) {
		return EBPElgKEWPinCategory::PC_Byte;
	} else if (InPinCategory == FName(TEXT("class"))) {
		return EBPElgKEWPinCategory::PC_Class;
	} else if (InPinCategory == FName(TEXT("softclass"))) {
		return EBPElgKEWPinCategory::PC_SoftClass;
	} else if (InPinCategory == FName(TEXT("int"))) {
		return EBPElgKEWPinCategory::PC_Int;
	} else if (InPinCategory == FName(TEXT("int64"))) {
		return EBPElgKEWPinCategory::PC_Int64;
	} else if (InPinCategory == FName(TEXT("float"))) {
		return EBPElgKEWPinCategory::PC_Float;
	} else if (InPinCategory == FName(TEXT("name"))) {
		return EBPElgKEWPinCategory::PC_Name;
	} else if (InPinCategory == FName(TEXT("delegate"))) {
		return EBPElgKEWPinCategory::PC_Delegate;
	} else if (InPinCategory == FName(TEXT("mcdelegate"))) {
		return EBPElgKEWPinCategory::PC_MCDelegate;
	} else if (InPinCategory == FName(TEXT("object"))) {
		return EBPElgKEWPinCategory::PC_Object;
	} else if (InPinCategory == FName(TEXT("interface"))) {
		return EBPElgKEWPinCategory::PC_Interface;
	} else if (InPinCategory == FName(TEXT("softobject"))) {
		return EBPElgKEWPinCategory::PC_SoftObject;
	} else if (InPinCategory == FName(TEXT("string"))) {
		return EBPElgKEWPinCategory::PC_String;
	} else if (InPinCategory == FName(TEXT("text"))) {
		return EBPElgKEWPinCategory::PC_Text;
	} else if (InPinCategory == FName(TEXT("struct"))) {
		return EBPElgKEWPinCategory::PC_Struct;
	} else if (InPinCategory == FName(TEXT("wildcard"))) {
		return EBPElgKEWPinCategory::PC_Wildcard;
	} else if (InPinCategory == FName(TEXT("enum"))) {
		return EBPElgKEWPinCategory::PC_Enum;
	} else if (InPinCategory == FName(TEXT("fieldpath"))) {
		return EBPElgKEWPinCategory::PC_FieldPath;
	} else if (InPinCategory == FName(TEXT("self"))) {
		return EBPElgKEWPinCategory::PSC_Self;
	} else if (InPinCategory == FName(TEXT("index"))) {
		return EBPElgKEWPinCategory::PSC_Index;
	} else if (InPinCategory == FName(TEXT("bitmask"))) {
		return EBPElgKEWPinCategory::PSC_Bitmask;
	}
	return EBPElgKEWPinCategory::PC_Exec;
}


FEdGraphPinType FElgKEWUtils::PinCategoryToPinType(EBPElgKEWPinCategory InPinCategory)
{
	FEdGraphPinType type;
	type.PinCategory = PinCategoryEnumToName(InPinCategory);
	type.PinSubCategory = NAME_None;  // TODO:: need to set sub category on some of them... and they will need more info
	type.PinSubCategoryObject = nullptr;
	return type;
}




FS_ElgGraphPinType FElgKEWUtils::ConvertToPinType(FEdGraphPinType InPinType)
{
	FS_ElgGraphPinType pinType;
	pinType.EdGraphPinType = InPinType;
	pinType.EPinCategory = NameToPinCategoryEnum(InPinType.PinCategory);
	pinType.EPinSubCategory = NameToPinCategoryEnum(InPinType.PinSubCategory);
	pinType.EPinContainerType = ConvertToPinContainerType(InPinType.ContainerType);

	return pinType;
}


FEdGraphPinType FElgKEWUtils::ConvertFromPinType(FS_ElgGraphPinType InPinType)
{
	return InPinType.EdGraphPinType;
}


EBPElgPinContainerType FElgKEWUtils::ConvertToPinContainerType(EPinContainerType InContainerType)
{
	switch (InContainerType)
	{
	case EPinContainerType::Array :	return EBPElgPinContainerType::Array;
	case EPinContainerType::Map:	return EBPElgPinContainerType::Map;
	case EPinContainerType::Set:	return EBPElgPinContainerType::Set;
	default:						return EBPElgPinContainerType::None;
	}
}


EPinContainerType FElgKEWUtils::ConvertFromPinContainerType(EBPElgPinContainerType InContainerType)
{
	switch (InContainerType)
	{
	case EBPElgPinContainerType::Array:		return EPinContainerType::Array;
	case EBPElgPinContainerType::Map:		return EPinContainerType::Map;
	case EBPElgPinContainerType::Set:		return EPinContainerType::Set;
	default:								return EPinContainerType::None;
	}
}


EBPElgEPinTypeFilter FElgKEWUtils::ConvertToPinTypeFilter(ETypeTreeFilter InPinFilter)
{
	switch (InPinFilter)
	{
	case ETypeTreeFilter::AllowExec:			return EBPElgEPinTypeFilter::AllowExec;
	case ETypeTreeFilter::AllowWildcard:		return EBPElgEPinTypeFilter::AllowWildcard;
	case ETypeTreeFilter::IndexTypesOnly:		return EBPElgEPinTypeFilter::IndexTypesOnly;
	case ETypeTreeFilter::RootTypesOnly:		return EBPElgEPinTypeFilter::RootTypesOnly;
	default:									return EBPElgEPinTypeFilter::None;
	}
}


ETypeTreeFilter FElgKEWUtils::ConvertFromPinTypeFilter(EBPElgEPinTypeFilter InPinFilter)
{
	switch (InPinFilter)
	{
	case EBPElgEPinTypeFilter::AllowExec:			return ETypeTreeFilter::AllowExec;
	case EBPElgEPinTypeFilter::AllowWildcard:		return ETypeTreeFilter::AllowWildcard;
	case EBPElgEPinTypeFilter::IndexTypesOnly:		return ETypeTreeFilter::IndexTypesOnly;
	case EBPElgEPinTypeFilter::RootTypesOnly:		return ETypeTreeFilter::RootTypesOnly;
	default:										return ETypeTreeFilter::None;
	}
}

FS_ElgGraphPinType FElgKEWUtils::GetDefaultPinType()
{
	FEdGraphPinType type;
	type.PinCategory = UEdGraphSchema_K2::PC_Boolean;
	return FElgKEWUtils::ConvertToPinType(type);	
}

FString FElgKEWUtils::GetPropertyValueAsString(UBlueprint* InBlueprint, FProperty* InProperty)
{
	check(InBlueprint);
	check(InProperty);

	FString outValue;
	UObject* generatedCDO = InBlueprint->GeneratedClass->GetDefaultObject();
	void* propertyAddr = InProperty->ContainerPtrToValuePtr<void>(generatedCDO);
	InProperty->ExportTextItem(outValue, propertyAddr, propertyAddr, nullptr, PPF_SerializedAsImportText);
	return outValue;
}


void FElgKEWUtils::SetPropertyValueAsString(UBlueprint* InBlueprint, FProperty* InProperty, FString InValue)
{
	check(InBlueprint);
	check(InProperty);

	const FScopedTransaction Transaction(LOCTEXT("ChangeDefaults", "Change Defaults"));
	InBlueprint->Modify();
	UObject* generatedCDO = InBlueprint->GeneratedClass->GetDefaultObject();
	void* propertyAddr = InProperty->ContainerPtrToValuePtr<void>(generatedCDO);
	InProperty->ImportText(*InValue, propertyAddr, 0, generatedCDO);
	FBlueprintEditorUtils::MarkBlueprintAsModified(InBlueprint);
}


void FElgKEWUtils::SetVariableCategory(UBlueprint* InBlueprint, const FName InVariableName, FText InNewCategory)
{
	// Remove excess whitespace and prevent categories with just spaces
	FText CategoryName = FText::TrimPrecedingAndTrailing(InNewCategory);

	if (FProperty* varProperty = GetBlueprintProperty(InBlueprint, InVariableName)) {
		UClass* outerClass = varProperty->GetOwner<UClass>();
		if (outerClass->ClassGeneratedBy == NULL) return;
		if (outerClass != InBlueprint->SkeletonGeneratedClass) return;

		FBlueprintEditorUtils::SetBlueprintVariableCategory(InBlueprint, InVariableName, NULL, CategoryName, false);
		//FBlueprintEditorUtils::MarkBlueprintAsModified(InBlueprint);
	}	
}


void FElgKEWUtils::RemoveVariable(UBlueprint* InBlueprint, const FName InVariableName)
{
	const FScopedTransaction Transaction(LOCTEXT("RemoveVariable", "Remove Variable"));
	InBlueprint->Modify();
	FBlueprintEditorUtils::RemoveMemberVariable(InBlueprint, InVariableName);
}


FProperty* FElgKEWUtils::GetBlueprintProperty(UBlueprint* InBlueprint, const FName InName)
{
	return FindFProperty<FProperty>(InBlueprint->SkeletonGeneratedClass, InName);
}


TArray<FProperty*> FElgKEWUtils::GetVariableProperties(UBlueprint* InBlueprint, const bool IncludeWidgets /*= false*/, const bool IncludeComponents/* = false*/)
{
	check(InBlueprint);
	TArray<FProperty*> outVars;

	// if we do not want any of the widget variables that are from widget tree we get there names so we check against them
	TArray<FName> widgetNames;
	if (!IncludeWidgets) {
		widgetNames = GetWidgetTreeNames(InBlueprint);
	}

	for (TFieldIterator<FProperty> PropertyIt(InBlueprint->SkeletonGeneratedClass, EFieldIteratorFlags::ExcludeSuper); PropertyIt; ++PropertyIt) 	{
		FProperty* propertyPtr = *PropertyIt;
		const bool bMulticastDelegateProp = propertyPtr->IsA(FMulticastDelegateProperty::StaticClass());
		const bool bDelegateProp = (propertyPtr->IsA(FDelegateProperty::StaticClass()) || bMulticastDelegateProp);
		const bool bShouldShowAsVar = (!propertyPtr->HasAnyPropertyFlags(CPF_Parm) && propertyPtr->HasAllPropertyFlags(CPF_BlueprintVisible)) && !bDelegateProp;
		const bool bShouldShowAsDelegate = !propertyPtr->HasAnyPropertyFlags(CPF_Parm) && bMulticastDelegateProp
			&& propertyPtr->HasAnyPropertyFlags(CPF_BlueprintAssignable | CPF_BlueprintCallable);
		if(!bShouldShowAsVar) continue;
		if (bShouldShowAsDelegate) continue;

		if (FObjectPropertyBase* Obj = CastField<FObjectPropertyBase>(propertyPtr)) {
			if (!IncludeWidgets && Obj->PropertyClass->IsChildOf(UWidget::StaticClass())) {
				if (widgetNames.Contains(Obj->GetFName())) continue; // this was a widget in the widget tree so skip it.
			}			
			if (!IncludeComponents && Obj->PropertyClass->IsChildOf(UActorComponent::StaticClass())) continue;
		}

		outVars.Add(propertyPtr);
	}
	return outVars;
}



TArray<FName> FElgKEWUtils::GetWidgetTreeNames(UBlueprint* InBlueprint)
{
	TArray<FName> outNames;
	if (UWidgetBlueprint* widgetBP = Cast<UWidgetBlueprint>(InBlueprint)) {
		TArray<UWidget*> allWidgets;
		widgetBP->WidgetTree->GetAllWidgets(allWidgets);
		for (UWidget* widget : allWidgets) {
			outNames.Add(widget->GetFName());
		}
	}
	return outNames;		 
}

FText FElgKEWUtils::GetPropertyCategoryText(FProperty* InProperty)
{	
	FText Category = FObjectEditorUtils::GetCategoryText(InProperty);

	// if its an Component set the category to Components if it do not have anything set.
	if (FObjectPropertyBase* Obj = CastField<FObjectPropertyBase>(InProperty)) {
		const bool bComponentProperty = Obj && Obj->PropertyClass ? Obj->PropertyClass->IsChildOf<UActorComponent>() : false;
		if (bComponentProperty) {
			FString category = Category.ToString();
			if (category.IsEmpty() || category.Equals("Default")) {
				Category = FText::FromString("Components");
			}
		}
	}	
	return Category;
}

TArray<FString> FElgKEWUtils::GetCategoryTextsAsString(UBlueprint* InBlueprint, const bool bShowUserVarsOnly /*= true*/)
{
	TArray<FString> categoryNames;
	categoryNames.Add("Default");

	TArray<FName> VisibleVariables;
	check(InBlueprint != NULL);
	check(InBlueprint->SkeletonGeneratedClass != NULL);

	EFieldIteratorFlags::SuperClassFlags SuperClassFlag = EFieldIteratorFlags::ExcludeSuper;
	if (!bShowUserVarsOnly) {
		SuperClassFlag = EFieldIteratorFlags::IncludeSuper;
	}

	for (TFieldIterator<FProperty> PropertyIt(InBlueprint->SkeletonGeneratedClass, SuperClassFlag); PropertyIt; ++PropertyIt) {
		FProperty* Property = *PropertyIt;
		if ((!Property->HasAnyPropertyFlags(CPF_Parm) && Property->HasAllPropertyFlags(CPF_BlueprintVisible))) {
			if (FObjectPropertyBase* Obj = CastField<FObjectPropertyBase>(Property)) {
				const bool bComponentProperty = Obj && Obj->PropertyClass ? Obj->PropertyClass->IsChildOf<UActorComponent>() : false;
				if (bComponentProperty) {
					categoryNames.AddUnique(GetPropertyCategoryText(Obj).ToString());
					continue;
				}
			}
			VisibleVariables.Add(Property->GetFName());
		}
	}

	for (int32 i = 0; i < VisibleVariables.Num(); ++i) {
		FText Category = FBlueprintEditorUtils::GetBlueprintVariableCategory(InBlueprint, VisibleVariables[i], nullptr);
		if (!Category.IsEmpty() && !Category.EqualTo(FText::FromString(InBlueprint->GetName()))) {
			categoryNames.AddUnique(Category.ToString());
		}
	}

	// Search through all function graphs for entry nodes to search for local variables to pull their categories
	for (UEdGraph* FunctionGraph : InBlueprint->FunctionGraphs) {
		if (UFunction* Function = InBlueprint->SkeletonGeneratedClass->FindFunctionByName(FunctionGraph->GetFName())) {
			FText FunctionCategory = FObjectEditorUtils::GetCategoryText(Function);
			if (!FunctionCategory.IsEmpty()) {
				categoryNames.AddUnique(FunctionCategory.ToString());
			}
		}

		UK2Node_EditablePinBase* EntryNode = FBlueprintEditorUtils::GetEntryNode(FunctionGraph);
		if (UK2Node_FunctionEntry* FunctionEntryNode = Cast<UK2Node_FunctionEntry>(EntryNode)) {
			for (FBPVariableDescription& Variable : FunctionEntryNode->LocalVariables) {
				categoryNames.AddUnique(Variable.Category.ToString());
			}
		}
	}

	for (UEdGraph* MacroGraph : InBlueprint->MacroGraphs) {
		UK2Node_EditablePinBase* EntryNode = FBlueprintEditorUtils::GetEntryNode(MacroGraph);
		if (UK2Node_Tunnel* TypedEntryNode = ExactCast<UK2Node_Tunnel>(EntryNode)) {
			if (!TypedEntryNode->MetaData.Category.IsEmpty()) {
				categoryNames.AddUnique(TypedEntryNode->MetaData.Category.ToString());
			}
		}
	}

	// Pull categories from overridable functions
	for (TFieldIterator<UFunction> FunctionIt(InBlueprint->ParentClass, EFieldIteratorFlags::IncludeSuper); FunctionIt; ++FunctionIt) {
		const UFunction* Function = *FunctionIt;
		const FName FunctionName = Function->GetFName();

		if (UEdGraphSchema_K2::CanKismetOverrideFunction(Function) && !UEdGraphSchema_K2::FunctionCanBePlacedAsEvent(Function)) {
			FText FunctionCategory = FObjectEditorUtils::GetCategoryText(Function);

			if (!FunctionCategory.IsEmpty()) {
				categoryNames.AddUnique(FunctionCategory.ToString());
			}
		}
	}

	categoryNames.Sort();
	return categoryNames;
}

TArray<FText> FElgKEWUtils::GetCategoryTexts(UBlueprint* InBlueprint, const bool bShowUserVarsOnly/* = true*/)
{
	TArray<FText> CategorySource;
	for (const FString& catName : GetCategoryTextsAsString(InBlueprint, bShowUserVarsOnly) ) {
		CategorySource.Add(FText::FromString(catName));
	}
	return CategorySource;
}

EBPElgKEWPropertyFlags FElgKEWUtils::ConvertFromEPropertyFlags(EPropertyFlags InFlag)
{
	switch (InFlag) {
	case EPropertyFlags::CPF_None:							return EBPElgKEWPropertyFlags::CPF_None;
	case EPropertyFlags::CPF_Edit:							return EBPElgKEWPropertyFlags::CPF_Edit;
	case EPropertyFlags::CPF_ConstParm:						return EBPElgKEWPropertyFlags::CPF_ConstParm;
	case EPropertyFlags::CPF_BlueprintVisible:				return EBPElgKEWPropertyFlags::CPF_BlueprintVisible;
	case EPropertyFlags::CPF_ExportObject:					return EBPElgKEWPropertyFlags::CPF_ExportObject;
	case EPropertyFlags::CPF_BlueprintReadOnly:				return EBPElgKEWPropertyFlags::CPF_BlueprintReadOnly;
	case EPropertyFlags::CPF_Net:							return EBPElgKEWPropertyFlags::CPF_Net;
	case EPropertyFlags::CPF_EditFixedSize:					return EBPElgKEWPropertyFlags::CPF_EditFixedSize;
	case EPropertyFlags::CPF_Parm:							return EBPElgKEWPropertyFlags::CPF_Parm;
	case EPropertyFlags::CPF_OutParm:						return EBPElgKEWPropertyFlags::CPF_OutParm;
	case EPropertyFlags::CPF_ZeroConstructor:				return EBPElgKEWPropertyFlags::CPF_ZeroConstructor;
	case EPropertyFlags::CPF_ReturnParm:					return EBPElgKEWPropertyFlags::CPF_ReturnParm;
	case EPropertyFlags::CPF_DisableEditOnTemplate:			return EBPElgKEWPropertyFlags::CPF_DisableEditOnTemplate;
	case EPropertyFlags::CPF_Transient:						return EBPElgKEWPropertyFlags::CPF_Transient;
	case EPropertyFlags::CPF_Config:						return EBPElgKEWPropertyFlags::CPF_Config;
	case EPropertyFlags::CPF_DisableEditOnInstance:			return EBPElgKEWPropertyFlags::CPF_DisableEditOnInstance;
	case EPropertyFlags::CPF_EditConst:						return EBPElgKEWPropertyFlags::CPF_EditConst;
	case EPropertyFlags::CPF_GlobalConfig:					return EBPElgKEWPropertyFlags::CPF_GlobalConfig;
	case EPropertyFlags::CPF_InstancedReference:			return EBPElgKEWPropertyFlags::CPF_InstancedReference;
	case EPropertyFlags::CPF_DuplicateTransient:			return EBPElgKEWPropertyFlags::CPF_DuplicateTransient;
	case EPropertyFlags::CPF_SaveGame:						return EBPElgKEWPropertyFlags::CPF_SaveGame;
	case EPropertyFlags::CPF_NoClear:						return EBPElgKEWPropertyFlags::CPF_NoClear;
	case EPropertyFlags::CPF_ReferenceParm:					return EBPElgKEWPropertyFlags::CPF_ReferenceParm;
	case EPropertyFlags::CPF_BlueprintAssignable:			return EBPElgKEWPropertyFlags::CPF_BlueprintAssignable;
	case EPropertyFlags::CPF_Deprecated:					return EBPElgKEWPropertyFlags::CPF_Deprecated;
	case EPropertyFlags::CPF_IsPlainOldData:				return EBPElgKEWPropertyFlags::CPF_IsPlainOldData;
	case EPropertyFlags::CPF_RepSkip:						return EBPElgKEWPropertyFlags::CPF_RepSkip;
	case EPropertyFlags::CPF_RepNotify:						return EBPElgKEWPropertyFlags::CPF_RepNotify;
	case EPropertyFlags::CPF_Interp:						return EBPElgKEWPropertyFlags::CPF_Interp;
	case EPropertyFlags::CPF_NonTransactional:				return EBPElgKEWPropertyFlags::CPF_NonTransactional;
	case EPropertyFlags::CPF_EditorOnly:					return EBPElgKEWPropertyFlags::CPF_EditorOnly;
	case EPropertyFlags::CPF_NoDestructor:					return EBPElgKEWPropertyFlags::CPF_NoDestructor;
	case EPropertyFlags::CPF_AutoWeak:						return EBPElgKEWPropertyFlags::CPF_AutoWeak;
	case EPropertyFlags::CPF_ContainsInstancedReference:	return EBPElgKEWPropertyFlags::CPF_ContainsInstancedReference;
	case EPropertyFlags::CPF_AssetRegistrySearchable:		return EBPElgKEWPropertyFlags::CPF_AssetRegistrySearchable;
	case EPropertyFlags::CPF_SimpleDisplay:					return EBPElgKEWPropertyFlags::CPF_SimpleDisplay;
	case EPropertyFlags::CPF_AdvancedDisplay:				return EBPElgKEWPropertyFlags::CPF_AdvancedDisplay;
	case EPropertyFlags::CPF_Protected:						return EBPElgKEWPropertyFlags::CPF_Protected;
	case EPropertyFlags::CPF_BlueprintCallable:				return EBPElgKEWPropertyFlags::CPF_BlueprintCallable;
	case EPropertyFlags::CPF_BlueprintAuthorityOnly:		return EBPElgKEWPropertyFlags::CPF_BlueprintAuthorityOnly;
	case EPropertyFlags::CPF_TextExportTransient:			return EBPElgKEWPropertyFlags::CPF_TextExportTransient;
	case EPropertyFlags::CPF_NonPIEDuplicateTransient:		return EBPElgKEWPropertyFlags::CPF_NonPIEDuplicateTransient;
	case EPropertyFlags::CPF_ExposeOnSpawn:					return EBPElgKEWPropertyFlags::CPF_ExposeOnSpawn;
	case EPropertyFlags::CPF_PersistentInstance:			return EBPElgKEWPropertyFlags::CPF_PersistentInstance;
	case EPropertyFlags::CPF_UObjectWrapper:				return EBPElgKEWPropertyFlags::CPF_UObjectWrapper;
	case EPropertyFlags::CPF_HasGetValueTypeHash:			return EBPElgKEWPropertyFlags::CPF_HasGetValueTypeHash;
	case EPropertyFlags::CPF_NativeAccessSpecifierPublic:	return EBPElgKEWPropertyFlags::CPF_NativeAccessSpecifierPublic;
	case EPropertyFlags::CPF_NativeAccessSpecifierProtected:return EBPElgKEWPropertyFlags::CPF_NativeAccessSpecifierProtected;
	case EPropertyFlags::CPF_NativeAccessSpecifierPrivate:	return EBPElgKEWPropertyFlags::CPF_NativeAccessSpecifierPrivate;
	case EPropertyFlags::CPF_SkipSerialization:				return EBPElgKEWPropertyFlags::CPF_SkipSerialization;
	}
	return EBPElgKEWPropertyFlags::CPF_None;
}

EPropertyFlags FElgKEWUtils::ConvertToEPropertyFlags(EBPElgKEWPropertyFlags InFlag)
{
	switch (InFlag) {
	case EBPElgKEWPropertyFlags::CPF_None:							return EPropertyFlags::CPF_None;
	case EBPElgKEWPropertyFlags::CPF_Edit:							return EPropertyFlags::CPF_Edit;
	case EBPElgKEWPropertyFlags::CPF_ConstParm:						return EPropertyFlags::CPF_ConstParm;
	case EBPElgKEWPropertyFlags::CPF_BlueprintVisible:				return EPropertyFlags::CPF_BlueprintVisible;
	case EBPElgKEWPropertyFlags::CPF_ExportObject:					return EPropertyFlags::CPF_ExportObject;
	case EBPElgKEWPropertyFlags::CPF_BlueprintReadOnly:				return EPropertyFlags::CPF_BlueprintReadOnly;
	case EBPElgKEWPropertyFlags::CPF_Net:							return EPropertyFlags::CPF_Net;
	case EBPElgKEWPropertyFlags::CPF_EditFixedSize:					return EPropertyFlags::CPF_EditFixedSize;
	case EBPElgKEWPropertyFlags::CPF_Parm:							return EPropertyFlags::CPF_Parm;
	case EBPElgKEWPropertyFlags::CPF_OutParm:						return EPropertyFlags::CPF_OutParm;
	case EBPElgKEWPropertyFlags::CPF_ZeroConstructor:				return EPropertyFlags::CPF_ZeroConstructor;
	case EBPElgKEWPropertyFlags::CPF_ReturnParm:					return EPropertyFlags::CPF_ReturnParm;
	case EBPElgKEWPropertyFlags::CPF_DisableEditOnTemplate:			return EPropertyFlags::CPF_DisableEditOnTemplate;
	case EBPElgKEWPropertyFlags::CPF_Transient:						return EPropertyFlags::CPF_Transient;
	case EBPElgKEWPropertyFlags::CPF_Config:						return EPropertyFlags::CPF_Config;
	case EBPElgKEWPropertyFlags::CPF_DisableEditOnInstance:			return EPropertyFlags::CPF_DisableEditOnInstance;
	case EBPElgKEWPropertyFlags::CPF_EditConst:						return EPropertyFlags::CPF_EditConst;
	case EBPElgKEWPropertyFlags::CPF_GlobalConfig:					return EPropertyFlags::CPF_GlobalConfig;
	case EBPElgKEWPropertyFlags::CPF_InstancedReference:			return EPropertyFlags::CPF_InstancedReference;
	case EBPElgKEWPropertyFlags::CPF_DuplicateTransient:			return EPropertyFlags::CPF_DuplicateTransient;
	case EBPElgKEWPropertyFlags::CPF_SaveGame:						return EPropertyFlags::CPF_SaveGame;
	case EBPElgKEWPropertyFlags::CPF_NoClear:						return EPropertyFlags::CPF_NoClear;
	case EBPElgKEWPropertyFlags::CPF_ReferenceParm:					return EPropertyFlags::CPF_ReferenceParm;
	case EBPElgKEWPropertyFlags::CPF_BlueprintAssignable:			return EPropertyFlags::CPF_BlueprintAssignable;
	case EBPElgKEWPropertyFlags::CPF_Deprecated:					return EPropertyFlags::CPF_Deprecated;
	case EBPElgKEWPropertyFlags::CPF_IsPlainOldData:				return EPropertyFlags::CPF_IsPlainOldData;
	case EBPElgKEWPropertyFlags::CPF_RepSkip:						return EPropertyFlags::CPF_RepSkip;
	case EBPElgKEWPropertyFlags::CPF_RepNotify:						return EPropertyFlags::CPF_RepNotify;
	case EBPElgKEWPropertyFlags::CPF_Interp:						return EPropertyFlags::CPF_Interp;
	case EBPElgKEWPropertyFlags::CPF_NonTransactional:				return EPropertyFlags::CPF_NonTransactional;
	case EBPElgKEWPropertyFlags::CPF_EditorOnly:					return EPropertyFlags::CPF_EditorOnly;
	case EBPElgKEWPropertyFlags::CPF_NoDestructor:					return EPropertyFlags::CPF_NoDestructor;
	case EBPElgKEWPropertyFlags::CPF_AutoWeak:						return EPropertyFlags::CPF_AutoWeak;
	case EBPElgKEWPropertyFlags::CPF_ContainsInstancedReference:	return EPropertyFlags::CPF_ContainsInstancedReference;
	case EBPElgKEWPropertyFlags::CPF_AssetRegistrySearchable:		return EPropertyFlags::CPF_AssetRegistrySearchable;
	case EBPElgKEWPropertyFlags::CPF_SimpleDisplay:					return EPropertyFlags::CPF_SimpleDisplay;
	case EBPElgKEWPropertyFlags::CPF_AdvancedDisplay:				return EPropertyFlags::CPF_AdvancedDisplay;
	case EBPElgKEWPropertyFlags::CPF_Protected:						return EPropertyFlags::CPF_Protected;
	case EBPElgKEWPropertyFlags::CPF_BlueprintCallable:				return EPropertyFlags::CPF_BlueprintCallable;
	case EBPElgKEWPropertyFlags::CPF_BlueprintAuthorityOnly:		return EPropertyFlags::CPF_BlueprintAuthorityOnly;
	case EBPElgKEWPropertyFlags::CPF_TextExportTransient:			return EPropertyFlags::CPF_TextExportTransient;
	case EBPElgKEWPropertyFlags::CPF_NonPIEDuplicateTransient:		return EPropertyFlags::CPF_NonPIEDuplicateTransient;
	case EBPElgKEWPropertyFlags::CPF_ExposeOnSpawn:					return EPropertyFlags::CPF_ExposeOnSpawn;
	case EBPElgKEWPropertyFlags::CPF_PersistentInstance:			return EPropertyFlags::CPF_PersistentInstance;
	case EBPElgKEWPropertyFlags::CPF_UObjectWrapper:				return EPropertyFlags::CPF_UObjectWrapper;
	case EBPElgKEWPropertyFlags::CPF_HasGetValueTypeHash:			return EPropertyFlags::CPF_HasGetValueTypeHash;
	case EBPElgKEWPropertyFlags::CPF_NativeAccessSpecifierPublic:	return EPropertyFlags::CPF_NativeAccessSpecifierPublic;
	case EBPElgKEWPropertyFlags::CPF_NativeAccessSpecifierProtected:return EPropertyFlags::CPF_NativeAccessSpecifierProtected;
	case EBPElgKEWPropertyFlags::CPF_NativeAccessSpecifierPrivate:	return EPropertyFlags::CPF_NativeAccessSpecifierPrivate;
	case EBPElgKEWPropertyFlags::CPF_SkipSerialization:				return EPropertyFlags::CPF_SkipSerialization;
	}
	return EPropertyFlags::CPF_None;
}


EGraphType FElgKEWUtils::GetGraphType(UEdGraph* InGraph)
{
	return InGraph->GetSchema()->GetGraphType(InGraph);
}

bool FElgKEWUtils::IsGraphAFunction(UEdGraph* InGraph)
{
	if (!InGraph) return false;
	EGraphType graphType = InGraph->GetSchema()->GetGraphType(InGraph);
	if (graphType == EGraphType::GT_Function) return true;
	return false;
}


TArray<FBPVariableDescription> FElgKEWUtils::GetLocalVariableDescriptions(UEdGraph* InGraph)
{
	TArray<FBPVariableDescription> varDesc;
	TArray<UK2Node_FunctionEntry*> functionEntryNodes;
	InGraph->GetNodesOfClass(functionEntryNodes);
	for (UK2Node_FunctionEntry* node : functionEntryNodes) {
		for (FBPVariableDescription var : node->LocalVariables) {
			varDesc.Add(var);
		}
	}
	return varDesc;
}


TArray<UElgBESGraphVariableLocal*> FElgKEWUtils::GetLocalVariables(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	TArray<UElgBESGraphVariableLocal*> outVars;
	if (!IsGraphAFunction(InGraph)) return outVars;

	TArray<UK2Node_FunctionEntry*> functionEntryNodes;
	InGraph->GetNodesOfClass(functionEntryNodes);
	for (UK2Node_FunctionEntry* node : functionEntryNodes) {
		for (FBPVariableDescription var : node->LocalVariables) {
			outVars.Add(UElgBESGraphVariableLocal::MakeGraphLocalVariableObject(InBlueprint, InGraph, node, var));
		}
	}
	outVars.Sort([&](const UElgBESGraphVariableLocal& dataA, const UElgBESGraphVariableLocal& dataB) {
		return dataA.VariableName.ToString() < dataB.VariableName.ToString();
		});
	return outVars;
}


UElgBESGraphFunctionPin* FElgKEWUtils::GetInputPinVariable(UBlueprint* InBlueprint, UEdGraph* InGraph, const FName InName)
{
	UElgBESGraphFunctionPin* outVar = nullptr;
	if (!IsGraphAFunction(InGraph)) return outVar;
	UK2Node_FunctionEntry* entryNode;
	TArray<UK2Node_FunctionEntry*> EntryNodes;
	InGraph->GetNodesOfClass(EntryNodes);
	if (EntryNodes.Num() == 1) {
		entryNode = EntryNodes[0];
		if (entryNode) {
			for (UEdGraphPin* pin : entryNode->Pins) {
				if (NameToPinCategoryEnum(pin->PinType.PinCategory) == EBPElgKEWPinCategory::PC_Exec) continue;
				if (pin->PinName == InName) {
					return UElgBESGraphFunctionPin::MakeGraphPinVariable(InBlueprint, InGraph, entryNode, pin);
				}
			}
		}
	}
	return outVar;
}

TArray<UElgBESGraphFunctionPin*> FElgKEWUtils::GetInputPinVariables(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	TArray<UElgBESGraphFunctionPin*> outVars;
	if (!IsGraphAFunction(InGraph)) return outVars;
	if (UK2Node_FunctionEntry* entryNode = GetFunctionEntryNodeFromGraph(InGraph)) {
		for (UEdGraphPin* pin : entryNode->Pins) {
			if (NameToPinCategoryEnum(pin->PinType.PinCategory) == EBPElgKEWPinCategory::PC_Exec) continue;
			outVars.Add(UElgBESGraphFunctionPin::MakeGraphPinVariable(InBlueprint, InGraph, entryNode, pin));
		}
	}
	return outVars;
}

TArray<UElgBESGraphFunctionPin*> FElgKEWUtils::GetOutputPinVariables(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	TArray<UElgBESGraphFunctionPin*> outVars;
	if (!IsGraphAFunction(InGraph)) return outVars;
	UK2Node_FunctionResult* entryNode;
	TArray<UK2Node_FunctionResult*> EntryNodes;
	InGraph->GetNodesOfClass(EntryNodes);
	if (EntryNodes.Num() == 1) {
		entryNode = EntryNodes[0];
		if (entryNode) {
			for (UEdGraphPin* pin : entryNode->Pins) {
				if (NameToPinCategoryEnum(pin->PinType.PinCategory) == EBPElgKEWPinCategory::PC_Exec) continue;
				outVars.Add(UElgBESGraphFunctionPin::MakeGraphPinVariable(InBlueprint, InGraph, entryNode, pin));
			}
		}
	}
	return outVars;
}

UElgBESGraphFunctionPin* FElgKEWUtils::GetOutputPinVariable(UBlueprint* InBlueprint, UEdGraph* InGraph, const FName InName)
{
	UElgBESGraphFunctionPin* outVar = nullptr;
	if (!IsGraphAFunction(InGraph)) return outVar;
	UK2Node_FunctionResult* entryNode;
	TArray<UK2Node_FunctionResult*> EntryNodes;
	InGraph->GetNodesOfClass(EntryNodes);
	if (EntryNodes.Num() == 1) {
		entryNode = EntryNodes[0];
		if (entryNode) {
			for (UEdGraphPin* pin : entryNode->Pins) {
				if (NameToPinCategoryEnum(pin->PinType.PinCategory) == EBPElgKEWPinCategory::PC_Exec) continue;
				if (pin->PinName == InName) {
					return UElgBESGraphFunctionPin::MakeGraphPinVariable(InBlueprint, InGraph, entryNode, pin);
				}
			}
		}
	}
	return outVar;
}

void FElgKEWUtils::GetVariableIconFromPinType(const FEdGraphPinType InPinType, FSlateBrush& OutBrush)
{
	
	FLinearColor tint = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	FSlateIcon icon = UK2Node_Variable::GetVarIconFromPinType(InPinType, tint);
	const FSlateBrush* iconBrush = FBlueprintEditorUtils::GetIconFromPin(InPinType, false);
	//OutBrush = *icon.GetOptionalIcon();
	OutBrush = *iconBrush;
	OutBrush.TintColor = tint;
}


UFunction* FElgKEWUtils::FindFunctionInBlueprint(UBlueprint* InBlueprint, const FName InFunctionName)
{
	return FindUField<UFunction>(InBlueprint->SkeletonGeneratedClass, InFunctionName);
}


FProperty* FElgKEWUtils::GetFunctionProperty(UFunction* InFunction, const FName InVariableName)
{
	if (UStruct* owningScope = Cast<UStruct>(InFunction)) {
		if (FProperty* variableProperty = FindFProperty<FProperty>(owningScope, InVariableName)) {
			return variableProperty;
		}
	}
	return nullptr;
}




bool FElgKEWUtils::NameValidator(UBlueprint* InBlueprint, const FName InOldName, const FName InNewName, FString& OutErrorMessage, UFunction* InFunction /*= NULL*/)
{
	UStruct* scopeLocalVariable;
	if (InFunction != NULL) {
		scopeLocalVariable = Cast<UStruct>(InFunction);
	} else {
		scopeLocalVariable = NULL;		
	}
	TSharedPtr<INameValidatorInterface> NameValidator = MakeShareable(new FKismetNameValidator(InBlueprint, InOldName, scopeLocalVariable));
	EValidatorResult ValidatorResult = NameValidator->IsValid(InNewName.ToString());
	
	switch (ValidatorResult) {
	case EValidatorResult::AlreadyInUse: 
		OutErrorMessage = FString::Printf(TEXT("%s is in use by another variable or function!"), *InNewName.ToString()); 
		return false;
	case  EValidatorResult::EmptyName:
		OutErrorMessage = FString(TEXT("Names cannot be left blank!"));
		return false;
	case EValidatorResult::TooLong:
		OutErrorMessage = FString::Printf(TEXT("Names must have fewer than %s characters!"), *FString::FromInt(FKismetNameValidator::GetMaximumNameLength()));
		return false;
	case EValidatorResult::LocallyInUse:
		OutErrorMessage = FString(TEXT("Conflicts with another local variable or function parameter!"));
		return false;
	}

	OutErrorMessage = TEXT("Success!");
	return true;
}



UFunction* FElgKEWUtils::GetLocalVaribleScope(UFunction* InFunction, const FName InName)
{
	if (FProperty* variableProperty = FElgKEWUtils::GetFunctionProperty(InFunction, InName)) {
		return variableProperty->GetOwner<UFunction>();
	}
	return NULL;
}


FName FElgKEWUtils::AddEditablePin(UBlueprint* InBlueprint, UK2Node_EditablePinBase* InEntry, const FName InName, FS_ElgGraphPinType InType, const FString InDefaultValue, EEdGraphPinDirection InPinDirection)
{
	FName newPinName;
	FEdGraphPinType pinType = ConvertFromPinType(InType);
	FScopedTransaction Transaction(LOCTEXT("AddInParam", "Add In Parameter"));
	InEntry->Modify();
	if (UEdGraphPin* newPin = InEntry->CreateUserDefinedPin(InName, pinType, InPinDirection)) {
		newPinName = newPin->PinName; // the name of the pin might have changed
		EditablePinChanged(InEntry);
		if (InPinDirection == EGPD_Output && !InDefaultValue.Equals("")) {
			for (UEdGraphPin* pin : InEntry->Pins) {
				if (pin->PinName == newPinName) {
					pin->DefaultValue = InDefaultValue;
					break;
				}
			}			
		}
	} else {
		Transaction.Cancel();
	}	
	return newPinName;
}


void FElgKEWUtils::RemoveEditablePin(UBlueprint* InBlueprint, UK2Node_EditablePinBase* InEntry, const FName InName)
{
	const FScopedTransaction Transaction(LOCTEXT("RemoveParam", "Remove Parameter"));
	InBlueprint->Modify();
	InEntry->Modify();
	InEntry->RemoveUserDefinedPinByName(InName);
	EditablePinChanged(InEntry);
}


void FElgKEWUtils::ChangeEditablePinType(UK2Node_EditablePinBase* InEntry, const FName InPinName, FS_ElgGraphPinType InNewType)
{
	const FScopedTransaction Transaction(LOCTEXT("ChangeParam", "Change Parameter Type"));
	InEntry->Modify();

	FEdGraphPinType pinType = ConvertFromPinType(InNewType);
	TSharedPtr<FUserPinInfo>* UDPinPtr = InEntry->UserDefinedPins.FindByPredicate([InPinName](TSharedPtr<FUserPinInfo>& UDPin)
	{
			return UDPin.IsValid() && (UDPin->PinName == InPinName);
	});

	if (UDPinPtr) {
		InEntry->Modify();
		(*UDPinPtr)->PinType = pinType;
		if (!pinType.bIsConst && InEntry->ShouldUseConstRefParams()) {
			(*UDPinPtr)->PinType.bIsConst = pinType.IsArray() || pinType.bIsReference;
		}
		(*UDPinPtr)->PinDefaultValue.Reset();
	}
	EditablePinChanged(InEntry);
}



FName FElgKEWUtils::ChangeEditablePinName(UK2Node_EditablePinBase* InEntry, const FName InPinName, const FName InNewName)
{
	const FScopedTransaction Transaction(LOCTEXT("ChangeParamName", "Change Parameter Name"));
	InEntry->Modify();

	FName pinName = InEntry->CreateUniquePinName(InNewName);

	TSharedPtr<FUserPinInfo>* UDPinPtr = InEntry->UserDefinedPins.FindByPredicate([InPinName](TSharedPtr<FUserPinInfo>& UDPin)
		{
			return UDPin.IsValid() && (UDPin->PinName == InPinName);
		});

	if (UDPinPtr) {
		InEntry->Modify();
		(*UDPinPtr)->PinName = pinName;
	}
	EditablePinChanged(InEntry);
	return pinName;
}


void FElgKEWUtils::EditablePinChanged(UK2Node_EditablePinBase* InEntry)
{
	const bool bCurDisableOrphanSaving = InEntry->bDisableOrphanPinSaving;
	InEntry->bDisableOrphanPinSaving = true;
	InEntry->ReconstructNode();
	InEntry->bDisableOrphanPinSaving = bCurDisableOrphanSaving;
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();
	K2Schema->HandleParameterDefaultValueChanged(InEntry);
}


bool FElgKEWUtils::MoveEditablePinDown(UK2Node_EditablePinBase* InNode, const int32 InCurrentIndex)
{
	const int32 NewParamIndex = InCurrentIndex + 1;
	if (InCurrentIndex != INDEX_NONE && NewParamIndex < InNode->UserDefinedPins.Num()) {
		const FScopedTransaction Transaction(LOCTEXT("MovePinDown", "Move Pin Down"));
		TArray<UK2Node_EditablePinBase*> TargetNodes = GatherAllResultNodes(InNode);
		for (UK2Node_EditablePinBase* Node : TargetNodes) {
			Node->Modify();
			Node->UserDefinedPins.Swap(InCurrentIndex, NewParamIndex);
			EditablePinChanged(Node);
		}
		return true;
	}
	return false;
}

bool FElgKEWUtils::MoveEditablePinUp(UK2Node_EditablePinBase* InNode, const int32 InCurrentIndex)
{
	const int32 NewParamIndex = InCurrentIndex - 1;
	if (InCurrentIndex != INDEX_NONE && NewParamIndex >= 0) {
		const FScopedTransaction Transaction(LOCTEXT("MovePinUp", "Move Pin Up"));
		TArray<UK2Node_EditablePinBase*> TargetNodes = GatherAllResultNodes(InNode);
		for (UK2Node_EditablePinBase* Node : TargetNodes) {
			Node->Modify();
			Node->UserDefinedPins.Swap(InCurrentIndex, NewParamIndex);
			EditablePinChanged(Node);
		}
		return true;
	}
	return false;
}


bool FElgKEWUtils::HasEditablePin(UK2Node_EditablePinBase* InNode, const FName InPinName)
{
	return InNode->UserDefinedPins.ContainsByPredicate([&InPinName](const TSharedPtr<FUserPinInfo>& UDPin)
		{
			return UDPin.IsValid() && (UDPin->PinName == InPinName);
		});

}


TArray<UK2Node_EditablePinBase*> FElgKEWUtils::GatherAllResultNodes(UK2Node_EditablePinBase* TargetNode)
{
	if (UK2Node_FunctionResult* ResultNode = Cast<UK2Node_FunctionResult>(TargetNode)) {
		return (TArray<UK2Node_EditablePinBase*>)ResultNode->GetAllResultNodes();
	} 
	TArray<UK2Node_EditablePinBase*> Result;
	if (TargetNode) {
		Result.Add(TargetNode);
	}
	return Result;
}


FS_ElgFunctionInfo FElgKEWUtils::CreateFunctionInfo(const UFunction* InFunction)
{	
	FS_ElgFunctionInfo info;
	if (!InFunction) return info;

	UClass* funcClass = CastChecked<UClass>(InFunction->GetOuter());
	info.FunctionName = InFunction->GetFName();
	info.ClassName = funcClass->GetFName();
	info.ToolTip = UK2Node_CallFunction::GetDefaultTooltipForFunction(InFunction);
	info.Description = GetDefault<UEdGraphSchema_K2>()->GetFriendlySignatureName(InFunction).ToString();
	if (info.Description.IsEmpty()) {
		info.Description = InFunction->GetName();
	}
	if (InFunction->HasMetaData(FBlueprintMetadata::MD_DeprecatedFunction)) {
		FText desc = FText::FromString(info.Description);
		info.Description = FBlueprintEditorUtils::GetDeprecatedMemberMenuItemName(desc).ToString();
	}
	info.Category = FObjectEditorUtils::GetCategoryText(InFunction).ToString();
	return info;
}


EBPElgKEWWFunctionType FElgKEWUtils::GetFuctionType(const UEdGraph* InFunctionGraph)
{
	EBPElgKEWWFunctionType functionType = EBPElgKEWWFunctionType::Function;
	if (InFunctionGraph) {
		if (UBlueprint* blueprint = Cast<UBlueprint>(InFunctionGraph->GetOuter())) {
			for (UEdGraph* graph : blueprint->FunctionGraphs) {
				if (graph == InFunctionGraph) {
					return EBPElgKEWWFunctionType::Function;
				}
			}

			for (UEdGraph* graph : blueprint->MacroGraphs) {
				if (graph == InFunctionGraph) {
					return EBPElgKEWWFunctionType::Macro;
				}
			}
				
			// interface



		}
	}	
	return functionType;
}


FName FElgKEWUtils::GetInterfaceClassName(const UClass* InClass)
{
	if (InClass) {
		if (InClass->ClassGeneratedBy) {
			return InClass->ClassGeneratedBy->GetFName();
		}
		else {
			return InClass->GetFName();
		}
	}
	return NAME_None;
}


bool FElgKEWUtils::AddOverridableFunction(UBlueprint* InBlueprint, const FName InFunctionName)
{
	if (InFunctionName.IsNone()) return false;
	FBlueprintEditorUtils::ConformImplementedInterfaces(InBlueprint);
	UFunction* overrideFunc = nullptr;
	UClass* const overrideFuncClass = FBlueprintEditorUtils::GetOverrideFunctionClass(InBlueprint, InFunctionName, &overrideFunc);

	if (!overrideFunc) {
		UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("AddOverridableFunction: Failed to find any function [%s] to override"), *InFunctionName.ToString());
		return false;
	}

	UEdGraph* eventGraph = FBlueprintEditorUtils::FindEventGraph(InBlueprint);
	TSet<FName> graphNames;
	FBlueprintEditorUtils::GetAllGraphNames(InBlueprint, graphNames);
	if (UEdGraphSchema_K2::FunctionCanBePlacedAsEvent(overrideFunc) && !graphNames.Contains(InFunctionName) && eventGraph) {
		UK2Node_Event* existingNode = FBlueprintEditorUtils::FindOverrideForFunction(InBlueprint, overrideFuncClass, InFunctionName);
		if (existingNode) {
			FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(existingNode);
			return true;
		}
		else {
			UK2Node_Event* newEventNode = FEdGraphSchemaAction_K2NewNode::SpawnNode<UK2Node_Event>(
				eventGraph,
				eventGraph->GetGoodPlaceForNewNode(),
				EK2NewNodeFlags::SelectNewNode,
				[InFunctionName, overrideFuncClass](UK2Node_Event* newInstance)
				{
					newInstance->EventReference.SetExternalMember(InFunctionName, overrideFuncClass);
					newInstance->bOverrideFunction = true;
				}
			);
			if (newEventNode) {
				FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(newEventNode);
				return true;
			}
		}
	}
	else {
		UEdGraph* const existingGraph = FindObject<UEdGraph>(InBlueprint, *InFunctionName.ToString());
		if (existingGraph)
		{
			FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(existingGraph);
			return true;
		} else {
			const FScopedTransaction Transaction(LOCTEXT("CreateOverrideFunctionGraph", "Create Override Function Graph"));
			InBlueprint->Modify();
			// Implement the function graph
			UEdGraph* const newGraph = FBlueprintEditorUtils::CreateNewGraph(InBlueprint, InFunctionName, UEdGraph::StaticClass(), UEdGraphSchema_K2::StaticClass());
			FBlueprintEditorUtils::AddFunctionGraph(InBlueprint, newGraph, /*bIsUserCreated=*/ false, overrideFuncClass);
			newGraph->Modify();
			return true;
		}
	}
	return false;
}


bool FElgKEWUtils::IsNodeConnected(UEdGraphNode* InNode)
{
	check(InNode);
	bool bHasExec = false;
	bool bHasInput = false;
	bool bConnectedIn = false;
	bool bConnectedFrom = false;
	bool bConnectedFromData = false;

	for (UEdGraphPin* pin : InNode->Pins) {
		if (pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Exec) {
			bHasExec = true;
			if (pin->Direction == EEdGraphPinDirection::EGPD_Input) {
				bHasInput = true;
				if (pin->LinkedTo.Num()) {
					bConnectedIn = true;
				}
			} else {
				if (pin->LinkedTo.Num()) {
					bConnectedFrom = true;
				}
			}
		} else if (pin->Direction == EEdGraphPinDirection::EGPD_Output) {  // if its not a exec output pin
			if (pin->LinkedTo.Num()) {
				bConnectedFromData = true;
			}
		}
	}
	// no exec pins but we have a data connected so its ok or ...
	//  if the node dont have any input exec pins, like Event Tick, 
	//  but has output connection then its fine anyway
	if (!bHasExec && bConnectedFromData || !bHasInput && bConnectedFrom) {
		bConnectedIn = true;
	}
	return bConnectedIn;
}



FName FElgKEWUtils::DuplicateVariable(UBlueprint* InBlueprint, FProperty* InVariableProperty, const bool IsMember/*=true*/)
{
	check(InBlueprint);
	check(InVariableProperty);
	const FName varName = InVariableProperty->GetFName();
	FName newVarName;

	if (IsMember) {
		const FScopedTransaction Transaction(LOCTEXT("DuplicateVariable", "Duplicate Variable"));
		InBlueprint->Modify();

		newVarName = FBlueprintEditorUtils::DuplicateVariable(InBlueprint, nullptr, varName);
		if (newVarName == NAME_None) {
			FEdGraphPinType VarPinType;
			GetDefault<UEdGraphSchema_K2>()->ConvertPropertyToPinType(InVariableProperty, VarPinType);
			newVarName = FBlueprintEditorUtils::FindUniqueKismetName(InBlueprint, varName.ToString());
			FBlueprintEditorUtils::AddMemberVariable(InBlueprint, newVarName, VarPinType);
		}
	} else {
		if (UFunction* varScope = InVariableProperty->GetOwner<UFunction>()) {
			const FScopedTransaction Transaction(LOCTEXT("Duplicate Local Variable", "Duplicate Local Variable"));
			InBlueprint->Modify();
			
			newVarName = FBlueprintEditorUtils::DuplicateVariable(InBlueprint, varScope, varName);
		}
	}

	return newVarName;
}


UEdGraph* FElgKEWUtils::DuplcateGraph(UBlueprint* InBlueprint, UEdGraph* InGraph)
{
	check(InGraph);
	// Only function, anim graph and macro duplication is supported
	EGraphType graphType = InGraph->GetSchema()->GetGraphType(InGraph);	
	if (graphType == EGraphType::GT_MAX || graphType == EGraphType::GT_StateMachine || graphType == EGraphType::GT_Ubergraph) return nullptr;

	const FScopedTransaction Transaction(LOCTEXT("DuplicateGraph", "Duplicate Graph"));
	InBlueprint->Modify();

	UEdGraph* duplicatedGraph = InGraph->GetSchema()->DuplicateGraph(InGraph);
	check(duplicatedGraph);
	duplicatedGraph->Modify();

	for (UEdGraphNode* graphNode : duplicatedGraph->Nodes) {
		if (graphNode) {
			graphNode->CreateNewGuid();
			if (UK2Node_AddComponent* componentNode = Cast<UK2Node_AddComponent>(graphNode)) {
				componentNode->MakeNewComponentTemplate();
			}
		}
	}
	if (graphType == EGraphType::GT_Function || graphType == EGraphType::GT_Animation) {
		InBlueprint->FunctionGraphs.Add(duplicatedGraph);
	} else if (graphType == EGraphType::GT_Macro) {
		InBlueprint->MacroGraphs.Add(duplicatedGraph);
	}
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(InBlueprint);
	return duplicatedGraph;

}


void FElgKEWUtils::ChangeVariableNodeReference(UBlueprint* InBlueprint, UK2Node_Variable* InNodeToUpdate, const FName InNewVariableName, FEdGraphPinType InNewPinType, UStruct* InOwnerScope, const bool bIsInputPin/*=false*/)
{
	const FScopedTransaction Transaction(LOCTEXT("ReplacePinVariable", "Replace Pin Variable"));

	const FName OldVarName = InNodeToUpdate->GetVarName();	
	TArray<UEdGraphPin*> BadLinks;
	GetUnsupportedPinsForProperty(InNodeToUpdate, InNewPinType, BadLinks);
	UEdGraphPin* Pin = InNodeToUpdate->FindPin(OldVarName);

	InBlueprint->Modify();
	InNodeToUpdate->Modify();

	if (Pin != nullptr) Pin->Modify();

	if (bIsInputPin) {
		FGuid tempGuid;
		tempGuid.Invalidate();
		InNodeToUpdate->VariableReference.SetLocalMember(InNewVariableName, InOwnerScope, tempGuid);
	} else {
		// if the variable do not have a valid Guid and has a pin called self its a input pin variable and we need to
		// remove the self pin otherwise we will get compile errors.
		if (!InNodeToUpdate->VariableReference.GetMemberGuid().IsValid()) {
			if (UEdGraphPin* PinSelf = InNodeToUpdate->FindPin(TEXT("self"))) {
				InNodeToUpdate->RemovePin(PinSelf);
				InNodeToUpdate->ErrorMsg.Empty();
			}
		}
		UEdGraphSchema_K2::ConfigureVarNode(InNodeToUpdate, InNewVariableName, InOwnerScope, InBlueprint);
	}

	const UEdGraphSchema_K2* Schema = Cast<const UEdGraphSchema_K2>(InNodeToUpdate->GetSchema());
	if ((Pin == nullptr) || (Pin->LinkedTo.Num() == BadLinks.Num()) || (Schema == nullptr)) {
		InNodeToUpdate->GetSchema()->ReconstructNode(*InNodeToUpdate);
	} else {
		Pin->PinName = InNewVariableName;
		Pin->PinType = InNewPinType;
		for (TArray<class UEdGraphPin*>::TIterator OtherPinIt(BadLinks); OtherPinIt; ++OtherPinIt) {
			Pin->BreakLinkTo(*OtherPinIt);
		}
	}
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(InBlueprint);

}

void FElgKEWUtils::ChangeVariableNodeReferenceToMember(UBlueprint* InBlueprint, UK2Node_Variable* InNodeToUpdate, const FName InVariableName)
{
	check(InBlueprint);
	check(InNodeToUpdate);
	FProperty* varProperty = GetBlueprintProperty(InBlueprint, InVariableName);
	check(varProperty);

	const UEdGraphSchema_K2* Schema = Cast<const UEdGraphSchema_K2>(InNodeToUpdate->GetSchema());
	FEdGraphPinType NewPinType;
	Schema->ConvertPropertyToPinType(varProperty, NewPinType);
	ChangeVariableNodeReference(InBlueprint, InNodeToUpdate, InVariableName, NewPinType, varProperty->GetOwnerClass());

}


void FElgKEWUtils::ChangeVariableNodeReferenceToLocal(UBlueprint* InBlueprint, UK2Node_Variable* InNodeToUpdate, UFunction* InFunction, const FName InVariableName)
{
	check(InBlueprint);
	check(InNodeToUpdate);
	UFunction* varScope = GetLocalVaribleScope(InFunction, InVariableName);
	FProperty* varProperty = GetFunctionProperty(InFunction, InVariableName);
	check(varProperty);
	const UEdGraphSchema_K2* Schema = Cast<const UEdGraphSchema_K2>(InNodeToUpdate->GetSchema());
	FEdGraphPinType NewPinType;
	Schema->ConvertPropertyToPinType(varProperty, NewPinType);
	ChangeVariableNodeReference(InBlueprint, InNodeToUpdate, InVariableName, NewPinType, varScope);
}

void FElgKEWUtils::ChangeVariableNodeReferenceToPinVar(UBlueprint* InBlueprint, UEdGraph* InFunctionGraph, UK2Node_VariableGet* InNodeToUpdate, const FName InVariableName)
{
	check(InBlueprint);
	check(InNodeToUpdate);

	UElgBESGraphFunctionPin* pinVar = GetInputPinVariable(InBlueprint, InFunctionGraph, InVariableName);
	if (!pinVar) return;
	
	UFunction* functionToSpawn = FindUField<UFunction>(InBlueprint->SkeletonGeneratedClass, InFunctionGraph->GetFName());
	ChangeVariableNodeReference(InBlueprint, InNodeToUpdate, InVariableName, pinVar->VarType, functionToSpawn, true);
}


void FElgKEWUtils::GetUnsupportedPinsForProperty(UK2Node_Variable* InNode, FEdGraphPinType InNewPinType, TArray<class UEdGraphPin*>& OutBroken)
{
	if (const UEdGraphSchema_K2* Schema = Cast<const UEdGraphSchema_K2>(InNode->GetSchema())) {
		if (UEdGraphPin* Pin = InNode->FindPin(InNode->GetVarName())) {
			for (TArray<class UEdGraphPin*>::TIterator i(Pin->LinkedTo); i; i++) {
				UEdGraphPin* Link = *i;
				if (false == Schema->ArePinTypesCompatible(InNewPinType, Link->PinType)) {
					OutBroken.Add(Link);
				}
			}
		}
	}
}



void FElgKEWUtils::DropNodeOnPin(UK2Node* InNode, UEdGraphPin* InPin)
{
	UEdGraphNode* pinNode = InPin->GetOwningNode();
	if (InPin->Direction == EGPD_Output) {
		InNode->NodePosX += 48;
		InNode->NodePosY = pinNode->NodePosY;
	}
	else {
		int32 size = FMath::Clamp(InNode->GetNodeTitle(ENodeTitleType::FullTitle).ToString().Len() * 8, 48, 300);
		InNode->NodePosX -= size;
		InNode->NodePosY = pinNode->NodePosY;
	}
	InNode->SnapToGrid(GetDefault<UEditorStyleSettings>()->GridSnapSize);
	InNode->AutowireNewNode(InPin);
}


bool FElgKEWUtils::DropVariableOnPin(UK2Node_Variable* InNode, UEdGraphPin* InPin)
{
	check(InNode);
	check(InPin);
	InPin->Modify();

	if (!CanDropVariableOnPin(InNode, InPin)) return false;

	if (InPin->Direction == EGPD_Input) {
		const int32 NodeDistance = 160;
		int32 XLocation = InNode->NodePosX;

		UEdGraphNode* PinNode = InPin->GetOwningNode();
		const float XDelta = FMath::Abs(PinNode->NodePosX - XLocation);
		if (XDelta < NodeDistance) {
			XLocation = PinNode->NodePosX - NodeDistance;
		}
		InNode->NodePosX = XLocation;
		InNode->NodePosY += 16;

	} else {
		InNode->NodePosX += 48;
		InNode->NodePosY -= 48;
	}

	InNode->SnapToGrid(GetDefault<UEditorStyleSettings>()->GridSnapSize);
	InNode->AutowireNewNode(InPin);
	return true;
}


bool FElgKEWUtils::CanDropVariableOnPin(UK2Node_Variable* InNode, UEdGraphPin* InPin)
{
	check(InNode);
	check(InPin);

	UEdGraph* nodeGraph = InNode->GetGraph();
	const UEdGraphSchema_K2* schema = Cast<const UEdGraphSchema_K2>(nodeGraph->GetSchema());
	if (schema->IsExecPin(*InPin)) return false;

	FProperty* variableProperty = InNode->GetPropertyForVariable();
	FEdGraphPinType VariablePinType;
	schema->ConvertPropertyToPinType(variableProperty, VariablePinType);
	if (!schema->ArePinTypesCompatible(VariablePinType, InPin->PinType)) return false;
	
	return true;
}


bool FElgKEWUtils::CanDropPropertyOnPin(FProperty* InProperty, UEdGraphPin* InPin)
{
	const UEdGraphSchema_K2* schema = GetDefault<UEdGraphSchema_K2>();
	FEdGraphPinType VariablePinType;
	schema->ConvertPropertyToPinType(InProperty, VariablePinType);
	if (!schema->ArePinTypesCompatible(VariablePinType, InPin->PinType)) return false;
	return true;
}


bool FElgKEWUtils::AddEventDispatcher(UBlueprint* InBlueprint, const FName InName)
{
	check(InBlueprint);
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();
	check(NULL != K2Schema);

	const FScopedTransaction Transaction(LOCTEXT("AddNewDelegate", "Add New Event Dispatcher"));
	InBlueprint->Modify();

	FEdGraphPinType delegateType;
	delegateType.PinCategory = UEdGraphSchema_K2::PC_MCDelegate;
	const bool bVarCreatedSuccess = FBlueprintEditorUtils::AddMemberVariable(InBlueprint, InName, delegateType);
	if (!bVarCreatedSuccess) return false;

	UEdGraph* const newGraph = FBlueprintEditorUtils::CreateNewGraph(InBlueprint, InName, UEdGraph::StaticClass(), UEdGraphSchema_K2::StaticClass());
	if (!newGraph) {
		FBlueprintEditorUtils::RemoveMemberVariable(InBlueprint, InName);
		return false;
	}

	newGraph->bEditable = false;

	K2Schema->CreateDefaultNodesForGraph(*newGraph);
	K2Schema->CreateFunctionGraphTerminators(*newGraph, (UClass*)NULL);
	K2Schema->AddExtraFunctionFlags(newGraph, (FUNC_BlueprintCallable | FUNC_BlueprintEvent | FUNC_Public));
	K2Schema->MarkFunctionEntryAsEditable(newGraph, true);

	InBlueprint->DelegateSignatureGraphs.Add(newGraph);
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(InBlueprint);

	return true;
}

#undef LOCTEXT_NAMESPACE