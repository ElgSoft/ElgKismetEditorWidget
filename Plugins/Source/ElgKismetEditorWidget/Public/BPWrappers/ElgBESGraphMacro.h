// Copyright 2019-2021 ElgSoft. All rights reserved. 

#pragma once

#include "ElgKEWEnum.h"
#include "ElgKEWStructs.h"
#include "ElgBESBase.h"
#include "ElgBESGraphMacro.generated.h"

class UEdGraph;
class UK2Node_Tunnel;

/**
 * Blueprint Wrapper object for a Macro in a Blueprint
 */
UCLASS(Blueprintable)
class ELGKISMETEDITORWIDGET_API UElgBESGraphMacro : public UElgBESBase
{
	GENERATED_BODY()

public: 

	UPROPERTY()
		FName Name;

	UPROPERTY()
		FGuid Guid;

	UEdGraph* GraphPtr;
	UK2Node_Tunnel* InputNodePtr;
	UK2Node_Tunnel* OutputNodePtr;



#pragma region PinVariables

	/* return any pin variables this function has */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Macro")
		TArray<UElgBESGraphMacroPin*> GetMacroPinVariables();

	/* return any input pin this function has */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Macro")
		TArray<UElgBESGraphMacroPin*> GetMacroInputPinVariables();

	/* return the input pin variable, if the function has it */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Macro")
		bool GetMacroInputPinVariable(const FName InName, UElgBESGraphMacroPin*& OutPinVariable);

	/* return any output pin this function has */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Macro")
		TArray<UElgBESGraphMacroPin*> GetMacroOutputPinVariables();

	/* return the output pin variable, if the function has it */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Macro")
		bool GetMacroOutputPinVariable(const FName InName, UElgBESGraphMacroPin*& OutPinVariable);

	/* Add a new input pin for the function */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Macro")
		UElgBESGraphMacroPin* AddInputPin(FS_ElgGraphPinType InType, FName InPinName, const FString InDefaultValue);

	/* Add a new output pin for the function */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Macro")
		UElgBESGraphMacroPin* AddOutputPin(FS_ElgGraphPinType InType, FName InPinName);

#pragma endregion


#pragma region Misc

	/** Open a Kismet window, focusing on the Macro.  Prefers existing windows, but will open a new application if required. */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Macro")
		void OpenMacroGraph();

	/* remove the Macro
	* bShowDialog if true and the variable is in use a Dialog is show to let the user decide if it should be removed or not.
	*/
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Macro")
		void Remove(const bool bShowDialog = false);

	/* Set the category for the Macro */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Macro", meta = (DisplayName = "SetCategory"))
		void SetCategory(FText InCategoryName);

	/* Set the name of the macro */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Macro", meta = (DisplayName = "SetName"))
		void SetMacroName(FName InNewName);

	/* Duplicate the Macro */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Macro")
		void Duplicate(UElgBESGraphMacro*& OutMacro);

#pragma endregion



#pragma region Getters

	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Macro", meta = (DisplayName = "GetGuid", CompactNodeTitle = "Guid"))
		FGuid GetGuid();
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Macro", meta = (DisplayName = "GetBlueprintName", CompactNodeTitle = "BlueprintName"))
		FName GetBlueprintName();
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Macro", meta = (DisplayName = "GetKeywords", CompactNodeTitle = "Keywords"))
		FText GetKeywords();
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Macro", meta = (DisplayName = "GetCompactNodeTitle", CompactNodeTitle = "CompactNodeTitle"))
		FText GetCompactNodeTitle();
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Macro", meta = (DisplayName = "IsValid", CompactNodeTitle = "IsValid"))
		bool IsMacroValid();

#pragma endregion

	virtual void ShowInDetailsInternal() override;
	virtual FName GetNameInternal() override;
	virtual FText GetToolTipInternal() override;
	virtual FText GetCategoryInternal() override;
	virtual int32 GetCountInternal() override;
	virtual void GetIconInternal(struct FSlateBrush& OutBrush) override;

#pragma region ptrGetters

	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Macro")
		UBlueprint* GetBlueprint();
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Macro")
		UEdGraph* GetGraph();

#pragma endregion

	void Setup(UEdGraph* InGraph);

	bool ValidateMacro();

	static UElgBESGraphMacro* MakeGraphMacroObject(UEdGraph* InGraph);
	static void GetMacroTunnelNodes(UEdGraph* InGraph, UK2Node_Tunnel*& OutInputNode, UK2Node_Tunnel*& OutOutputNode);
};
