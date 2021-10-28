// Copyright 2019-2021 ElgSoft. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "ElgBESGraphEditablePinBase.h"
#include "ElgBESGraphMacroPin.generated.h"

/**
 * Macro Input/output Pin Blueprint Wrapper
 */
UCLASS(Blueprintable)
class ELGKISMETEDITORWIDGET_API UElgBESGraphMacroPin : public UElgBESGraphEditablePinBase
{
	GENERATED_BODY()

public:

	virtual bool SupportExecPins() override;

	static UElgBESGraphMacroPin* MakeGraphMacroPinVariable(UBlueprint* InBlueprint, UEdGraph* InGraph, class UK2Node_EditablePinBase* InNode, UEdGraphPin* InPin);

};
