// Copyright 2019-2021 ElgSoft. All rights reserved. 

#pragma once

#include "ElgKEWEnum.h"
#include "ElgKEWStructs.h"
#include "ElgBESBase.h"
#include "ElgBESGraphEditablePinBase.generated.h"

/**
 * Base for editable pin, Function, Macro and EventDispatcher, where you can name/change type etc.
 */
UCLASS()
class ELGKISMETEDITORWIDGET_API UElgBESGraphEditablePinBase : public UElgBESBase
{
	GENERATED_BODY()
	
public:

#pragma region Property

	UPROPERTY()
		bool bCanAddExecPins;

	UPROPERTY()
		EBPElgKEWPinDirection Direction;

	UPROPERTY()
		bool bInputPin;

	/** Name of the  */
	UPROPERTY()
		FName PinName;

	/** A Guid that will remain constant even if the VarName changes */
	UPROPERTY()
		FGuid Guid;

	/** Friendly name of the  */
	UPROPERTY()
		FString FriendlyName;

	/** Category this  should be in */
	UPROPERTY()
		FText Category;

	UPROPERTY()
		FS_ElgGraphPinType PinType;

	/** Optional new default value stored as string*/
	UPROPERTY()
		FString DefaultValue;

	struct FEdGraphPinType VarType;
	FBPVariableDescription VariableDescription;

	class UFunction* FunctionPtr;
	class UEdGraphPin* PinPtr;
	class UK2Node_EditablePinBase* NodePtr;

#pragma endregion

	/* Remove the Input Pin  */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|EditablePin")
	void Remove();

#pragma region Setters

	/*
	* Set the default value of this , the default value is always stored as a string.
	* Only input pins can have a default value.
	*/
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|EditablePin")
		void SetDefaultValue(const FString InValue);

	/* Set the type of this Pin */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|EditablePin")
		void SetType(FS_ElgGraphPinType InNewType);

	/* Set the name of this Pin */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|EditablePin")
		void SetName(const FName InNewName);

#pragma endregion


#pragma region PinOrder

	/* Move the pin up on the function node*/
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|EditablePin")
		bool MovePinUp();
	/* Move the pin down on the function node*/
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|EditablePin")
		bool MovePinDown();


#pragma endregion


#pragma region Getters


	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|EditablePin", meta = (DisplayName = "GetFriendlyName", CompactNodeTitle = "FriendlyName"))
		FString GetFriendlyName();
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|EditablePin", meta = (DisplayName = "GetPinType", CompactNodeTitle = "PinType"))
		FS_ElgGraphPinType GetPinType();
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|EditablePin", meta = (DisplayName = "GetDefaultValue", CompactNodeTitle = "DefaultValue"))
		FString GetDefaultValue();
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|EditablePin", meta = (DisplayName = "GetIsInput", CompactNodeTitle = "bInput"))
		bool GetIsInputPin();
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|EditablePin", meta = (DisplayName = "GetDirection", CompactNodeTitle = "Direction"))
		EBPElgKEWPinDirection GetDirection();

#pragma endregion


	virtual void ShowInDetailsInternal() override;
	virtual FName GetNameInternal() override;
	virtual void GetIconInternal(struct FSlateBrush& OutBrush) override;


	/* Setup the Editable Pin */
	void Setup(UBlueprint* InBlueprint, UEdGraph* InGraph, class UK2Node_EditablePinBase* InNode, UEdGraphPin* InPin);
	virtual void SetupCustom() {};
	virtual bool SupportExecPins() { return false; };

	/* Update all the base pin property*/
	void UpdatePinProperties();
	
	/* validate that the pin is valid and make sure it's updated. */
	bool ValidateAndUpdate();

};
