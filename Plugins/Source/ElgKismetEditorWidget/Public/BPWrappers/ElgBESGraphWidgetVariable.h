// Copyright 2019-2021 ElgSoft. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "BPWrappers/ElgBESGraphVariableBase.h"
#include "ElgBESGraphWidgetVariable.generated.h"

/**
 * Widget Variable
 */
UCLASS(Blueprintable)
class ELGKISMETEDITORWIDGET_API UElgBESGraphWidgetVariable : public UElgBESGraphVariableBase
{
	GENERATED_BODY()

public:

	class UWidget* WidgetPtr;

	class UWidget* GetWidget();

	virtual int32 GetVarReferenceCount() override;
	virtual EBPElgBESVariableType GetVarType() override;
	virtual bool IsVarValid() override;
	virtual bool ValidateAndUpdate() override;

	void Setup(UBlueprint* InBlueprint, FProperty* InProperty);

	static UElgBESGraphWidgetVariable* MakeGraphWidgetVariable(UBlueprint* InBlueprint, FProperty* InProperty);

};
