// Copyright 2019-2023 ElgSoft. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "BPWrappers/ElgBESGraphVariableBase.h"
#include <K2Node_FunctionEntry.h>
#include "ElgKEWStructs.h"
#include <ElgKEWEnum.h>
#include "ElgBESGraphVariableLocal.generated.h"

/**
 * Blueprint wrapper for a LocalVariable
 */
UCLASS(Blueprintable)
class ELGKISMETEDITORWIDGET_API UElgBESGraphVariableLocal : public UElgBESGraphVariableBase
{
	GENERATED_BODY()

public:

	UEdGraph* GraphPtr;
	UFunction* FunctionPtr;
	UK2Node_FunctionEntry* EntryNodePtr;


	/* Remove the variable
	* bShowDialog if true and the variable is in use a Dialog is show to let the user decide if it should be removed or not.
	*/
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|LocalVariable")
		void RemoveVariable(const bool bShowDialog = false);



#pragma region Setters

	/* Rename the variable */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|LocalVariable", meta = (DisplayName = "SetName"))
		void SetVariableName(FName InName);

	/* Set the type of this variable */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|LocalVariable", meta = (DisplayName = "SetType"))
		void SetVariableType(FS_ElgGraphPinType InNewType);

	/* Set the default value of this variable, the default value is always stored as a string */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|LocalVariable", meta = (DisplayName = "SetDefaultValue"))
		void SetVariableDefaultValue(const FString InValue);

#pragma endregion

	
	void Setup(UBlueprint* InBlueprint, UEdGraph* InGraph, UK2Node_FunctionEntry* InEntry, FBPVariableDescription InVariableDescription);

#pragma region overrides
	virtual int32 GetVarReferenceCount() override;
	virtual EBPElgBESVariableType GetVarType() override;
	virtual FName DuplicateVar() override;
	virtual FProperty* GetProperty() override;
	virtual bool IsVarValid() override;
	virtual FString GetDefaultValue() override;
	virtual UFunction* GetVariableScope() override;
	virtual bool ValidateAndUpdate() override;

#pragma endregion

	static UElgBESGraphVariableLocal* MakeGraphLocalVariableObject(UBlueprint* InBlueprint, UEdGraph* InGraph, UK2Node_FunctionEntry* InEntry, FBPVariableDescription InVariableDescription);
};
