// Copyright 2019-2023 ElgSoft. All rights reserved.


#include "BPWrappers/ElgBESBase.h"
#include <Styling/SlateBrush.h>
#include <ElgEditorContext_BlueprintEditor.h>
#include <ElgKEWEditorSubSystem.h>
#include "Styling/AppStyle.h"


void UElgBESBase::ShowInDetails()
{
	ShowInDetailsInternal();
}

void UElgBESBase::SelectInMyBlueprint()
{
	SelectInMyBlueprintInternal();
}

FName UElgBESBase::GetBaseName()
{
	return GetNameInternal();
}


FString UElgBESBase::GetNameAsString()
{
	return GetNameInternal().ToString();
}


FText UElgBESBase::GetToolTip()
{
	return GetToolTipInternal();
}


FText UElgBESBase::GetCategory()
{
	return GetCategoryInternal();
}


FString UElgBESBase::GetCategoryAsString()
{
	return GetCategoryInternal().ToString();
}


int32 UElgBESBase::GetUsage()
{
	return GetCountInternal();
}


bool UElgBESBase::IsInUse()
{
	if (GetCountInternal()) return true;
	return false;
}


void UElgBESBase::GetIcon(struct FSlateBrush& OutBrush)
{
	GetIconInternal(OutBrush);
}





void UElgBESBase::ShowInDetailsInternal()
{

}

void UElgBESBase::SelectInMyBlueprintInternal()
{
	if (BaseName.IsNone()) return;
	UElgKEWEditorSubSystem* kewSS = GEditor->GetEditorSubsystem<UElgKEWEditorSubSystem>();
	if (UElgEditorContext_BlueprintEditor* context = kewSS->FindEditorContext(BlueprintPtr)) {
		context->SelectItemInMyBlueprint(BaseName);
	}
}

FName UElgBESBase::GetNameInternal()
{
	return NAME_None;
}


FText UElgBESBase::GetToolTipInternal()
{
	return FText();
}


FText UElgBESBase::GetCategoryInternal()
{
	return FText::FromString(TEXT("Default"));
}




int32 UElgBESBase::GetCountInternal()
{
	return 0;
}


void UElgBESBase::GetIconInternal(struct FSlateBrush& OutBrush)
{
	FLinearColor tint = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	OutBrush = *FAppStyle::GetBrush(TEXT("GraphEditor.PureFunction_16x"));
	OutBrush.TintColor = tint;
}
