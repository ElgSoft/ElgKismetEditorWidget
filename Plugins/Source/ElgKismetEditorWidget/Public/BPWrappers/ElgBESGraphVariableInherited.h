// Copyright 2019-2023 ElgSoft. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "BPWrappers/ElgBESGraphVariableBase.h"
#include "ElgBESGraphVariableInherited.generated.h"

/**
 * Blueprint object for a Inherited variable
 */ 
UCLASS(Blueprintable)
class ELGKISMETEDITORWIDGET_API UElgBESGraphVariableInherited : public UElgBESGraphVariableBase
{
	GENERATED_BODY()

public:

	void Setup(UBlueprint* InBlueprint, FProperty* InVariableProperty);

#pragma region overrides
	virtual EBPElgBESVariableType GetVarType() override;
	virtual bool ValidateAndUpdate() override;

#pragma endregion

	static UElgBESGraphVariableInherited* MakeGraphVariable(UBlueprint* InBlueprint, FProperty* InVariableProperty);

};
