// Copyright 2019-2021 ElgSoft. All rights reserved. 


#include "BPWrappers/ElgBESGraphInterface.h"
#include <ElgKEWUtils.h>
#include <ElgBESGraphFunction.h>
#include <ElgBESGraphInterfaceFunction.h>
#include <ClassViewerModule.h>
#include <ClassViewerFilter.h>
#include <Kismet2/KismetEditorUtilities.h>
#include <AssetRegistry/AssetRegistryModule.h>
#include <ElgKEWBlueprintInterfaceFilter.h>
#include "ElgKEW_Log.h"


TArray<FS_ElgFunctionInfo> UElgBESGraphInterface::GetFunctionsInfo()
{
	TArray<FS_ElgFunctionInfo> infos;
	for (UFunction* func : GetFunctions()) {
		infos.Add(FElgKEWUtils::CreateFunctionInfo(func));
	}
	infos.Sort([](const FS_ElgFunctionInfo& varA, const FS_ElgFunctionInfo& varB) {
		return varA.FunctionName.ToString() < varB.FunctionName.ToString();
		});

	return infos;
}


TArray<UElgBESGraphInterfaceFunction*> UElgBESGraphInterface::GetInterfaceFunctions()
{
	TArray<UElgBESGraphInterfaceFunction*> functions;
	for (UFunction* func : GetFunctions()) {
		if (!bInherit) {
			functions.Add(UElgBESGraphInterfaceFunction::MakeGraphInterfaceFunction(BlueprintPtr, func, Description));
		} else {
			functions.Add(UElgBESGraphInterfaceFunction::MakeGraphInterfaceFunction(BlueprintPtr, func, InterfaceClassPtr));
		}
	}
	return functions;
}


TArray<UFunction*> UElgBESGraphInterface::GetFunctions()
{
	TArray<UFunction*> functions;
	if (!ValidateAndUpdate()) return functions;
	//if (bInherit) return functions;
	if (InterfaceClassPtr) {
		for (TFieldIterator<UFunction> FunctionIt(InterfaceClassPtr, EFieldIteratorFlags::IncludeSuper); FunctionIt; ++FunctionIt) {
			UFunction* Function = *FunctionIt;
			const FName FunctionName = Function->GetFName();
			if (FunctionName != UEdGraphSchema_K2::FN_ExecuteUbergraphBase) {
				functions.Add(Function);
			}
		}
	}
	return functions;
}


UEdGraph* UElgBESGraphInterface::GetFunctionsGraph(UFunction* InFunction)
{
	if (!ValidateAndUpdate()) return nullptr;
	if (bInherit) return nullptr;
	for (UEdGraph* const Graph : Description.Graphs) {
		if (Graph && Graph->GetFName() == InFunction->GetFName()) {
			return Graph;
		}
	}
	return nullptr;
}



bool UElgBESGraphInterface::ValidateAndUpdate()
{
	if (BlueprintPtr && InterfaceName.IsValid()) {
		if (!bInherit) {
			for (int32 i = 0; i < BlueprintPtr->ImplementedInterfaces.Num(); i++) {
				FBPInterfaceDescription& InterfaceDesc = BlueprintPtr->ImplementedInterfaces[i];
				if (FElgKEWUtils::GetInterfaceClassName(InterfaceDesc.Interface.Get()) == InterfaceName) {
					Setup(BlueprintPtr, InterfaceDesc);
					return true;
				}
			}
		} else {
			if (InterfaceClassPtr && FElgKEWUtils::GetInterfaceClassName(InterfaceClassPtr) == InterfaceName) {
				return true;
			}			
		}
		UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("Interface [%s] is no longer valid!"), *InterfaceName.ToString());
		return false;
	}
	UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("Interface is no longer valid!"));
	return false;
}


void UElgBESGraphInterface::Setup(UBlueprint* InBlueprint, FBPInterfaceDescription InDescription)
{
	BlueprintPtr = InBlueprint;
	Description = InDescription;	
	bInherit = false;
	InterfaceClassPtr = InDescription.Interface.Get();
	InterfaceClassName = InterfaceClassPtr->GetFName();
	InterfaceName = FElgKEWUtils::GetInterfaceClassName(InterfaceClassPtr);
}



void UElgBESGraphInterface::Setup(UBlueprint* InBlueprint, UClass* InInterfaceClass)
{
	BlueprintPtr = InBlueprint;
	//Description = InDescription; 
	bInherit = true;
	InterfaceClassPtr = InInterfaceClass;
	InterfaceClassName = InterfaceClassPtr->GetFName();
	InterfaceName = FElgKEWUtils::GetInterfaceClassName(InterfaceClassPtr);
}


UElgBESGraphInterface* UElgBESGraphInterface::MakeGraphInterface(UBlueprint* InBlueprint, FBPInterfaceDescription InDescription)
{
	UElgBESGraphInterface* interfaceObj = NewObject<UElgBESGraphInterface>();
	interfaceObj->Setup(InBlueprint, InDescription);
	return interfaceObj;	
}


UElgBESGraphInterface* UElgBESGraphInterface::MakeGraphInterface(UBlueprint* InBlueprint, UClass* InInterfaceClass)
{
	UElgBESGraphInterface* interfaceObj = NewObject<UElgBESGraphInterface>();
	interfaceObj->Setup(InBlueprint, InInterfaceClass);
	return interfaceObj;

}

TArray<UElgBESGraphInterface*> UElgBESGraphInterface::GetBlueprintInterfaces(UBlueprint* InBlueprint, const bool bIncludeInherent /*= false*/)
{
	TArray<UElgBESGraphInterface*> interfaces;
	check(InBlueprint);
	for (int32 i = 0; i < InBlueprint->ImplementedInterfaces.Num(); i++) {
		FBPInterfaceDescription& InterfaceDesc = InBlueprint->ImplementedInterfaces[i];
		interfaces.Add(UElgBESGraphInterface::MakeGraphInterface(InBlueprint, InterfaceDesc));
	}
	if (bIncludeInherent) {
		// Now get all the ones the blueprint's parents implemented
		UClass* BlueprintParent = InBlueprint->ParentClass;
		while (BlueprintParent) {
			for (FImplementedInterface ImplementedInterface : BlueprintParent->Interfaces) {
				interfaces.Add(UElgBESGraphInterface::MakeGraphInterface(InBlueprint, ImplementedInterface.Class));
				//UE_LOG(LogElgKismetEditorWidget, Log, TEXT("Parent Interface: %s"), *FElgKEWUtils::GetInterfaceClassName(ImplementedInterface.Class).ToString());
			}
			BlueprintParent = BlueprintParent->GetSuperClass();
		}
	}
	return interfaces;
}


UElgBESGraphInterface* UElgBESGraphInterface::GetBlueprintInterface(UBlueprint* InBlueprint, const FName InName)
{
	for (int32 i = 0; i < InBlueprint->ImplementedInterfaces.Num(); i++) {
		FBPInterfaceDescription& InterfaceDesc = InBlueprint->ImplementedInterfaces[i];
		if (FElgKEWUtils::GetInterfaceClassName(InterfaceDesc.Interface.Get()) == InName ) {
			return UElgBESGraphInterface::MakeGraphInterface(InBlueprint, InterfaceDesc);
		}
	}
	return nullptr;
}




TArray<FName> UElgBESGraphInterface::GetInterfaces(UBlueprint* InBlueprint)
{
	TArray<FName> outInterfaces;

	TSharedPtr<FElgKEWBlueprintInterfaceFilter> interfaceFilter = MakeShareable(new FElgKEWBlueprintInterfaceFilter(InBlueprint));
	
	for (TObjectIterator< UClass > ClassIt; ClassIt; ++ClassIt) {
		if (ClassIt->IsChildOf(UInterface::StaticClass())) {
			UClass* classT = *ClassIt;
			if (interfaceFilter->IsClassAllowed(classT)) {
				outInterfaces.Add(classT->GetFName());
			}
		}
	}

	// check blueprint assets (they might not be loaded and missed by the TObjectIterator)	
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	FARFilter Filter;	
	Filter.ClassNames.Add(UBlueprint::StaticClass()->GetFName());
	Filter.bRecursiveClasses = true;
	Filter.TagsAndValues.Add(FName(TEXT("BlueprintType")), FString(TEXT("BPTYPE_Interface")));

	TArray<FAssetData> AssetList;
	AssetRegistry.GetAssets(Filter, AssetList);
	for (auto const& Asset : AssetList) {	
		if (interfaceFilter->IsClassAllowed(Asset)) {
			outInterfaces.AddUnique(Asset.AssetName);
		}
	}

	outInterfaces.Sort([&](const FName& varA, const FName& varB) {
		//return varA.Name.to < varB.Name;
		return varA.ToString() < varB.ToString();
		});
	return outInterfaces;
}


FName UElgBESGraphInterface::GetNameInternal()
{
	if (!ValidateAndUpdate()) return NAME_None;
	return InterfaceName;
}
