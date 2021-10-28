// Copyright 2019-2021 ElgSoft. All rights reserved. 


#include "BPWrappers/ElgBESGraphWidget.h"
#include <Kismet2/BlueprintEditorUtils.h>
#include <Components/Widget.h>
#include <Engine/Blueprint.h>
#include <Styling/SlateIconFinder.h>
#include <ElgKEWVariableBP.h>
#include <ElgBESGraphWidgetVariable.h>
#include "ElgKEW_Log.h"

#define LOCTEXT_NAMESPACE "FElgKismetEditorWidgetModule"

void UElgBESGraphWidget::SetIsVariable(const bool bIsVariable, const bool bShowDialog)
{
	if (GetIsVariable() == bIsVariable) return;
	
	if (WidgetPtr->bIsVariable && bShowDialog) {
		if (UElgBESGraphWidgetVariable* variable = GetWidgetVariable()) {
			if (variable->IsInUse()) {
				EAppReturnType::Type DialogReturn = EAppReturnType::No;
				DialogReturn = FMessageDialog::Open(EAppMsgType::YesNoCancel, NSLOCTEXT("UnrealEd", "WidgetVariable", "The variable is referenced in the blueprint graph, are you sure you want hide/remove it?"));
				if (DialogReturn == EAppReturnType::Cancel || DialogReturn == EAppReturnType::No) return;
			}
		}	
	}

	const FScopedTransaction Transaction(LOCTEXT("VariableToggle", "Widget Variable Toggle"));
	WidgetPtr->Modify();
	WidgetPtr->bIsVariable = bIsVariable;
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(BlueprintPtr);
}


bool UElgBESGraphWidget::GetIsVariable()
{
	return WidgetPtr->bIsVariable;
}



void UElgBESGraphWidget::GetStateIcon(FSlateBrush& OutBrush)
{
	UElgKEWVariableBP::GetVariableExposedIcon(WidgetPtr->bIsVariable, OutBrush);
}


UElgBESGraphWidgetVariable* UElgBESGraphWidget::GetWidgetVariable()
{
	if (!WidgetPtr->bIsVariable) {
		UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("You can not get a widget variable if the widget is not exposed as an variable!!"));
		return nullptr;
	}

	if (FProperty* variableProperty = FindFProperty<FProperty>(BlueprintPtr->SkeletonGeneratedClass, WidgetName)) {
		return UElgBESGraphWidgetVariable::MakeGraphWidgetVariable(BlueprintPtr, variableProperty);
	}
	UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("Failed to find a FProperty for Widget Variable [%s]!"), *WidgetName.ToString());
	return nullptr;
}


FName UElgBESGraphWidget::GetNameInternal()
{
	return WidgetName;
}

void UElgBESGraphWidget::GetIconInternal(struct FSlateBrush& OutBrush)
{
	const FSlateBrush* brush = FSlateIconFinder::FindIconBrushForClass(WidgetPtr->GetClass());
	OutBrush = *brush;
}

FText UElgBESGraphWidget::GetToolTipInternal()
{
	return UElgKEWVariableBP::GetVariableExposedTipText(WidgetPtr->bIsVariable);
}

void UElgBESGraphWidget::Setup(UBlueprint* InBlueprint, UWidget* InWidget)
{
	BlueprintPtr = InBlueprint;
	WidgetPtr = InWidget;
	WidgetName = WidgetPtr->GetFName();

}

UElgBESGraphWidget* UElgBESGraphWidget::MakeGraphWidget(UBlueprint* InBlueprint, UWidget* InWidget)
{
	UElgBESGraphWidget* bpVariable = NewObject<UElgBESGraphWidget>();
	bpVariable->Setup(InBlueprint, InWidget);
	return bpVariable;

}

#undef LOCTEXT_NAMESPACE