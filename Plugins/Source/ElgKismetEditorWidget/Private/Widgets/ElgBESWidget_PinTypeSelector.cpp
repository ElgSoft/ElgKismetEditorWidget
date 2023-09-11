// Copyright 2019-2023 ElgSoft. All rights reserved. 


#include "Widgets/ElgBESWidget_PinTypeSelector.h"
#include <Kismet2/BlueprintEditorUtils.h>
#include <SPinTypeSelector.h>
#include <EditorUtilityWidget.h>
#include <ElgKEWUtils.h>

#define LOCTEXT_NAMESPACE "FElgKismetEditorWidgetModule"

UElgBESWidget_PinTypeSelector::UElgBESWidget_PinTypeSelector(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SPinTypeSelector::FArguments Defaults;
	VariablePinType = FElgKEWUtils::GetDefaultPinType();
	PinTypeFilter = EBPElgEPinTypeFilter::None;
	
}


void UElgBESWidget_PinTypeSelector::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

void UElgBESWidget_PinTypeSelector::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyPinTypeSelector.Reset();
}

void UElgBESWidget_PinTypeSelector::PostLoad()
{
	Super::PostLoad();
}


#if WITH_EDITOR

const FText UElgBESWidget_PinTypeSelector::GetPaletteCategory()
{
	return LOCTEXT("PinTypeSelector", "Input");
}

#endif


void UElgBESWidget_PinTypeSelector::SetPinType(FS_ElgGraphPinType InPinType)
{
	VariablePinType = InPinType;
}

FS_ElgGraphPinType UElgBESWidget_PinTypeSelector::GetPinType()
{
	return VariablePinType;
}



TSharedRef<SWidget> UElgBESWidget_PinTypeSelector::RebuildWidget()
{
	const UEdGraphSchema_K2* Schema = GetDefault<UEdGraphSchema_K2>();

	MyPinTypeSelector = SNew(SPinTypeSelector, FGetPinTypeTree::CreateUObject(Schema, &UEdGraphSchema_K2::GetVariableTypeTree))
		.TargetPinType(TAttribute<FEdGraphPinType>::Create(TAttribute<FEdGraphPinType>::FGetter::CreateUObject(this, &UElgBESWidget_PinTypeSelector::HandleOnGetVarType)))
		.OnPinTypeChanged(FOnPinTypeChanged::CreateUObject(this, &UElgBESWidget_PinTypeSelector::HandleOnVarTypeChanged))
		.IsEnabled(true)
		.Schema(Schema)
		.TypeTreeFilter(FElgKEWUtils::ConvertFromPinTypeFilter(PinTypeFilter))
		;
	//.ToolTip(VarTypeTooltip)
	return MyPinTypeSelector.ToSharedRef();
}

FEdGraphPinType UElgBESWidget_PinTypeSelector::HandleOnGetVarType() const
{
	return VariablePinType.EdGraphPinType;
}

void UElgBESWidget_PinTypeSelector::HandleOnVarTypeChanged(const FEdGraphPinType& NewPinType)
{
	if (FBlueprintEditorUtils::IsPinTypeValid(NewPinType)) {
		//VariablePinType = FElgKEWUtils::ConvertToPinType()
		VariablePinType = FElgKEWUtils::ConvertToPinType(NewPinType);
		OnPinTypeChanged.Broadcast(VariablePinType);
	}
}



#undef LOCTEXT_NAMESPACE