// Copyright 2019-2023 ElgSoft. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SWidget.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class ELGKISMETEDITORWIDGET_API SElgKEWViewerWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SElgKEWViewerWidget) {}
	SLATE_ARGUMENT(TWeakPtr<class FBlueprintEditor>, Kismet2)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual ~SElgKEWViewerWidget() override;

	void HandleOnCompiled(UBlueprint* InBlueprint);

protected:
	class UEditorUtilityWidget* editorWidget = nullptr;

	/* Build the widget using the EditorWidget */
	void MakeKismetEditorWidget();

	TSharedRef<SWidget> MakeWidgetPicker();

	/*  */
	TSharedPtr<SComboButton> AssetPickerAnchor;
	void OnAssetSelectedFromPicker(const FAssetData& InAssetData);
	void OnAssetEnterPressedInPicker(const TArray<FAssetData>& InSelectedAssets);

	/*  */
	FText OnGetComboTextValue() const;

	/* Get the color for the combobox based on if its hovered or not */
	FSlateColor OnGetComboForeground() const;
	FSlateColor OnGetWidgetForeground() const;
	FSlateColor OnGetWidgetBackground() const;

	/* The current selected asset data in the combobox */
	FAssetData CurrentAssetData;

	/** Border widget that wraps a dynamic context-sensitive widget for editing objects that the property window is displaying */
	TSharedPtr<SBorder> ContextualBorderWidget;

	TSharedPtr<SVerticalBox> ContextualVerticalWidget;

	TWeakPtr<FBlueprintEditor> BlueprintEditorPtr;
};
