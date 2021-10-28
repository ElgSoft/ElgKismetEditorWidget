// Copyright 2019-2021 ElgSoft. All rights reserved. 


#include "ElgBESGraphEventDispatcherPin.h"



void UElgBESGraphEventDispatcherPin::SetupCustom()
{
	bCanAddExecPins = false;
}


UElgBESGraphEventDispatcherPin* UElgBESGraphEventDispatcherPin::MakeGraphEventDispatcherPin(UBlueprint* InBlueprint, UEdGraph* InGraph, class UK2Node_EditablePinBase* InNode, UEdGraphPin* InPin)
{
	UElgBESGraphEventDispatcherPin* pin = NewObject<UElgBESGraphEventDispatcherPin>();
	pin->Setup(InBlueprint, InGraph, InNode, InPin);
	return pin;
}
