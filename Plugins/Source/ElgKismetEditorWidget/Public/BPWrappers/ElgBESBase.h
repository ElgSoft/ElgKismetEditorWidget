// Copyright 2019-2021 ElgSoft. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ElgBESBase.generated.h"

struct FSlateBrush;

/**
 * Base class for for all wrappers
 */
UCLASS(Blueprintable)
class ELGKISMETEDITORWIDGET_API UElgBESBase : public UObject
{
	GENERATED_BODY()
public:

	/** Name of the variable */
	UPROPERTY()
		FName BaseName;

	UBlueprint* BlueprintPtr;
	UEdGraph* GraphPtr;

public:

	/* Show this in the Details window, if it's open. */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		void ShowInDetails();

	/* Select this in MyBlueprint window */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget")
		void SelectInMyBlueprint();


	/* Get the name */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget", meta = (DisplayName = "GetName", CompactNodeTitle = "Name"))
		FName GetBaseName();
	/* Get the name as string */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget", meta = (DisplayName = "GetNameAsString", CompactNodeTitle = "Name"))
		FString GetNameAsString();
	/* Get the tool tip*/
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget", meta = (DisplayName = "GetToolTip", CompactNodeTitle = "TipText"))
		FText GetToolTip();	
	/* Get the category, "Default" if it do not have any set- */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget", meta = (DisplayName = "GetCategory", CompactNodeTitle = "Category"))
		FText GetCategory();
	/* Get the category as a string */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget", meta = (DisplayName = "GetCategoryAsString", CompactNodeTitle = "Category"))
		FString GetCategoryAsString();
	/* How many times are this used in the blueprint? */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget", meta = (DisplayName = "GetUsage", CompactNodeTitle = "Usage"))
		int32 GetUsage();
	/* if this is used anywhere in the blueprint? */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget", meta = (DisplayName = "IsInUse", CompactNodeTitle = "IsInUse"))
		bool IsInUse();
	/* return the icon used for this,  the tint is set on the brush */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget", meta = (DisplayName = "GetIcon", CompactNodeTitle = "Icon"))
		void GetIcon(struct FSlateBrush& OutBrush);

	
	virtual void ShowInDetailsInternal();
	virtual void SelectInMyBlueprintInternal();
	virtual FName GetNameInternal();
	virtual FText GetToolTipInternal();
	virtual FText GetCategoryInternal();
	virtual int32 GetCountInternal();
	virtual void GetIconInternal(struct FSlateBrush& OutBrush);
	

};
