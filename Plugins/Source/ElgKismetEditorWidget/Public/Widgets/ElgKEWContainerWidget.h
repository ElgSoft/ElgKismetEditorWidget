// Copyright 2019-2023 ElgSoft. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class FBlueprintEditor;


/**
 * 
 */
class ELGKISMETEDITORWIDGET_API SElgKEWContainerWidget  : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SElgKEWContainerWidget )	{}
		SLATE_ARGUMENT(TWeakPtr<FBlueprintEditor>, Kismet2)
		SLATE_ARGUMENT(FName, ViewIdentifier)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual ~SElgKEWContainerWidget () override;
	void ConstructEditorWidget();

	void HandleOnCompiled(UBlueprint* InBlueprint);

protected:
	class UEditorUtilityWidget* editorWidget = nullptr;

	TSharedPtr<FName> widgetInfoIdentifier;
	const struct FS_ElgKEWConfig* WidgetInfo = nullptr;

	TWeakPtr<FBlueprintEditor> BlueprintEditorPtr;

	TSharedPtr<SBorder> BorderWidget;	
};
