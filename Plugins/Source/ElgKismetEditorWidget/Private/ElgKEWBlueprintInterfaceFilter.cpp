// Copyright 2019-2021 ElgSoft. All rights reserved. 


#include "ElgKEWBlueprintInterfaceFilter.h"
#include <ElgKEWUtils.h>
#include <Kismet2/KismetEditorUtilities.h>


FElgKEWBlueprintInterfaceFilter::FElgKEWBlueprintInterfaceFilter(UBlueprint* InBlueprint)
{
	BlueprintPtr = InBlueprint;
	Setup();
}

FElgKEWBlueprintInterfaceFilter::~FElgKEWBlueprintInterfaceFilter()
{
	BlueprintPtr = nullptr;
	ClassToFilterOut.Empty();
	ClassNames.Empty();
}


bool FElgKEWBlueprintInterfaceFilter::IsClassAllowed(const UClass* InClass)
{
	if (!InClass->HasAnyClassFlags(CLASS_Interface)) return false;
	if (InClass->HasAnyClassFlags(CLASS_Deprecated | CLASS_NewerVersionExists)) return false;
	if (FKismetEditorUtilities::IsClassABlueprintSkeleton(InClass)) return false;
	if (InClass->HasMetaData(FBlueprintMetadata::MD_CannotImplementInterfaceInBlueprint)) return false;
	if (ClassToFilterOut.Contains(InClass)) return false;

	for (auto curClassIt = ClassToFilterOut.CreateConstIterator(); curClassIt; ++curClassIt) {
		if (InClass->IsChildOf(*curClassIt)) return false;
	}

	return true;
}


bool FElgKEWBlueprintInterfaceFilter::IsClassAllowed(const FName InName)
{
	if (ClassNames.Contains(InName)) return false;
	return true;
}


bool FElgKEWBlueprintInterfaceFilter::IsClassAllowed(const FAssetData& InAssetData)
{
	FAssetDataTagMapSharedView::FFindTagResult ResultType = InAssetData.TagsAndValues.FindTag(TEXT("BlueprintType"));
	if (ResultType.IsSet()) {
		const FString& bpType = ResultType.GetValue();
		if (!bpType.Equals(TEXT("BPTYPE_Interface"))) return false ;
		if (ClassNames.Contains(InAssetData.AssetName)) return false;
		return true;
	}

	return false;
}


void FElgKEWBlueprintInterfaceFilter::Setup()
{
	for (TArray<FBPInterfaceDescription>::TConstIterator it(BlueprintPtr->ImplementedInterfaces); it; ++it) {
		const FBPInterfaceDescription& CurrentInterface = *it;
		ClassToFilterOut.Add(CurrentInterface.Interface);
	}

	if (BlueprintPtr->GeneratedClass) {
		ClassToFilterOut.Add(BlueprintPtr->GeneratedClass);
	}

	// see if any parent class has any prohibited interfaces
	UClass* parentClass = BlueprintPtr->ParentClass;
	while (parentClass) {
		if (parentClass->HasMetaData(FBlueprintMetadata::MD_ProhibitedInterfaces)) {
			FString const& prohibitedList = parentClass->GetMetaData(FBlueprintMetadata::MD_ProhibitedInterfaces);

			TArray<FString> prohibitedInterfaceNames;
			prohibitedList.ParseIntoArray(prohibitedInterfaceNames, TEXT(","), true);

			// loop over all the prohibited interfaces
			for (int32 exclusionIndex = 0; exclusionIndex < prohibitedInterfaceNames.Num(); ++exclusionIndex) {
				prohibitedInterfaceNames[exclusionIndex].TrimStartInline();
				FString const& prohibitedInterfaceName = prohibitedInterfaceNames[exclusionIndex].RightChop(1);
				UClass* prohibitedInterface = (UClass*)StaticFindObject(UClass::StaticClass(), ANY_PACKAGE, *prohibitedInterfaceName);
				if (prohibitedInterface) {
					ClassToFilterOut.Add(prohibitedInterface);
				}
			}
		}
		else if (parentClass->Interfaces.Num()) {
			for (FImplementedInterface ImplementedInterface : parentClass->Interfaces) {
				ClassToFilterOut.Add(ImplementedInterface.Class);
			}
		}
		parentClass = parentClass->GetSuperClass();
	}

	for (const UClass* disClass : ClassToFilterOut) {
		ClassNames.Add(FElgKEWUtils::GetInterfaceClassName(disClass));
	}
}
