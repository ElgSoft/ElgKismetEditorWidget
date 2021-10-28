// Copyright 2019-2021 ElgSoft. All rights reserved. 

#pragma once

#include "ElgKEWEnum.h"
#include "ElgKEWStructs.h"
#include "ElgBESBase.h"
#include "ElgBESGraphFunction.generated.h"

class UEdGraph;
class UK2Node_FunctionEntry;
class UK2Node_FunctionResult;

/**
 * 
 */
UCLASS(Blueprintable)
class ELGKISMETEDITORWIDGET_API UElgBESGraphFunction : public UElgBESBase
{
	GENERATED_BODY()

public:

	UPROPERTY()
		FName Name;

	UPROPERTY()
		FGuid Guid;

	UEdGraph* GraphPtr;
	UK2Node_FunctionEntry* EntryNodePtr;
	UK2Node_FunctionResult* ResultNodePtr;



#pragma region FunctionSetters

	/* Set if the function should be a pure or not */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function")
		void SetIsPure(const bool bIsPure = true);
	/* Set the Access Specifier for the function */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function")
		void SetAccessSpecifier(EBPElgKEWFunctionAccess InAccessSpecifier);
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function")
		void SetCategory(FText InText);
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function")
		void SetTipText(const FText InText);
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function")
		void SetKeywords(const FText InText);
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function")
		void SetCompactNodeTitle(const FText InText);
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function")
		void SetDeprecationMessage(const FString InText);
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function")
		void SetIsDeprecated(const bool InState);
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function")
		void SetCallInEditor(const bool InState);

	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function", meta=(DisplayName="SetName"))
		void SetFunctionName(FName InNewName);

#pragma endregion

#pragma region Misc

	/** Open a Kismet window, focusing on the function.  Prefers existing windows, but will open a new application if required. */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function")
		void OpenFunctionGraph();

	/* remove the function
	* bShowDialog if true and the variable is in use a Dialog is show to let the user decide if it should be removed or not.* 
	*/
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function")
		void Remove(const bool bShowDialog = false);

	/* Duplicate the function */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function")
		void Duplicate(UElgBESGraphFunction*& OutFunction);

#pragma endregion


#pragma region FunctionFlags

	/* check if the function has a flag */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Function")
		bool HasFunctionFlag(EElgFunctionFlags InFlag);

	/* check if the function has any flags */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Function")
		bool HasFunctionFlags(TArray<EElgFunctionFlags> InFlags);

#pragma endregion



#pragma region LocalVariables

	/* return any local variables this function has */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function", meta = (DisplayName = "Get Local Variables"))
		TArray<UElgBESGraphVariableLocal*> GetFunctionLocalVariables();

	/* return the local variable named InName */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function", meta = (DisplayName = "Get Local Variable"))
		bool GetLocalVariableByName(const FName InName, UElgBESGraphVariableLocal*& OutVariable );

	/* Add a new local variable to the function */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function", meta = (DisplayName="Create Local Variable"))
		bool FunctionAddLocalVariable(const FName InVariableName, const FS_ElgGraphPinType InType, const FText InCategoryName, const FString InDefaultValue, UElgBESGraphVariableLocal*& OutVariable);

	/* Remove a local variable if it can be found */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function", meta = (DisplayName = "Remove Local Variable"))
		void FunctionRemoveLocalVariableByName(const FName InVariableName);

	/* Check if a Name is valid to use as name for local variable */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function", meta = (ExpandEnumAsExecs = "Branch"))
		bool IsNameValid(const FName InName, EBPElgKEWWResult& Branch);


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

	/* Add a new input pin for the function */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function")
		UElgBESGraphFunctionPin* FunctionAddInputPin(FS_ElgGraphPinType InType, FName InPinName, const FString InDefaultValue);
	
	/* Add a new output pin for the function */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Function")
		UElgBESGraphFunctionPin* FunctionAddOutputPin(FS_ElgGraphPinType InType, FName InPinName);

#pragma endregion


#pragma region FunctionGetters

	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Function", meta = (DisplayName = "GetGuid", CompactNodeTitle = "Guid"))
		FGuid GetGuid();
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Function", meta = (DisplayName = "GetIsPure", CompactNodeTitle = "Pure?"))
		bool GetIsPure();
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


	/* Return if this is an parent function that are override */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Function", meta = (DisplayName = "GetIsOverride", CompactNodeTitle = "bOverride"))
		bool GetIsOverride();
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Function", meta = (DisplayName = "GetIsEditiable", CompactNodeTitle = "bEditiable"))
		bool GetIsEditable();

#pragma endregion

	virtual void ShowInDetailsInternal() override;
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

	void Setup(UEdGraph* InGraph);

	bool ValidateFunction();

	static UElgBESGraphFunction* MakeGraphFunctionObject(UEdGraph* InGraph);

};
