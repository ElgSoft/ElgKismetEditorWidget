// Copyright 2019-2023 ElgSoft. All rights reserved. 


#include "BPWrappers/ElgBESGraphMacroPin.h"
#include <ElgKEWUtils.h>

#define LOCTEXT_NAMESPACE "FElgKismetEditorWidgetModule"

bool UElgBESGraphMacroPin::SupportExecPins()
{
	return true;
}

UElgBESGraphMacroPin* UElgBESGraphMacroPin::MakeGraphMacroPinVariable(UBlueprint* InBlueprint, UEdGraph* InGraph, class UK2Node_EditablePinBase* InNode, UEdGraphPin* InPin)
{
	UElgBESGraphMacroPin* pinVar = NewObject<UElgBESGraphMacroPin>();
	pinVar->Setup(InBlueprint, InGraph, InNode, InPin);
	return pinVar;
}


#undef LOCTEXT_NAMESPACE