// Copyright 2019-2023 ElgSoft. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "ElgKEWSettings.generated.h"


USTRUCT(BlueprintType)
struct FS_ElgKEWConfig
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, Category = "Widget")
		FText TabLabel;

	// the menu want to have a FSlateIcon...
	//UPROPERTY(EditAnywhere, Category = "Widget")
	//	UTexture2D* TabIcon;

	UPROPERTY(EditAnywhere, Category = "Widget")
		FText MenuDescription;

	UPROPERTY(EditAnywhere, Category = "Widget")
		FText MenuToolTip;
	
	UPROPERTY(EditAnywhere, Category = "Widget", meta = (AllowedClasses = "EditorUtilityWidgetBlueprint"))
		FSoftObjectPath EditorWidget;

	UPROPERTY(EditAnywhere, Category = "Widget")
		TEnumAsByte<EVerticalAlignment> VerticalAlignment;
	
	UPROPERTY(EditAnywhere, Category = "Widget")
		TEnumAsByte<EHorizontalAlignment> HorizontalAlignment;
};


/**
 *  Editor widgets that can be open and used in Blueprint Editor, the widgets will be 
 *  dockable windows.
 */
UCLASS(config = Engine, defaultconfig, meta = (DisplayName = "Kismet Editor Widgets"))
class ELGKISMETEDITORWIDGET_API UElgKEWSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	/* Editor Widgets to add to the Blueprint Editor Window list.
	*	The Key will be used as the Identifier for the Tab and Widget.
	*/
	UPROPERTY(EditAnywhere, Config, Category = "KismetEditorWidgets", meta = (ConfigRestartRequired = true))
		TMap<FName, FS_ElgKEWConfig> Widgets;

	
};
