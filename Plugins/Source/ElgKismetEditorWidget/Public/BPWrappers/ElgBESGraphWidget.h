// Copyright 2019-2021 ElgSoft. All rights reserved. 

#pragma once

#include "ElgBESBase.h"
#include "ElgBESGraphWidget.generated.h"

class UBlueprint;
class UWidget;
class UElgBESGraphWidgetVariable;


/**
 * Blueprintable representation for a Widget in the WidgetTree
 */
UCLASS(Blueprintable) 
class ELGKISMETEDITORWIDGET_API UElgBESGraphWidget : public UElgBESBase
{

	GENERATED_BODY()

public:

	UPROPERTY()
		FName WidgetName;
	
	UWidget* WidgetPtr;
	UBlueprint* BlueprintPtr;


	/* Set if the widget is exposed as a variable, if ShowDialog is true a warning will be shown if the variable is used in the graph. */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable")
		void SetIsVariable(const bool bIsVariable, const bool bShowDialog = false);

	/* Get if the widget is exposed as a variable or not */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Widget", meta = (DisplayName = "GetIsVariable", CompactNodeTitle = "IsVariable"))
		bool GetIsVariable();

	/* Icon representing the variable's "edit-on-instance" state. */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Widget", meta = (DisplayName = "GetStateIcon", CompactNodeTitle = "StateIcon"))
		void GetStateIcon(FSlateBrush& OutBrush);

	/* Get the WidgetVariable from this, if it's exposed as an variable otherwise it will be null */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Widget")
		UElgBESGraphWidgetVariable* GetWidgetVariable();


	virtual FName GetNameInternal() override;
	virtual void GetIconInternal(struct FSlateBrush& OutBrush) override;
	virtual FText GetToolTipInternal() override;


	void Setup(UBlueprint* InBlueprint, UWidget* InWidget);

	static UElgBESGraphWidget* MakeGraphWidget(UBlueprint* InBlueprint, UWidget* InWidget);

};
