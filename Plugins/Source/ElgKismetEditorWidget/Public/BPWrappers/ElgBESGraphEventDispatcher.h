// Copyright 2019-2021 ElgSoft. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ElgBESGraphVariableBase.h"
#include "ElgBESGraphEventDispatcher.generated.h"

class UEdGraph;
class UFunction;
class UK2Node_FunctionEntry;

/**
 * Blueprint wrapper for EventDispacther in a Graph.
 */
UCLASS(Blueprintable)
class ELGKISMETEDITORWIDGET_API UElgBESGraphEventDispatcher : public UElgBESGraphVariableBase
{
	GENERATED_BODY()

public:

	UEdGraph* GraphPtr;
	UK2Node_EditablePinBase* NodePtr;
	

	/** Open a Kismet window, focusing on the function.  Prefers existing windows, but will open a new application if required. */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|EventDispatcher")
		void OpenEventGraph();


	/* Remove this Event Dispatcher */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|EventDispatcher", meta = (DisplayName = "Remove"))
		void RemoveEventDispatcher();

	/* return any pin variables */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|EventDispatcher")
		TArray<UElgBESGraphEventDispatcherPin*> GetPinVariables();

	/* return any input pin*/
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|EventDispatcher")
		TArray<UElgBESGraphEventDispatcherPin*> GetInputPinVariables();

	/* return the input pin variable, if it exist */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|EventDispatcher")
		bool GetInputPinVariable(const FName InName, UElgBESGraphEventDispatcherPin*& OutPinVariable);


	/* Add a new editable input pin  */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|EventDispatcher")
		UElgBESGraphEventDispatcherPin* AddInputPin(FS_ElgGraphPinType InType, FName InPinName, const FString InDefaultValue);

	/* Remove any existing pins and add pins in the Function Signature */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|EventDispatcher")
		void CopySignature(const FName InFunctionName);


	FMulticastDelegateProperty* GetDelegateProperty();

	void Setup(UBlueprint* InBlueprint, UEdGraph* InGraph);
	
	virtual FName DuplicateVar() override;
	virtual bool IsVarValid() override;
	virtual int32 GetVarReferenceCount() override;
	virtual bool ValidateAndUpdate() override;

	virtual void ShowInDetailsInternal() override;

	/* return all function signatures that can used as a base for EventDispatchers */
	static TArray<FName> GetFunctionsSignature(UBlueprint* InBlueprint);
	static UElgBESGraphEventDispatcher* MakeGraphEventDispatcherObject(UBlueprint* InBlueprint, UEdGraph* InGraph);

};
