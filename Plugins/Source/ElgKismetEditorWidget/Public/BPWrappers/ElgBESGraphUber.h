// Copyright 2019-2023 ElgSoft. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "ElgBESBase.h"
#include "ElgBESGraphUber.generated.h"

class UElgBESGraphNode;
struct FSlateBrush;



/**
 * BP wrapper for Uber Graphs
 */
UCLASS(Blueprintable)
class ELGKISMETEDITORWIDGET_API UElgBESGraphUber : public UElgBESBase
{
	GENERATED_BODY()

public:
	FString GraphName;

	/* Removed the Uber graph, can't remove the default EventGraph
	* bShowDialog if true and the graph has any nodes a Dialog is show to let the user decide if it should be removed or not.* 
	*/
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|UberGraph")
		void Remove(const bool bShowDialog = false);
	
	/* Open the Uber graph */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|UberGraph")
		void Open();

	/* Set the name of the Uber graph, can't rename the default EventGraph */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|UberGraph")
		void SetName(FName InName);

	/* Return the graph pointer */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|UberGraph")
		UEdGraph* GetGraph();

	/* Return all nodes in this UberGraph */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|UberGraph")
		TArray<UElgBESGraphNode*> GetNodes();

	/* Return all event nodes in this UberGraph */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|UberGraph")
		TArray<UElgBESGraphNode*> GetEventNodes();

	/* Return if the UberGraph can be removed */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|UberGraph", meta=(DisplayName = "DoAllowDeletion", CompactNodeTitle = "bAllowDeletion"))
		bool DoAllowDeletion();


	virtual void ShowInDetailsInternal() override;
	virtual void SelectInMyBlueprintInternal() override;
	virtual FName GetNameInternal() override;
	virtual void GetIconInternal(struct FSlateBrush& OutBrush) override;
	virtual int32 GetCountInternal() override;


	void Setup(UEdGraph* InGraph);

	bool ValidateUber();

	static UElgBESGraphUber* MakeGraphUberObject(UEdGraph* InGraph);

};

