// Copyright 2019-2021 ElgSoft. All rights reserved. 

#pragma once

#include "ElgKEWEnum.h"
#include "ElgKEWStructs.h"
#include "ElgBESBase.h"
#include "ElgBESGraphVariableBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ELGKISMETEDITORWIDGET_API UElgBESGraphVariableBase : public UElgBESBase
{
	GENERATED_BODY()
	
public:

	/** Name of the variable */
	UPROPERTY()
		FName VariableName;

	/** A Guid that will remain constant even if the VarName changes */
	UPROPERTY()
		FGuid VariableGuid;

	/** Friendly name of the variable */
	UPROPERTY()
		FString FriendlyName;

	/** Category this variable should be in */
	UPROPERTY()
		FText Category;

	UPROPERTY()
		FS_ElgGraphPinType PinType;

	/** Optional new default value stored as string*/
	UPROPERTY()
		FString DefaultValue;

	/** Type of the variable */
	struct FEdGraphPinType VarType;
	FBPVariableDescription VariableDescription;
	UBlueprint* BlueprintPtr;
	FProperty* PropertyPtr;



	/* Set the category for the variable */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "SetCategory"))
		void SetVariableCategory(FText InCategoryName);

	/* Duplicate the variable */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable")
		FName Duplicate();

#pragma region Getters

	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "GetIsValid", CompactNodeTitle = "IsValid"))
		bool GetVariableIsValid();

	/*  */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "GetPinType", CompactNodeTitle = "PinType"))
		FS_ElgGraphPinType GetVariablePinType();

	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "GetPinTypeName", CompactNodeTitle = "PinTypeName"))
		FName GetVariablePinTypeName();

	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "GetPinTypeNameAsString", CompactNodeTitle = "PinTypeString"))
		FString GetVariablePinTypeNameAsString();

	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "GetDefaultValue", CompactNodeTitle = "DefaultValue"))
		FString GetVariableDefaultValue();

	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "GetGuid", CompactNodeTitle = "Guid"))
		FGuid GetVariableGuid();
	
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "GetGuidAsString", CompactNodeTitle = "Guid"))
		FString GetVariableGuidAsString();
	
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "GetFriendlyName", CompactNodeTitle = "FriendlyName"))
		FString GetVariableFriendlyName();
	/* Get the type of the variable it is. */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "GetType", CompactNodeTitle = "Type"))
		EBPElgBESVariableType GetVariableType();

#pragma endregion


	void GetIconFromProperty(FSlateBrush& OutBrush);
	FString GetVarPropteryTipText();
	FString GetDefaultValueFromPropterty();
	FS_ElgGraphPinType GetPinTypeFromProperty();
	

	/* Default will check for any referenced UK2Node_Variable nodes */
	virtual int32 GetVarReferenceCount();
	virtual EBPElgBESVariableType GetVarType() { return EBPElgBESVariableType::Unknown; };
	virtual FName DuplicateVar();
	virtual FProperty* GetProperty();
	virtual bool IsVarValid();
	virtual FString GetDefaultValue() { return FString(); };	
	virtual FString GetVarTipText() { return FString(); };
	virtual UFunction* GetVariableScope() { return nullptr; };
	virtual bool ValidateAndUpdate();
	

	virtual void ShowInDetailsInternal() override;	
	virtual FName GetNameInternal() override;
	virtual FText GetToolTipInternal() override;
	virtual FText GetCategoryInternal() override;
	virtual int32 GetCountInternal() override;
	virtual void GetIconInternal(struct FSlateBrush& OutBrush) override;

	
	void UpdateVariableDescription(FBPVariableDescription InVariableDescription);
	void UpdateVariableProperty(FProperty* InProperty);
	void ClearVariableDescription();


};
