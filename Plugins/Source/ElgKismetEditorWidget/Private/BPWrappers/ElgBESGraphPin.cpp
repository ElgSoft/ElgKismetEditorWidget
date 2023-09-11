// Copyright 2019-2023 ElgSoft. All rights reserved. 


#include "BPWrappers/ElgBESGraphPin.h"
#include <ElgKEWUtils.h>
#include "ElgKEWEnum.h"
#include "ElgKEWStructs.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet2/KismetEditorUtilities.h>
#include <ElgKEWEditorSubSystem.h>
#include <ElgEditorContext_BlueprintEditor.h>
#include "ElgKEW_Log.h"


void UElgBESGraphPin::Setup(UEdGraphPin* InPin)
{
	PinName = InPin->PinName;
	PinId = InPin->PinId;
	NodePtr = InPin->GetOwningNode();
	GraphPtr = Cast<UEdGraph>(NodePtr->GetOuter());
	BlueprintPtr = Cast<UBlueprint>(GraphPtr->GetOuter());	
}


void UElgBESGraphPin::SelectPin()
{
	if (UEdGraphPin* pin = GetPin()) {
		FKismetEditorUtilities::BringKismetToFocusAttentionOnPin(pin);
	}
}

void UElgBESGraphPin::BreakAllPinLinks(bool bNotifyNodes /*= true*/)
{
	if (UEdGraphPin* pin = GetPin()) {
		pin->BreakAllPinLinks(bNotifyNodes);
	}
}

void UElgBESGraphPin::BreakLinkTo(UElgBESGraphPin* InPinToBreak)
{
	if (!InPinToBreak) return;
	if (UEdGraphPin* pin = GetPin()) {
		if (UEdGraphPin* pinToBreak = InPinToBreak->GetPin()) {
			pin->BreakLinkTo(pinToBreak);
		}		
	}
}

void UElgBESGraphPin::MakeLinkTo(UElgBESGraphPin* InPinToLink)
{
	if (!InPinToLink) return;
	UEdGraphPin* pin = GetPin();
	UEdGraphPin* pinLink = InPinToLink->GetPin();
	if (pin && pinLink) {
		pin->MakeLinkTo(pinLink);
	}
}

void UElgBESGraphPin::ResetDefaultValue()
{
	if (UEdGraphPin* pin = GetPin()) {
		pin->ResetDefaultValue();
	}		
}


void UElgBESGraphPin::PromoteToVariable(const bool bInToMemberVariable)
{
	UElgKEWEditorSubSystem* kewSS = GEditor->GetEditorSubsystem<UElgKEWEditorSubSystem>();
	if (UElgEditorContext_BlueprintEditor* context = kewSS->GetContext(BlueprintPtr)) {
		if (GetDefault<UEdGraphSchema_K2>()->CanPromotePinToVariable(*GetPin(), bInToMemberVariable)) {
			if (context->BlueprintEditorPtr.IsValid()) {
				context->BlueprintEditorPtr.Pin()->DoPromoteToVariable(BlueprintPtr, GetPin(), bInToMemberVariable);
			}
		}
	}
}


#pragma region PinGetters

bool UElgBESGraphPin::IsVisibleData()
{
	if (!GetPinIsHidden()&& GetPinType().EPinCategory != EBPElgKEWPinCategory::PC_Exec) return true;
	return false;
}

FName UElgBESGraphPin::GetPinCleanName()
{
	if (UEdGraphPin* pin = GetPin()) {
		if (pin->bAllowFriendlyName) {
			FName name = FName(*pin->PinFriendlyName.ToString());
			if (name.IsValid() && !name.IsNone()) return name;
		}
		return pin->PinName;
	}
	return FName();
}

FString UElgBESGraphPin::GetType()
{
	if (UEdGraphPin* pin = GetPin()) {
		if (pin->PinType.PinSubCategoryObject.IsValid()) {
			return UKismetSystemLibrary::GetDisplayName(pin->PinType.PinSubCategoryObject.Get());
		}
		return pin->PinType.PinCategory.ToString();
	}
	return FString();
}

FName UElgBESGraphPin::GetPinName()
{
	return PinName;
}

FGuid UElgBESGraphPin::GetPinId()
{
	return PinId;
}

FGuid UElgBESGraphPin::GetPinGuid()
{
	return PinId;
}

FString UElgBESGraphPin::GetPinGuidAsString()
{
	return PinId.ToString();
}

bool UElgBESGraphPin::GetPinConnected()
{
	if (UEdGraphPin* pin = GetPin()) {
		return pin->HasAnyConnections();
	}
	return false;
}

bool UElgBESGraphPin::GetPinIsHidden()
{
	if (UEdGraphPin* pin = GetPin()) {
		return pin->bHidden;
	}

	return false;
}

TArray<UElgBESGraphPin*> UElgBESGraphPin::GetPinLinkedTo()
{
	TArray<UElgBESGraphPin*> pins;
	if (!NodePtr) return pins;
	if (!BlueprintPtr) return pins;
	
	for (UEdGraphPin* pin : GetPin()->LinkedTo) {		
		pins.Add(MakeGraphPinObject(pin));
	}
	return pins;
}

FName UElgBESGraphPin::GetPinCategory()
{
	if (UEdGraphPin* pin = GetPin()) {
		return pin->PinType.PinCategory;
	}
	return FName();
}

FName UElgBESGraphPin::GetPinSubCategory()
{
	if (UEdGraphPin* pin = GetPin()) {
		return pin->PinType.PinSubCategory;
	}
	return FName();
}

UObject* UElgBESGraphPin::GetPinSubObject()
{
	if (UEdGraphPin* pin = GetPin()) {
		TWeakObjectPtr<UObject> obj = pin->PinType.PinSubCategoryObject;
		if (obj.IsValid()) return obj.Get();
	}
	return nullptr;
}

bool UElgBESGraphPin::GetPinIsInput()
{
	if (UEdGraphPin* pin = GetPin()) {
		return (pin->Direction == EEdGraphPinDirection::EGPD_Input) ? true : false;
	}
	return false;	
}

bool UElgBESGraphPin::GetPinIsOutput()
{
	if (UEdGraphPin* pin = GetPin()) {
		return (pin->Direction == EEdGraphPinDirection::EGPD_Output) ? true : false;
	}
	return false;
}

EBPElgKEWPinDirection UElgBESGraphPin::GetPinDirection()
{
	if (UEdGraphPin* pin = GetPin()) {
		EBPElgKEWPinDirection pinDirection = (pin->Direction == EEdGraphPinDirection::EGPD_Input) ? EBPElgKEWPinDirection::Input : EBPElgKEWPinDirection::Output;
		return pinDirection;
	}
	return EBPElgKEWPinDirection::Input;
}


FString UElgBESGraphPin::GetPinTipText()
{
	if (UEdGraphPin* pin = GetPin()) {
		pin->PinToolTip;
	}
	return FString();
}

FS_ElgGraphPinType UElgBESGraphPin::GetPinType()
{
	if (UEdGraphPin* pin = GetPin()) {
		return FElgKEWUtils::ConvertToPinType(pin->PinType);
	} 
	return FElgKEWUtils::GetDefaultPinType();
}

FString UElgBESGraphPin::GetPinDefaultValue()
{
	if (UEdGraphPin* pin = GetPin()) {
		return pin->GetDefaultAsString();
	}
	return FString();
}

FString UElgBESGraphPin::GetPinAutogeneratedDefaultValue()
{
	if (UEdGraphPin* pin = GetPin()) {
		return pin->AutogeneratedDefaultValue;
	}
	return FString();
}

UObject* UElgBESGraphPin::GetPinDefaultObject()
{
	if (UEdGraphPin* pin = GetPin()) {
		return pin->DefaultObject;
	}
	return nullptr;	
}

FText UElgBESGraphPin::GetPinDefaultTextValue()
{
	if (UEdGraphPin* pin = GetPin()) {
		return pin->GetDefaultAsText();
	}
	return FText();	
}

bool UElgBESGraphPin::GetPinNotConnectable()
{
	if (UEdGraphPin* pin = GetPin()) {
		return pin->bNotConnectable;
	}
	return false;	
}

bool UElgBESGraphPin::GetPinDefaultValueIsReadOnly()
{
	if (UEdGraphPin* pin = GetPin()) {
		return pin->bDefaultValueIsReadOnly;
	}
	return false;	
}

bool UElgBESGraphPin::GetPinDefaultValueIsIgnored()
{
	if (UEdGraphPin* pin = GetPin()) {
		return pin->bDefaultValueIsIgnored;
	}
	return false;	
}

bool UElgBESGraphPin::GetPinbIsDiffing()
{
	//#fixme: remove or update this to use SGraphPanel::DiffResults or something.
	if (UEdGraphPin* pin = GetPin()) {
		return false;
	}
	return false;
}

bool UElgBESGraphPin::GetPinbDisplayAsMutableRef()
{
	if (UEdGraphPin* pin = GetPin()) {
		return pin->bDisplayAsMutableRef;
	}
	return false;
}

bool UElgBESGraphPin::GetPinAllowFriendlyName()
{
	if (UEdGraphPin* pin = GetPin()) {
		return pin->bAllowFriendlyName;
	}
	return false;	
}

FText UElgBESGraphPin::GetPinPinFriendlyName()
{
	if (GetPin()) {
		return GetPin()->PinFriendlyName;
	}
	return FText();	
}


bool UElgBESGraphPin::GetIsValid()
{
	if (NodePtr && PinId.IsValid()) {
		if (NodePtr->FindPinById(PinId)) {
			return true;
		}
	}
	return false;
}

void UElgBESGraphPin::IsValidBranch(EBPElgKEWWResult& Branch)
{
	Branch = GetIsValid() ? EBPElgKEWWResult::outTrue : EBPElgKEWWResult::outFalse;
}

#pragma endregion


class UEdGraphPin* UElgBESGraphPin::GetPin()
{
	if (NodePtr && PinId.IsValid()) {
		if (UEdGraphPin* pin = NodePtr->FindPinById(PinId)) {
			return pin;
		}
	}

	FString message = TEXT("Unknown");
	if (PinName.IsValid()) {
		message = PinName.ToString();
	}
	UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("Pin [%s] is no longer valid!"), *message);
	return nullptr;
}


UElgBESGraphPin* UElgBESGraphPin::MakeGraphPinObject(UEdGraphPin* InPin)
{
	UElgBESGraphPin* bpPin = NewObject<UElgBESGraphPin>();
	bpPin->Setup(InPin);
	return bpPin;
}

