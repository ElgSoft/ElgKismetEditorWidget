// Copyright 2019-2023 ElgSoft. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "ElgBESGraphEditablePinBase.h"
#include "ElgBESGraphFunctionPin.generated.h"


/**
 * Function Input/output Pin Blueprint Wrapper
 */
UCLASS(Blueprintable)
class ELGKISMETEDITORWIDGET_API UElgBESGraphFunctionPin : public UElgBESGraphEditablePinBase
{
	GENERATED_BODY()

public:	

	
	/** Promote the pin to a variable, 
		@param	bInToMemberVariable		TRUE if attempting to create a member variable, FALSE if the variable should be local
	*/
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|FunctionPin")
		void PromoteToVariable(const bool bInToMemberVariable);


	virtual int32 GetCountInternal() override;

	static UElgBESGraphFunctionPin* MakeGraphPinVariable(UBlueprint* InBlueprint, UEdGraph* InGraph, class UK2Node_EditablePinBase* InEntry, UEdGraphPin* InPin);

};
