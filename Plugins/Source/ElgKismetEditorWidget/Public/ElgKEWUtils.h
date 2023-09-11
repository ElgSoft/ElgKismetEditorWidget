// Copyright 2019-2023 ElgSoft. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "ElgKEWStructs.h"
#include <ElgKEWEnum.h>
#include <ElgBESGraphFunctionPin.h>

struct FKismetUserDeclaredFunctionMetadata;
class UFunction;
class UK2Node_EditablePinBase;
class UEdGraph;
class UBlueprint;
class UK2Node_FunctionEntry;
class SGraphPanel;
class UElgBESGraphVariableLocal;
class UK2Node_FunctionResult;
class UK2Node_MacroInstance;

/**
 * 
 */
class ELGKISMETEDITORWIDGET_API FElgKEWUtils
{
public:

	/* Get all graphs */
	static TArray<UEdGraph*> GetAllGraphs(UBlueprint* InBlueprint);

	/* Try to find the function by it's name, checks in the BPs GeneratedClass */
	static 	UFunction* FindFunction(UBlueprint* InBlueprint, FName InFunctionName);

	static int32 NodeCount(UBlueprint* InBlueprint);

	static TArray<UFunction*> GetAllFunctions(UBlueprint* InBlueprint);

	static bool DoFunctionHasFlag(UFunction* InFunction, EElgFunctionFlags InFlag);

	static bool IsInvalid(UObject* InObject, const FString& InMessage);

	static UEdGraph* GetFunctionGraph(UBlueprint* InBlueprint, UFunction* InFunction);
	static UEdGraph* GetMacroGraph(UBlueprint* InBlueprint, const FName InName);

	static UK2Node_FunctionEntry* GetFunctionEntryNodeFromGraph(UEdGraph* InGraph);
	static UK2Node_FunctionEntry* GetFunctionEntryNode(UBlueprint* InBlueprint, UFunction* InFunction);
	
	static UK2Node_FunctionResult* GetFunctionResultNodeFromGraph(UEdGraph* InGraph);


	static void UpdatedNodeParameters(UK2Node_EditablePinBase* InNode);

	static bool IsFunctionPure(UFunction* InFunction);
	
	/* Add a function node to a graph */
	static bool SpawnFunctionNodeInGraph(UBlueprint* InBlueprint, 
		UEdGraph* InGraph, 
		const FName InFunctionName, 
		const FVector2D InNodePosition,
		UEdGraphPin* InPin = nullptr
	);
	
	static bool SpawnMacroNodeInGraph(UBlueprint* InBlueprint,
		UEdGraph* InGraph,
		const FName InMacroName,
		const FVector2D InNodePosition,
		UEdGraphPin* InPin = nullptr
	);

	static bool SpawnVariableNodeInGraph(UBlueprint* InBlueprint,
		UEdGraph* InGraph,
		const FName InVariableName,
		const FVector2D InNodePosition,
		bool bGetter,
		UEdGraphPin* InPin = nullptr
	);

	static bool SpawnLocalVariableNodeInGraph(UBlueprint* InBlueprint,
		UEdGraph* InGraph,
		const FName InVariableName,
		const FVector2D InNodePosition,
		bool bGetter,
		UEdGraphPin* InPin = nullptr
	);
	static bool SpawnPinVariableNodeInGraph(UBlueprint* InBlueprint,
		UEdGraph* InGraph,
		const FName InVariableName,
		const FVector2D InNodePosition,
		UEdGraphPin* InPin = nullptr
	);
	static bool SpawnEventDispatcherNodeInGraph(UBlueprint* InBlueprint,
		UEdGraph* InGraph,
		const FName InVariableName,
		const FVector2D InNodePosition,
		const bool bInCall,
		UEdGraphPin* InPin = nullptr
	);


	static bool SpawnEventNodeInGraph(
		UBlueprint* InBlueprint,
		const FName InEventName,
		const FVector2D InNodePosition = FVector2D(0.0f, 0.0f)
	);


	/* Return true if the mouse is over a SGraphPanel, if the mouse is over a 
		node or something like that it will return false
	*/
	static bool IsMouseOverGraphEditor();

	static TSharedPtr<SWidget> GetSlateWidgetUnderMouse();
	static TArray<TSharedRef<SWidget>> GetSlateWidgetsUnderMouse();
	static FVector2D GetMousePosition();
	static FVector2D GetMouseViewportPosition();
	static FVector2D GetMousePositionInGraph(const TSharedPtr<SGraphPanel>& InGraphPanelWidget);
	static bool IsWidgetSGraphPanel(const TSharedPtr<SWidget>& InWidget);

	static UObject* GetEditorWidgetParent(UObject* InObject);

	static void GetNodeMemberName(UEdGraphNode* InNode, FString& OutName, FString& OutParentClass, FString& OutParentPath);

	/* Get the name of the class and function are used by the node */
	static void GetCallFunctionMemberName(UEdGraphNode* InNode, FString& OutName, FString& OutParentClass,  FString& OutParentPath);
	static FGuid GetCallFunctionMemberGuid(UEdGraphNode* InNode);


	/* return the name of the EBPElgKEWPinCategory */
	static FName PinCategoryEnumToName(EBPElgKEWPinCategory InPinCategory);
	static EBPElgKEWPinCategory NameToPinCategoryEnum(FName InPinCategory);
	static FEdGraphPinType PinCategoryToPinType(EBPElgKEWPinCategory InPinCategory);

	/* Convert FEdGraphPinType to BP pin type */
	static FS_ElgGraphPinType ConvertToPinType(FEdGraphPinType InPinType);
	static FEdGraphPinType ConvertFromPinType(const FS_ElgGraphPinType& InPinType);
	static EBPElgPinContainerType ConvertToPinContainerType(EPinContainerType InContainerType);
	static EPinContainerType ConvertFromPinContainerType(EBPElgPinContainerType InContainerType);
	static EBPElgEPinTypeFilter ConvertToPinTypeFilter(ETypeTreeFilter InPinFilter);
	static ETypeTreeFilter ConvertFromPinTypeFilter(EBPElgEPinTypeFilter InPinFilter);
	static FS_ElgGraphPinType GetDefaultPinType();

	static FString GetPropertyValueAsString(UBlueprint* InBlueprint, FProperty* InProperty);
	static void SetPropertyValueAsString(UBlueprint* InBlueprint, FProperty* InProperty, const FString& InValue);
	//static FString GetPropertyValueAsString(UBlueprint* InBlueprint, FProperty* InProperty);

	static void SetVariableCategory(UBlueprint* InBlueprint, const FName InVariableName, const FText& InNewCategory);
	static void RemoveVariable(UBlueprint* InBlueprint, const FName InVariableName);

	static FProperty* GetBlueprintProperty(UBlueprint* InBlueprint, const FName InName);
	/* Get all the variable properties on the blueprint, will not include any inherited variables */
	static TArray<FProperty*> GetVariableProperties(UBlueprint* InBlueprint, const bool IncludeWidgets = false, const bool IncludeComponents=false);
	/* return the name of all the widgets in the widget tree */
	static TArray<FName> GetWidgetTreeNames(UBlueprint* InBlueprint);
	
	static FText GetPropertyCategoryText(FProperty* InProperty);
	static TArray<FString> GetCategoryTextsAsString(UBlueprint* InBlueprint, const bool bShowUserVarsOnly = true);
	static TArray<FText> GetCategoryTexts(UBlueprint* InBlueprint, const bool bShowUserVarsOnly = true);
	

	/* convert a EPropertyFlag to the EBPElgKEWPropertyFlags version */
	static EBPElgKEWPropertyFlags ConvertFromEPropertyFlags(EPropertyFlags InFlag);
	static EPropertyFlags ConvertToEPropertyFlags(EBPElgKEWPropertyFlags InFlag);

	static EGraphType GetGraphType(UEdGraph* InGraph);
	static bool IsGraphAFunction(UEdGraph* InGraph);
	static TArray<FBPVariableDescription> GetLocalVariableDescriptions(UEdGraph* InGraph);
	static TArray<UElgBESGraphVariableLocal*> GetLocalVariables(UBlueprint* InBlueprint, UEdGraph* InGraph);
	static UElgBESGraphFunctionPin* GetInputPinVariable(UBlueprint* InBlueprint, UEdGraph* InGraph, const FName InName);
	static TArray<UElgBESGraphFunctionPin*> GetInputPinVariables(UBlueprint* InBlueprint, UEdGraph* InGraph);
	static TArray<UElgBESGraphFunctionPin*> GetOutputPinVariables(UBlueprint* InBlueprint, UEdGraph* InGraph);
	static UElgBESGraphFunctionPin* GetOutputPinVariable(UBlueprint* InBlueprint, UEdGraph* InGraph, const FName InName);

	static void GetVariableIconFromPinType(const FEdGraphPinType InPinType, FSlateBrush& OutBrush);
	static UFunction* FindFunctionInBlueprint(UBlueprint* InBlueprint, const FName InFunctionName);
	static FProperty* GetFunctionProperty(UFunction* InFunction, const FName InVariableName);

	

	/* If you don't have a OldName pass in NAME_None */
	static bool NameValidator(UBlueprint* InBlueprint, const FName InOldName, const FName InNewName, FString& OutErrorMessage, UFunction* InFunction = NULL);
	
	static UFunction* GetLocalVariableScope(UFunction* InFunction, const FName InName);
	
	/* Return the name of the new pin */
	static FName AddEditablePin(UBlueprint* InBlueprint, UK2Node_EditablePinBase* InEntry, const FName InName, FS_ElgGraphPinType InType, const FString InDefaultValue, EEdGraphPinDirection InPinDirection);
	static void RemoveEditablePin(UBlueprint* InBlueprint, UK2Node_EditablePinBase* InEntry, const FName InName);
	static void ChangeEditablePinType(UK2Node_EditablePinBase* InEntry, const FName InPinName, FS_ElgGraphPinType InNewType);
	/* return the new pin name, it could have been changed to make it unique */
	static FName ChangeEditablePinName(UK2Node_EditablePinBase* InEntry, const FName InPinName, const FName InNewName);
	static void EditablePinChanged(UK2Node_EditablePinBase* InEntry);
	static bool MoveEditablePinDown(UK2Node_EditablePinBase* InNode, const int32 InCurrentIndex);
	static bool MoveEditablePinUp(UK2Node_EditablePinBase* InNode, const int32 InCurrentIndex);
	static bool HasEditablePin(UK2Node_EditablePinBase* InNode, const FName InPinName);
	static TArray<UK2Node_EditablePinBase*> GatherAllResultNodes(UK2Node_EditablePinBase* TargetNode);

	static FS_ElgFunctionInfo CreateFunctionInfo(const UFunction* InFunction);

	static EBPElgKEWWFunctionType GetFunctionType(const UEdGraph* InFunctionGraph);

	static FName GetInterfaceClassName(const UClass* InClass);

	static bool AddOverridableFunction(UBlueprint* InBlueprint, const FName InFunctionName);


	static bool IsNodeConnected(UEdGraphNode* InNode);

	static FName DuplicateVariable(UBlueprint* InBlueprint, FProperty* InVariableProperty, const bool IsMember=true);
	static UEdGraph* DuplicateGraph(UBlueprint* InBlueprint, UEdGraph* InGraph);

	/* Update the the variable reference the node has. */
	static void ChangeVariableNodeReference(UBlueprint* InBlueprint, UK2Node_Variable* InNodeToUpdate, const FName InNewVariableName, FEdGraphPinType InNewPinType, UStruct* InOwnerScope, const bool bIsInputPin=false);
	static void ChangeVariableNodeReferenceToMember(UBlueprint* InBlueprint, UK2Node_Variable* InNodeToUpdate, const FName InVariableName);
	static void ChangeVariableNodeReferenceToLocal(UBlueprint* InBlueprint, UK2Node_Variable* InNodeToUpdate, UFunction* InFunction, const FName InVariableName);
	static void ChangeVariableNodeReferenceToPinVar(UBlueprint* InBlueprint, UEdGraph* InFunctionGraph, UK2Node_VariableGet* InNodeToUpdate, const FName InVariableName);
	/* Return any Pins that not are PinTypesCompatible withe the property type */
	static void GetUnsupportedPinsForProperty(UK2Node_Variable* InNode, const FEdGraphPinType& InNewPinType, TArray<UEdGraphPin*>& OutBroken);

	/* Connect the node to the pin, if it can, and adjust the position of the node */
	static void DropNodeOnPin(UK2Node* InNode, UEdGraphPin* InPin);
	/* Connect the the node to the Pin if the pin type is compatible, it will adjust the position of the node */
	static bool DropVariableOnPin(UK2Node_Variable* InNode, UEdGraphPin* InPin);
	/* Check if the variable can be connected to the pin */
	static bool CanDropVariableOnPin(UK2Node_Variable* InNode, UEdGraphPin* InPin);
	static bool CanDropPropertyOnPin(FProperty* InProperty, UEdGraphPin* InPin);


	static bool AddEventDispatcher(UBlueprint* InBlueprint, const FName InName);

};


	