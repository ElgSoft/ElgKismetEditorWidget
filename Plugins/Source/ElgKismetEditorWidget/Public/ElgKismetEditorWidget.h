// Copyright 2019-2023 ElgSoft. All rights reserved.

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

	static FElgKismetEditorWidgetModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FElgKismetEditorWidgetModule>("ElgKismetEditorWidget");
	}

private:
	/** Pointer back to the kismet 2 tool that owns us */
	TWeakPtr<FBlueprintEditor> BlueprintEditorPtr;
};


