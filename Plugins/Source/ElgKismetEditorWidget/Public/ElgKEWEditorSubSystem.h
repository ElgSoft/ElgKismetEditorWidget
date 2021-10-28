// Copyright 2019-2021 ElgSoft. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include <EditorUtilityWidget.h>
#include <BlueprintEditor.h>
#include "ElgEditorContext_BlueprintEditor.h"
#include "ElgKEWEditorSubSystem.generated.h"

class UObject;

/**
 * 
 */
UCLASS(meta = (DisplayName = "KismetEditorWidgetSS"))
class ELGKISMETEDITORWIDGET_API UElgKEWEditorSubSystem : public UEditorSubsystem
{
	GENERATED_BODY()

public:

#pragma region Init

	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

#pragma endregion

	TMap<UObject*, class TWeakPtr<FBlueprintEditor>> WidgetBPEditorMap;
	
	UPROPERTY()
		TMap<UBlueprint*, UElgEditorContext_BlueprintEditor*> EditorWidgetContextMap;

	/* return and create a context for an KismetEditorWidgt BlueprintEditor */
	UElgEditorContext_BlueprintEditor* GetContext(UObject* InSelf);

	/* one KismetEditorWidget has been closed so we need to clean up anything that use it */
	void ContainerWidgetDestroyed(UBlueprint* Blueprint);

	UElgEditorContext_BlueprintEditor* FindEditorContext(UBlueprint* Blueprint);



};
