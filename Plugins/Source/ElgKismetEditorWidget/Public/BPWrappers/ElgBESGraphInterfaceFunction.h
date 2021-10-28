// Copyright 2019-2021 ElgSoft. All rights reserved. 

#pragma once

#include "ElgBESBase.h"
#include "ElgBESGraphInterfaceFunction.generated.h"

/**
 * Blueprint wrapper for Interface functions.
 */
UCLASS(Blueprintable)
class ELGKISMETEDITORWIDGET_API UElgBESGraphInterfaceFunction : public UElgBESBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
		FName FunctionName;
	UPROPERTY()
		FName InterfaceName;

	/* Is it a Function or Event */
	UPROPERTY()
		bool bFunction;
	/* if true the interface is inherit from a parent class. */
	UPROPERTY()
		bool bInherit;

	UBlueprint* BlueprintPtr; 
	UFunction* FunctionPtr;
	/* Local graph for functions, events do not have a graph. */
	UEdGraph* GraphPtr;
	UClass* InterfaceClassPtr;
	/* Ptr to the function graph in the Interface Class */
	UEdGraph* InterfaceGraphPtr;
	UK2Node_FunctionEntry* EntryNodePtr;
	UK2Node_FunctionResult* ResultNodePtr;

	/* has all the important info about the interface */
	FBPInterfaceDescription InterfaceDescription;
	/* the meta data for the interface function. */
	FKismetUserDeclaredFunctionMetadata* InterfaceMetaData;


	/* Show the function in the Details window, if it's open. */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function")
		void OpenOrSelect();

	/** Open a Kismet window, focusing on the function.  Prefers existing windows, but will open a new application if required. */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function")
		void OpenFunctionGraph();

	/** Add the Interface Function as an event,  */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function")
		bool ImpletentEvent();

	/** Add a inherited Interface Function as a override function or add the event to the graph*/
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function")
		bool ImpletentInherited();

#pragma region FunctionFlags

	/* check if the function has a flag */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Function")
		bool HasFunctionFlag(EElgFunctionFlags InFlag);

	/* check if the function has any flags */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Function")
		bool HasFunctionFlags(TArray<EElgFunctionFlags> InFlags);

#pragma endregion


#pragma region PinVariables

	/* return any pin variables this function has */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function")
		TArray<UElgBESGraphFunctionPin*> GetFunctionPinVariables();

	/* return any input pin this function has */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function")
		TArray<UElgBESGraphFunctionPin*> GetFunctionInputPinVariables();

	/* return the input pin variable, if the function has it */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function")
		bool GetFunctionInputPinVariable(const FName InName, UElgBESGraphFunctionPin*& OutPinVariable);

	/* return any output pin this function has */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function")
		TArray<UElgBESGraphFunctionPin*> GetFunctionOutputPinVariables();

	/* return the output pin variable, if the function has it */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function")
		bool GetFunctionOutputPinVariable(const FName InName, UElgBESGraphFunctionPin*& OutPinVariable);

#pragma endregion


#pragma region FunctionGetters
	/* Return if this is a function or event */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Function", meta = (DisplayName = "GetIsFunction", CompactNodeTitle = "IsFunction"))
		bool GetIsFunction();
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Function", meta = (DisplayName = "GetBlueprintName", CompactNodeTitle = "BlueprintName"))
		FName GetBlueprintName();
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Function", meta = (DisplayName = "GetKeywords", CompactNodeTitle = "Keywords"))
		FText GetKeywords();
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Function", meta = (DisplayName = "GetCompactNodeTitle", CompactNodeTitle = "CompactNodeTitle"))
		FText GetCompactNodeTitle();
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Function", meta = (DisplayName = "GetDeprecationMessage", CompactNodeTitle = "DeprecationMessage"))
		FString GetDeprecationMessage();
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Function", meta = (DisplayName = "GetIsDeprecated", CompactNodeTitle = "bIsDeprecated"))
		bool GetIsDeprecated();
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Function", meta = (DisplayName = "GetbCallInEditor", CompactNodeTitle = "bCallInEditor"))
		bool GetCallInEditor();
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Function", meta = (DisplayName = "GetbSupportDragDrop", CompactNodeTitle = "bSupportDragDrop"))
		bool GetSupportDragDrop();
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Function", meta = (DisplayName = "GetNumParams", CompactNodeTitle = "NumParams"))
		int32 GetNumParamsp();
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Function", meta = (DisplayName = "GetHasReturnValue", CompactNodeTitle = "HasReturnValue"))
		bool GetHasReturnValue();

	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Function", meta = (DisplayName = "GetIsValid", CompactNodeTitle = "IsValid"))
		bool GetIsValid();
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function", meta = (ExpandEnumAsExecs = "Branch"))
		void IsValidBranch(EBPElgKEWWResult& Branch);


	/* Return if this interface is inherited from a parent class  */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Interface", meta = (DisplayName = "GetIsInherited", CompactNodeTitle = "IsInherited?"))
		bool GetIsInherited() { return bInherit; };

#pragma endregion

	virtual void SelectInMyBlueprintInternal() override;
	virtual FName GetNameInternal() override;
	virtual FText GetToolTipInternal() override;
	virtual FText GetCategoryInternal() override;
	virtual int32 GetCountInternal() override;
	virtual void GetIconInternal(struct FSlateBrush& OutBrush) override;


#pragma region ptrGetters

	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function")
		UFunction* GetFunction();
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function")
		UBlueprint* GetBlueprint();
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function")
		UEdGraph* GetGraph();
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function")
		UK2Node_FunctionEntry* GetFunctionEntryNode();
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function")
		UK2Node_FunctionResult* GetFunctionResultNode();

#pragma endregion

	void Setup(UBlueprint* InBlueprint, UFunction* InFunction, FBPInterfaceDescription InInterface);
	void Setup(UBlueprint* InBlueprint, UFunction* InFunction, UClass* InInterfaceClass);

	bool ValidateFunction();

	static UElgBESGraphInterfaceFunction* MakeGraphInterfaceFunction(UBlueprint* InBlueprint, UFunction* InFunction, FBPInterfaceDescription InInterface);
	static UElgBESGraphInterfaceFunction* MakeGraphInterfaceFunction(UBlueprint* InBlueprint, UFunction* InFunction, UClass* InInterfaceClass);
};
