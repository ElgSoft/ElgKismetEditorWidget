// Copyright 2019-2021 ElgSoft. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "BPWrappers/ElgBESGraphVariableBase.h"
#include "ElgBESGraphVariable.generated.h"

/**
 * Blueprint Wrapper for a variable in a blueprint graph
 */
UCLASS(Blueprintable, meta=(DisplayName = "UElgBESGraphVariableMember"))
class ELGKISMETEDITORWIDGET_API UElgBESGraphVariable : public UElgBESGraphVariableBase
{
	GENERATED_BODY()

public:

	/* Remove this variable 
	* bShowDialog if true and the variable is in use a Dialog is show to let the user decide if it should be removed or not.
	*/
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "Remove Variable"))
		void RemoveVariable(const bool bShowDialog = false);

#pragma region Setters

	/* Rename the variable */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "SetName"))
		void SetVariableName(FName InName);

	/* Set the type of this variable */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "SetType"))
		void SetVariableType(FS_ElgGraphPinType InNewType);

	/* Set the default value of this variable, the default value is always stored as a string */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "SetDefaultValue"))
		void SetVariableDefaultValue(const FString InValue);

	/* Whether this variable is publicly editable on instances of this Blueprint. */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "SetInstanceEditableFlag"))
		void SetVariableInstanceEditable(const bool bSetFlag = true);

	/* Whether this variable can be set by Blueprint nodes or if it is read-only. */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "SetBPReadOnlyFlag"))
		void SetVariableBPReadOnly(const bool bSetFlag = true);

	/* Should this variable be exposed as a pin when spawning this Blueprint? */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "SetExposeOnFlag"))
		void SetVariableExposeOnSpawn(const bool bSetFlag = true);

	/* Should this variable be private (derived blueprints cannot modify it)? */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "SetPrivateFlag"))
		void SetVariablePrivate(const bool bSetFlag = true);

	/* Should this variable be exposed for Matinee or Sequencer to modify? */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "SetExposeToCinematicFlag"))
		void SetVariableExposeToCinematic(const bool bSetFlag = true);

	/*  */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "SetReplicationConditionFlag"))
		void SetVariableReplicationCondition(ELifetimeCondition InCondition);

	/* Set the rep notify function name, if the name is None it remove any previews function. */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "SetRepNotifyFunc"))
		void SetVariableRepNotifyFunc(const FName InFunc);

	/* Should this Variable be replicated over the network? Note that Set and Map properties cannot be replicated */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "SetReplicationType"))
		void SetVariableReplicationType(const EBPElgVariableReplication InType);


	/* Should this variable read its default value from a config file if it is present? */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "SetConfigFlag"))
		void SetVariableConfigState(const bool bSetFlag = true);

	/* Should this variable not serialize and be zero-filled at load? */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "SetTransientFlag"))
		void SetVariableTransient(const bool bSetFlag = true);

	/* Should this variable be serialized for saved games? */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "SetSaveGameFlag"))
		void SetVariableSaveGame(const bool bSetFlag = true);

	/* Hide this variable in Class Defaults windows by default */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "SetAdvanceDisplayFlag"))
		void SetVariableAdvanceDisplay(const bool bSetFlag = true);

	/* Deprecate usage of this variable. Any nodes that reference it will produce a compiler warning indicating that it should be removed or replaced. */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "SetDepricatedFlag"))
		void SetVariableDepricated(const bool bSetFlag = true);

	/* Optional message to include with the deprecation compiler warning. An empty text remove the message. */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "SetDeprecationMessageFlag"))
		void SetVariableDeprecationMessage(const FText& InText);

	/* Extra information about this variable, shown when cursor is over it. */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "SetToolTip"))
		void SetVariableToolTip(const FText& InText);


#pragma endregion

#pragma region PropertyFlags

	/* Set a PropertyFlag on the variable, Add or remove the flag */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable")
		void SetVariableFlag(EBPElgKEWPropertyFlags InFlag, const bool bAdd = true);

	/* Check if the variable has a PropertyFlag or not  */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable")
		bool HasVariableFlag(EBPElgKEWPropertyFlags InFlag);

	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable")
		bool HasVariableFlags(TArray<EBPElgKEWPropertyFlags> InFlags);

#pragma endregion

#pragma region Getters

	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "GetbInstanceEditable", CompactNodeTitle = "bInstanceEditable"))
		bool GetVariableInstanceEditable();
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "GetbBlueprintReadOnly", CompactNodeTitle = "bBlueprintReadOnly"))
		bool GetVariableBlueprintReadOnly();
	/* Is this variable be exposed as a pin when spawning this Blueprint */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "GetbExposeOnSpawn", CompactNodeTitle = "bExposeOnSpawn"))
		bool GetVariableExposeOnSpawn();
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "GetbPrivate", CompactNodeTitle = "bPrivate"))
		bool GetVariablePrivate();
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "GetbExposeToCinematics", CompactNodeTitle = "bExposeToCinematics"))
		bool GetVariableExposeToCinematics();

	/* Does this variable read its default value from a config file if it is present?*/
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "GetbConfig", CompactNodeTitle = "bConfig"))
		bool GetVariableIsConfig();
	/* Is this variable not serialize and zero-filled at load? */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "GetbTransient", CompactNodeTitle = "bTransient"))
		bool GetVariableTransient();
	/* Is this variable serialized for saved games? */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "GetbSaveGame", CompactNodeTitle = "bSaveGame"))
		bool GetVariableSaveGame();
	/* Is this variable visible in the Class Default windows */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "GetbAdvancedDisplay", CompactNodeTitle = "bAdvancedDisplay"))
		bool GetVariableAdvancedDisplay();
	/* Is the variable Deprecated? */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "GetbDeprecated", CompactNodeTitle = "bDeprecated"))
		bool GetVariableDeprecated();
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "GetbDeprecatedMessage", CompactNodeTitle = "DeprecatedMessge"))
		FString GetVariableDeprecatedMessage();

	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "GetReplicationCondition", CompactNodeTitle = "ReplicationCondition"))
		ELifetimeCondition GetVariableReplicationCondition();

	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "GetRepNotifyFunc", CompactNodeTitle = "RepNotifyFunc"))
		FName GetVariableRepNotifyFunc();
	/* Get how the variable is replicated */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "GetReplication", CompactNodeTitle = "Replication"))
		EBPElgVariableReplication GetVariableReplicationType();

	/* return if the variable is replicated or not */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "GetbReplicated", CompactNodeTitle = "bReplicated"))
		bool GetVariableIsReplicated();


#pragma endregion


	void Setup(UBlueprint* InBlueprint, FBPVariableDescription InVariableDescription);
	void Setup(UBlueprint* InBlueprint, FProperty* InVariableProperty);

#pragma region overrides

	virtual EBPElgBESVariableType GetVarType() override;
	virtual FString GetDefaultValue() override;
	virtual bool ValidateAndUpdate() override;
	virtual FString GetVarTipText() override;

#pragma endregion

	static UElgBESGraphVariable* MakeGraphVariableObject(UBlueprint* InBlueprint, FBPVariableDescription InVariableDescription);
	static UElgBESGraphVariable* MakeGraphVariableObject(UBlueprint* InBlueprint, FProperty* InVariableProperty);

};
