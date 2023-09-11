// Copyright 2019-2023 ElgSoft. All rights reserved. 

#pragma once

#include "ElgKEWEnum.h"
#include "ElgBESBase.h"
#include "ElgBESGraphNode.generated.h"

class UElgBESGraphPin;
/**
 * BP wrapper for Nodes placed in a Blueprint
 */
UCLASS(Blueprintable)
class ELGKISMETEDITORWIDGET_API UElgBESGraphNode : public UElgBESBase
{
	GENERATED_BODY()
public:

	UPROPERTY()
		FGuid Guid;
	UPROPERTY()
		FString NodeName;

	UPROPERTY()
		FString MemberName;
	
	UPROPERTY()
		FString ParentName;
	
	UPROPERTY()
		FString ParentPath;
	
	UEdGraphNode* NodePtr;



	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Node")
		void SelectNode();

	/* Return all the visible input data pins */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Node", meta = (DisplayName = "GetInputPins", CompactNodeTitle = "InputPins"))
		void GetNodeInputPins(TArray<UElgBESGraphPin*>& OutPins);
	/* Return all the visible output data pins */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Node", meta = (DisplayName = "GetOutputPins", CompactNodeTitle = "OutputPins"))
		void GetNodeOutputPins(TArray<UElgBESGraphPin*>& OutPins);

#pragma region NodeGetters

	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Node", meta = (DisplayName = "GetIconTint", CompactNodeTitle = "IconTint"))
		void GetTintColor(struct FLinearColor& OutTint);

	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Node", meta = (DisplayName = "GetGuid", CompactNodeTitle = "Guid"))
		FGuid GetNodeGuid();

	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Node", meta = (DisplayName = "GetGuidAsString", CompactNodeTitle = "GuidString"))
		FString GetNodeGuidAsString();

	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Node", meta = (DisplayName = "GetMemberName", CompactNodeTitle = "MemberName"))
		FString GetNodeMemberName();
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Node", meta = (DisplayName = "GetMemberParent", CompactNodeTitle = "MemberParent"))
		FString GetNodeMemberParent();
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Node", meta = (DisplayName = "GetMemberParentPath", CompactNodeTitle = "MemberParentPath"))
		FString GetNodeMemberParentPath();
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Node", meta = (DisplayName = "GetMemberPath", CompactNodeTitle = "MemberPath"))
		FString GetNodeMemberPath();

	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Node", meta = (DisplayName = "GetType", CompactNodeTitle = "Type"))
		FString GetNodeType();

	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Node", meta = (DisplayName = "GetPins", CompactNodeTitle = "Pins"))
		void GetNodePins(TArray<UElgBESGraphPin*>& OutPins);

	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Node", meta = (DisplayName = "GetPosition", CompactNodeTitle = "Position"))
		FVector2D GetNodePosition();

	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Node", meta = (DisplayName = "GetTipText", CompactNodeTitle = "TipText"))
		FString GetNodeTipText();


	/* return the graph that this node are in. */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Node", meta = (DisplayName = "GetGraph", CompactNodeTitle = "Graph"))
		UEdGraph* GetGraph();

	/* return the name of the graph that this node are in. */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Node", meta = (DisplayName = "GetGraphName", CompactNodeTitle = "GraphName"))
		FString GetGraphName();


	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Node", meta = (DisplayName = "GetIsValid", CompactNodeTitle = "IsValid"))
		bool GetIsValid();

	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Node", meta = (ExpandEnumAsExecs = "Branch"))
		void IsValidBranch(EBPElgKEWWResult& Branch);

	/* Return if the node is connected to anything */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Node", meta = (DisplayName = "GetIsConnected", CompactNodeTitle = "IsConnected"))
		bool IsConnected();

	/* Return if the node is connected to anything */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Node", meta = (ExpandEnumAsExecs = "Branch"))
		void IsConnectedBranch(EBPElgKEWWResult& Branch);



#pragma endregion

	virtual FName GetNameInternal() override;
	virtual void GetIconInternal(struct FSlateBrush& OutBrush) override;


	void Setup(UEdGraphNode* InNode);

	bool ValidateNode();

	static UElgBESGraphNode* MakeGraphNodeObject(UEdGraphNode* InNode);
};
