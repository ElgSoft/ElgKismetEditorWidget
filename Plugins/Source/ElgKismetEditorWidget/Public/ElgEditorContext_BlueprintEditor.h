// Copyright 2019-2021 ElgSoft. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ElgKEWStructs.h"
#include "ElgKEWEnum.h"
#include "ElgEditorContext_BlueprintEditor.generated.h"

class UEdGraph;
class UElgBESGraphNode;
class UElgBESGraphVariableLocal;
class UElgBESGraphVariable;
class UElgBESGraphEventDispatcher;



#pragma region Delegates

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FElgKEWOnCompileSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FElgKEWOnChangeSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FElgKEWOnNodeSelectionChangedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FElgKEWOnFocusGraphChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FElgKEWOnEditorRefresh);

#pragma endregion


/**
 * Expose FBlueprintEditor functionality to EditorWidgets
 */
UCLASS(Blueprintable)
class ELGKISMETEDITORWIDGET_API UElgEditorContext_BlueprintEditor : public UObject
{
	GENERATED_BODY()

public:

	UBlueprint* BlueprintPtr;
	TWeakPtr<class FBlueprintEditor> BlueprintEditorPtr;
	UEdGraph* CurrentGraphTab;


#pragma region Setup

	virtual ~UElgEditorContext_BlueprintEditor();

	void Setup(TWeakPtr<class FBlueprintEditor> InEditor);

	void Cleanup();

	/* update the Blueprint Editor reference if it's no longer valid */
	void UpdateBlueprintEditor(TWeakPtr<class FBlueprintEditor> InEditor);

#pragma endregion	
	

#pragma region Ticker

	/* Delay between each process tick */
	UPROPERTY()
		float myTickDelay = 0.1f;
	FTickerDelegate myTickDelegate;	
	FTSTicker::FDelegateHandle myTickDelegateHandle;
	void SetupTicker();
	bool MyTick(float DeltaSeconds);

#pragma endregion 

#pragma region SelctionCheck
	UPROPERTY()
		int32 SelectedNodeCount = 0;
	UPROPERTY()
		TSet<UObject*> SelectedNodeSet;

#pragma endregion


#pragma region Nodes

	/* return the number of node the Blueprint has */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		int32 NodeCount();

	/* Get any selected node in the blueprint */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		TArray<UElgBESGraphNode*> GetSelectedNodes();

	/* Get all the nodes in the graph */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		TArray<UElgBESGraphNode*> GetNodes();

	/* Get all the nodes used in the function */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		TArray<UElgBESGraphNode*> GetNodesInFunction(UElgBESGraphFunction* InFunction);

	/* Get all the nodes in a graph */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		TArray<UElgBESGraphNode*> GetNodesInGraph(UEdGraph* InGraph);

	/* Get all the nodes the graph that has focus */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		TArray<UElgBESGraphNode*> GetNodesCurrentGraph();

	/* Get a node by its guid */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		bool GetNodeByGuid(FGuid InGuid, UElgBESGraphNode*& OutNode);

	/* Add the node to the node selection */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		void AddNodeToSelection(UElgBESGraphNode* InNode);

	/* Add the nodes to the node selection */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		void AddNodesToSelection(TArray<UElgBESGraphNode*> InNodes);

#pragma endregion 


#pragma region Messages

	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		void LogSimpleMessage(const FText InMessage);

	/* Log to the compiler log TODO:: Should have a Enum to select what type of message it is. */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		void LogMessageToCompilerLog(const FText InMessage, EBPElgKEWMessageSeverity InSeverity, bool bForceMessageDisplay = true);

#pragma endregion 


#pragma region Misc

	// Jumps to a hyper linked node, pin, or graph, if it belongs to this blueprint
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		void JumpToObject(UObject* InObjectToJumpTo);

	/* Close a Tab, pass in the graph */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		void CloseTab(UObject* InTabToClose);

	/* Get the blueprint Ptr of the current editor */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget", meta = (DisplayName="GetUBluepritntPtr"))
		UBlueprint* GetBlueprint();

	/* Get the UEdGraph ptr */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget", meta = (DisplayName = "GetUEdGraphPtr"))
		UEdGraph* GetFocusGraph();


	/* Open the Search UI */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Editor")
		void SummonSearchUI(const FString InSearchTerms="", const bool bInSelectFirstResult = false);

	/* Open the FindAndReplaceUI */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Editor")
		void SummonFindAndReplaceUI();

	/** Returns true if in debugging mode */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Editor")
		bool InDebuggingMode();

	/** Returns true if in editing mode */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Editor")
		bool InEditingMode();

	/** Returns true if able to compile */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Editor")
		bool IsCompilingEnabled();

	/** Returns whether a graph is editable or not */
	//UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Editor")
	//	bool IsEditable();

	/** Determines if the graph is ReadOnly, this differs from editable in that it is never expected to be edited and is in a read-only state */
	//UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Editor")
	//	bool IsGraphReadOnly();

	/** Gets the focused graph view location/zoom amount */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Editor")
		void GetViewLocation(FVector2D& OutLocation, float& OutZoomAmount);
	
	/** Sets the focused graph view location/zoom amount */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Editor")
		void SetViewLocation(const FVector2D InLocation, float InZoomAmount);
	
	/** Returns true if the parent class of the Blueprint being edited is also a Blueprint */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Editor")
		bool IsParentClassABlueprint();
	
	/** Returns true if the parent class of the Blueprint being edited is native */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Editor")
		bool IsParentClassNative();

	/** Returns our PIE Status - SIMULATING / SERVER / CLIENT */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Editor")
		FText GetPIEStatus();

	/* Open editing defaults; */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Editor")
		void StartEditingDefaults(bool bInAutoFocus = true, bool bInForceRefresh = false);

	/** Gets the UI selection state of this editor */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Editor")
		void GetUISelectionState(FName& OutCurrentState);
	
	/** Set the UI Selection State, SelectionState_*,  MyBlueprint, Components, Graph, ClassSettings or ClassDefaults */
	//UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Editor")
		void SetUISelectionState(const FName InSelectionOwner);

	/** Can generate native code for current blueprint */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Editor", meta = (DeprecatedFunction, DeprecationMessage = "Function has been deprecated as NativeCodeGeneration is no longer supported."))
		bool CanGenerateNativeCode();

	/** Opens a native code generation modal window */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Editor", meta = (DeprecatedFunction, DeprecationMessage = "Function has been deprecated as NativeCodeGeneration is no longer supported."))
		void OpenNativeCodeGenerationTool();

	/** Make nodes which are unrelated to the selected nodes fade out */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Editor")
		void ToggleHideUnrelatedNodes();
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Editor")
		bool IsToggleHideUnrelatedNodesChecked();

	/* No longer needed as the button is always shown now */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Editor", meta = (DeprecatedFunction, DeprecationMessage = "Function has been deprecated as the Toggle Hide Unrelated Nodes button is always shown now."))
		bool ShouldShowToggleHideUnrelatedNodes(bool bInIsToolbar);

	/* Get all the Category's used by the blueprint */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Editor", meta=(bShowUserVarsOnly="true"))
		void GetCategoryTexts(bool bShowUserVarsOnly, TArray<FText>& OutCategory);
	/* Get all the Category's used by the blueprint */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Editor", meta = (bShowUserVarsOnly = "true"))
		void GetCategoryTextsAsString(bool bShowUserVarsOnly, TArray<FString>& OutCategory);


	
	UFUNCTION()
	void HandleDragAndDrop(const FPointerEvent& PointerEvent, UDragDropOperation* Operation);

	/* Return a map with the name of the variable and how many times its used inside the blueprint */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable")
		TMap<FName, int32> GetVariableUsageMap();

	/* Check if a Name is valid, no other variable or function use it */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget", meta = (ExpandEnumAsExecs = "Branch"))
		bool IsNameValid(const FName InName, EBPElgKEWWResult& Branch);

	/* Make sure the name is unique, will add _1 etc if something already is named InName */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget")
		void GetUniqueName(const FName InName, FName& OutName);

#pragma endregion 


#pragma region ClearSelection

	/* Deselect all nodes in the graph */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		void ClearSelectionGraph();

	/* Deselect anything selected in the MyBlueprint window. */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		void ClearSelectionMyBlueprint();
	
	/* Deselect any selected Components */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		void ClearSelectionComponents();

	/* Deselect anything selected */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		void ClearSelectionAll();

#pragma endregion 


#pragma region KismetInspector

	/* Show the property in the Details inspector window, 
		this will deselect anything selected, nodes or stuff in MyBlueprint.
	*/
	void ShowPropertyInInspector(FProperty* InProperty);

	void ShowGraphInInspector(UEdGraph* InGraph);

	void ShowDelegateInInspector(UEdGraph* InGraph, UK2Node_EditablePinBase* InNode);

#pragma endregion 


#pragma region MyBlueprint

	/* select an item in MyBlueprint */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		void SelectItemInMyBlueprint(FName InName);

#pragma endregion 


#pragma region Function 

	/* Get GraphFunction objects with information about all functions in this Blueprint */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		TArray<UElgBESGraphFunction*> GetFunctions();

	/* Get GraphFunction object with all the info about the function */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		UElgBESGraphFunction* GetFunctionByGuid(FGuid InGuid, bool& OutResult);
	
	/* Get GraphFunction objects with all the info about the function */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		UElgBESGraphFunction* GetFunctionByName(FName InName, bool& OutResult);

	/* Get the GraphFunction if the focus graph is a function, otherwise it will be a nullptr */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		UElgBESGraphFunction* GetCurrentFunction(bool& OutResult);


	/* Create a new function */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		UElgBESGraphFunction* CreateNewFunction(FName InName, FText InCategory);

	/* Get all functions that has been overridden in the blueprint*/
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		TArray<UElgBESGraphFunction*> GetOverrideFunctions();

	/* Return an array with the name of all the functions that can be overridden in this blueprint */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		void GetOverridableFunctions(TArray<FS_ElgFunctionInfo>& OutInfo);

	/* Add new function */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		UElgBESGraphFunction* AddOverridableFunction(const FName InName);

#pragma endregion


#pragma region Macro

	/* Get GraphMacro objects with information about the macros in this Blueprint */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		TArray<UElgBESGraphMacro*> GetMacros();

	/* Get GraphMacro objects */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		UElgBESGraphMacro* GetMacroByName(FName InName, bool& OutResult);

	/* Create a new macro */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		UElgBESGraphMacro* CreateNewMacro(FName InName, FText InCategory);

#pragma endregion


#pragma region Variables

	/* Create a new variable in the blueprint  */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		bool CreateVariable(const FName InName, const FS_ElgGraphPinType InType, const FText InCategoryName, const FString InDefaultValue, UElgBESGraphVariable*& OutVariable);

	/* Remove a variable in the blueprint  */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		void RemoveVariable(const FName InName);

	/* Get the variables in the blueprint */ 
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		TArray<UElgBESGraphVariable*> GetVariables();

	/* Get a variable by its name */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		bool GetVariableByName(const FName InName, UElgBESGraphVariable*& OutVariable);

	/* Get a variable by its Guid */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		bool GetVariableByGuid(const FGuid InGuid, UElgBESGraphVariable*& OutVariable);
		
	/* Get all inherit variables */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		TArray<UElgBESGraphVariableInherited*> GetInheritVariables();


#pragma endregion


#pragma region FunctionPinVariables

	/* Return the function pins the in the graph, will check both regular and interface functions. */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget", meta = (DisplayName = "GetInputPinVariablesInGraph"))
		TArray<UElgBESGraphFunctionPin*> GetFunctionInputPinVariablesInGraph(UEdGraph* InGraph);


	/* Return the function pins in the focused graph, will check both regular and interface functions. */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget", meta=(DisplayName="GetInputPinVariablesInFocusGraph"))
		TArray<UElgBESGraphFunctionPin*> GetFunctionInputPinVariablesInFocusGraph();

#pragma endregion


#pragma region WidgetVariables

	/* Get any widgets that are exposed as variables, only work inside WidgetBlueprint graphs. */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		TArray<UElgBESGraphWidgetVariable*> GetWidgetVariables();

	/* Get all widgets in the WidgetTree/Hierarchy, only work inside WidgetBlueprint graphs.  */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		TArray<UElgBESGraphWidget*> GetWidgetTreeWidgets();


#pragma endregion


#pragma region LocalVariables

	/* Get the local variables in the graph that are in focus */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		TArray<UElgBESGraphVariableLocal*> GetLocalVariablesInFocusGraph();

#pragma endregion


#pragma region EventDispatchers

	/* Add a new EventDispatchers to the Blueprint */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		bool CreateEventDispatcher(FName InName, FText InCategory, UElgBESGraphEventDispatcher*& OutEventDispatcher);

	/* Get the EventDispatchers in the graph */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		TArray<UElgBESGraphEventDispatcher*> GetEventDispatchers();

	/* Get a EventDispatcher by it's name, nullptr if not found */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		UElgBESGraphEventDispatcher* GetEventDispatcherByName(const FName InName);

	/* Get all function signatures that can be used as a base for the Dispatcher. */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		TArray<FName> GetEventDispatcherSignaturesToCopy();


#pragma endregion


#pragma region Interface

	/* Get the Implemented interface in this blueprint */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		TArray<UElgBESGraphInterface*> GetInterfaceImplemented(const bool bIncludeInherent = false);

	/* Get the Implemented interface by name */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		UElgBESGraphInterface* GetInterfaceByName(const FName InName);

	/* Get the Implemented interface in the blueprint parent class */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		TArray<UElgBESGraphInterface*> GetInterfaceNativelyImplemented();

	/* Get the names of all the interface that exist in the project */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		TArray<FName> GetInterfaceNames();

	/* Add an interface  */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		void AddInterface(const FName InterfaceClass);
	
	/* Remove a interface */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		void RemoveInterface(const FName InterfaceClass, const bool InShowDialog = true);


#pragma endregion

	/* Remove a graph, if the blueprint has one with that name  */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		void RemoveGraphByName(const FName InName);

	/* Create a new Event/UberGraph in the Blueprint */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		void CreateUberGraph(FName InName, UElgBESGraphUber*& OutUberGraph);

	/* Get all Uber/Event graphs in the blueprint */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		TArray<UElgBESGraphUber*> GetUberGraphs();

	/* Get a new Event/UberGraph in the Blueprint */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		UElgBESGraphUber* GetUberGraphByName(const FName InName);

	/* return if the Blueprint has any extra Uber/Event graphs */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget")
		bool HasExtraUberGraphs();
	
	/* Get the default Event/UberGraph in the Blueprint, "EventGraph" */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		UElgBESGraphUber* GetEventGraph();


#pragma region Delegates

	TMap<UEdGraph*, FDelegateHandle> GraphChangedDelegatesMap;
	FDelegateHandle EditorRefreshDelegate;

	/* Event when the Blueprint is compiled */
	UPROPERTY(BlueprintAssignable)
		FElgKEWOnCompileSignature OnGraphCompiled;
	void HandleOnCompiled(class UBlueprint* InBlueprint);

	/**/
	UPROPERTY()
		bool bOnGraphChanged;

	/* Event when misc stuff happens in the blueprint */
	UPROPERTY(BlueprintAssignable)
		FElgKEWOnChangeSignature OnGraphChanged;
	void HandleOnChanged(class UBlueprint* InBlueprint);

	void HandleOnGraphChanged(const struct FEdGraphEditAction& InAction);

	/* Event when the selected node(s) change in anyway, select/deselect */
	UPROPERTY(BlueprintAssignable)
		FElgKEWOnNodeSelectionChangedSignature OnNodeSelectionChanged;
	void HandleOnNodeSelectionChanged();

	/* Event when the graph tab is changed */
	UPROPERTY(BlueprintAssignable)
		FElgKEWOnFocusGraphChanged OnFocusGraphChanged;
	void HandleOnFocusGraphChanged();

	/** Broadcasts a notification whenever the editor needs associated controls to refresh */
	UPROPERTY(BlueprintAssignable)
		FElgKEWOnEditorRefresh OnEditorRefresh;
	void HandleOnEditorRefresh();
	/* Proper show in the inspector window */
	FName KismetInspectorPropertyName;
	UObject* KismetInspectorPropertyWrapper = nullptr;


#pragma endregion

};
