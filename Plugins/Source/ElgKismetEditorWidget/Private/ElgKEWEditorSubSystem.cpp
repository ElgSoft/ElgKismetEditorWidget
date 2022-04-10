// Copyright 2019-2021 ElgSoft. All rights reserved. 


#include "ElgKEWEditorSubSystem.h"
#include <ElgKismetEditorWidget.h>
#include <BlueprintEditor.h>
#include <EditorUtilityWidgetBlueprint.h>
#include <ElgEditorContext_BlueprintEditor.h>


#pragma region Init

void UElgKEWEditorSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
}

void UElgKEWEditorSubSystem::Deinitialize()
{
}				

#pragma endregion



UElgEditorContext_BlueprintEditor* UElgKEWEditorSubSystem::GetContext(UObject* InSelf)
{
	const TWeakPtr<FBlueprintEditor>* editorPtr = WidgetBPEditorMap.Find(InSelf);
	if (!editorPtr) {
		return nullptr; // if there are no Editor for this widget we can't do anything.
	}
	TWeakPtr<FBlueprintEditor> editor = *editorPtr;
	UBlueprint* bp = editor.Pin()->GetBlueprintObj();

	// need to check if there already are an editor for the object
	UElgEditorContext_BlueprintEditor* context = FindEditorContext(bp);
	if (context) {
		// the editor might have been destroyed so make sure it's valid
		// no need to create a new object just update it.
		context->UpdateBlueprintEditor(editor);
		return context;
	}

	UElgEditorContext_BlueprintEditor* editorContext = NewObject<UElgEditorContext_BlueprintEditor>();
	editorContext->Setup(editor);
	EditorWidgetContextMap.Add(bp, editorContext);
	return editorContext;
}


void UElgKEWEditorSubSystem::ContainerWidgetDestroyed(UBlueprint* Blueprint)
{
	if (UElgEditorContext_BlueprintEditor* context = FindEditorContext(Blueprint)) {
		context->Cleanup();
		EditorWidgetContextMap.Remove(Blueprint);
	}	
}

UElgEditorContext_BlueprintEditor* UElgKEWEditorSubSystem::FindEditorContext(UBlueprint* Blueprint)
{
	auto* context = EditorWidgetContextMap.Find(Blueprint);
	if (context) {
		return *context;
	}
	return nullptr;
}

#pragma endregion