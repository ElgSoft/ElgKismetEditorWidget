// Copyright 2019-2023 ElgSoft. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "BPWrappers/ElgBESGraphEditablePinBase.h"
#include "ElgBESGraphEventDispatcherPin.generated.h"

/**
 * Blueprint wrapper for a EventDispatcherPin, can be edit in the editor.
 */
UCLASS(Blueprintable)
class ELGKISMETEDITORWIDGET_API UElgBESGraphEventDispatcherPin : public UElgBESGraphEditablePinBase
{
	GENERATED_BODY()

public:

	virtual void SetupCustom() override;


	static UElgBESGraphEventDispatcherPin* MakeGraphEventDispatcherPin(UBlueprint* InBlueprint, UEdGraph* InGraph, class UK2Node_EditablePinBase* InNode, UEdGraphPin* InPin);

};
