// Copyright 2019-2023 ElgSoft. All rights reserved. 

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ELGKISMETEDITORWIDGET_API FElgKEWBlueprintInterfaceFilter
{
public:
	FElgKEWBlueprintInterfaceFilter(UBlueprint* InBlueprint);
	~FElgKEWBlueprintInterfaceFilter();

	/** Classes to filter out. */
	TSet< const UClass* > ClassToFilterOut;
	
	/** Name of the Classes to filter out. */
	TSet<FName> ClassNames;
		
	UBlueprint* BlueprintPtr;

	bool IsClassAllowed(const UClass* InClass);
	bool IsClassAllowed(const FName InName);
	bool IsClassAllowed(const FAssetData& InAssetData);

	void Setup();

};
