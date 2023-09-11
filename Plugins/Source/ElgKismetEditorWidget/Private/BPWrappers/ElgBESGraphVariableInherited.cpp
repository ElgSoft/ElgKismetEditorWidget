// Copyright 2019-2023 ElgSoft. All rights reserved. 


#include "BPWrappers/ElgBESGraphVariableInherited.h"
#include "ElgKEW_Log.h"

void UElgBESGraphVariableInherited::Setup(UBlueprint* InBlueprint, FProperty* InVariableProperty)
{
	BlueprintPtr = InBlueprint;
	UpdateVariableProperty(InVariableProperty);
}


EBPElgBESVariableType UElgBESGraphVariableInherited::GetVarType()
{
	return EBPElgBESVariableType::Inherited;
}

bool UElgBESGraphVariableInherited::ValidateAndUpdate()
{
	if (!VariableName.IsNone() && BlueprintPtr) {
		PropertyPtr = GetProperty();
		if (PropertyPtr) {
			return true;
		}		
		UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("Inherited variable [%s] is no longer valid!"), *VariableName.ToString());
	}
	UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("Inherited variable [Unknowm] is no longer valid!"));
	return false;
}

UElgBESGraphVariableInherited* UElgBESGraphVariableInherited::MakeGraphVariable(UBlueprint* InBlueprint, FProperty* InVariableProperty)
{
	UElgBESGraphVariableInherited* bpVariable = NewObject<UElgBESGraphVariableInherited>();
	bpVariable->Setup(InBlueprint, InVariableProperty);
	return bpVariable;

}
