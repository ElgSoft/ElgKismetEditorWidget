// Copyright 2019-2023 ElgSoft. All rights reserved. 


#include "ElgKEWGraphPinBP.h"
#include <ElgKEWStructs.h>
#include <ElgBESGraphPin.h>



void UElgKEWGraphPinBP::DivideByPinDirection(const TArray<UElgBESGraphPin*> InPins, TArray<UElgBESGraphPin*>& OutInputPins, TArray<UElgBESGraphPin*>& OutOutputPins)
{
	for (UElgBESGraphPin* pin : InPins) {
		if (pin->GetPinDirection() == EBPElgKEWPinDirection::Input) {
			OutInputPins.Add(pin);
		} else {
			OutOutputPins.Add(pin);
		}
	}
}

#pragma region PinFilters

TArray<UElgBESGraphPin*> UElgKEWGraphPinBP::FilterPinDirection(const TArray<UElgBESGraphPin*> InPins, EBPElgKEWPinDirection InDirection)
{
	TArray<UElgBESGraphPin*> outPins;
	for (UElgBESGraphPin* pin : InPins) {
		if (pin->GetPinDirection() == InDirection) {
			outPins.Add(pin);
		}
	}
	return outPins;
}


TArray<UElgBESGraphPin*> UElgKEWGraphPinBP::FilterPinCategory(const TArray<UElgBESGraphPin*> InPins, const FName InCategory)
{
	TArray<UElgBESGraphPin*> outPins;
	for (UElgBESGraphPin* pin : InPins) {
		if (pin->GetPinCategory() == InCategory) {
			outPins.Add(pin);
		}
	}
	return outPins;
}


TArray<UElgBESGraphPin*> UElgKEWGraphPinBP::FilterPinType(const TArray<UElgBESGraphPin*> InPins, const EBPElgKEWPinCategory InType)
{
	TArray<UElgBESGraphPin*> outPins;
	for (UElgBESGraphPin* pin : InPins) {
		if (pin->GetPinType().EPinCategory == InType) {
			outPins.Add(pin);
		}
	}
	return outPins;
}


TArray<UElgBESGraphPin*> UElgKEWGraphPinBP::FilterPinVisible(const TArray<UElgBESGraphPin*> InPins)
{
	TArray<UElgBESGraphPin*> outPins;
	for (UElgBESGraphPin* pin : InPins) {
		if (!pin->GetPinIsHidden()) {
			outPins.Add(pin);
		}
	}
	return outPins;
}


TArray<UElgBESGraphPin*> UElgKEWGraphPinBP::FilterPinData(const TArray<UElgBESGraphPin*> InPins)
{
	TArray<UElgBESGraphPin*> outPins;
	for (UElgBESGraphPin* pin : InPins) {
		if (pin->GetPinType().EPinCategory != EBPElgKEWPinCategory::PC_Exec) {
			outPins.Add(pin);
		}
	}
	return outPins;
}

TArray<UElgBESGraphPin*> UElgKEWGraphPinBP::FilterPinVisibleData(const TArray<UElgBESGraphPin*> InPins)
{
	TArray<UElgBESGraphPin*> outPins;
	for (UElgBESGraphPin* pin : InPins) {
		if (!pin->GetPinIsHidden() && pin->GetPinType().EPinCategory != EBPElgKEWPinCategory::PC_Exec) {
			outPins.Add(pin);
		}
	}
	return outPins;
}


#pragma endregion

