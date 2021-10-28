// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"


class FElgKismetEditorWidgetModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/* Called from OnRegisterTabsForEditor from the blueprint editor module */
	void OnRegisterTabSpawner(class FWorkflowAllowedTabSet& InTabSpawners, FName InModeName, TSharedPtr<class FBlueprintEditor> InBlueprintEditor);

	/* Return the active BlueprintEditor or nullptr */
	TWeakPtr<FBlueprintEditor> GetBlueprintEditor();

	static inline FElgKismetEditorWidgetModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FElgKismetEditorWidgetModule>("ElgKismetEditorWidget");
	}

private:
	/** Pointer back to the kismet 2 tool that owns us */
	TWeakPtr<FBlueprintEditor> BlueprintEditorPtr;
};


