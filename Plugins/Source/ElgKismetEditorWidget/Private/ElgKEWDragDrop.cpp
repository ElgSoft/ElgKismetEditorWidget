// Copyright 2019-2021 ElgSoft. All rights reserved. 


#include "ElgKEWDragDrop.h"

#include <ElgKEWUtils.h>
#include <ElgKEWEditorSubSystem.h>
#include <ElgEditorContext_BlueprintEditor.h>
#include "ElgKEW_Log.h"


void UElgKEWDragDropBase::DragCancelled_Implementation(const FPointerEvent& PointerEvent)
{
	if (Payload == nullptr) {
		UE_LOG(LogElgKismetEditorWidget, Error, TEXT("Invalid Payload! Any Kismet Editor Widget drag and drop need´s the widget as a input (self)"));
		OnDragCancelled.Broadcast(this);
		return; 
	}

	if (NameOfDragDropObject.IsNone()) {
		UE_LOG(LogElgKismetEditorWidget, Error, TEXT("Invalid NameOfDragDropObject! Any Kismet Editor Widget drag and drop need´s the NameOfDragDropObject to be set to the name of whatever you want to drag and drop"));
		OnDragCancelled.Broadcast(this);
		return;
	}

	UObject* editorWidget = FElgKEWUtils::GetEditorWidgetParent(Payload);
	if (editorWidget) {
		UElgKEWEditorSubSystem* kewSS = GEditor->GetEditorSubsystem<UElgKEWEditorSubSystem>();
		UElgEditorContext_BlueprintEditor* context = kewSS->GetContext(editorWidget);
		if (context) {
			context->HandleDragAndDrop(PointerEvent, this);
		}	
	}

	OnDragCancelled.Broadcast(this);
}

void UElgKEWDragDropBase::Dragged_Implementation(const FPointerEvent& PointerEvent)
{
	// could do some check here to see if you can drop the widget here.
}
