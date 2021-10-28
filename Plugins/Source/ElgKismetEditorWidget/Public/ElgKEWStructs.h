// Copyright 2019-2021 ElgSoft. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "ElgKEWEnum.h"
#include "ElgKEWStructs.generated.h"




/* Function Output Pin Variable Struct */
USTRUCT(BlueprintType, meta = (DisplayName = "GraphFunctionInfo"))
struct FS_ElgFunctionInfo {

	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="FunctionInfo")
		FName FunctionName;

	UPROPERTY(BlueprintReadOnly, Category = "FunctionInfo")
		FName ClassName;

	UPROPERTY(BlueprintReadOnly, Category = "FunctionInfo")
		FString ToolTip;

	UPROPERTY(BlueprintReadOnly, Category = "FunctionInfo")
		FString Description;
	
	UPROPERTY(BlueprintReadOnly, Category = "FunctionInfo")
		FString Category;

};


/*
* Blueprint wrapper for FEdGraphPinType.
* Struct used to define the type of information carried on this pin
*/
USTRUCT(BlueprintType, meta = (DisplayName = "EdGraphPinType"))
struct FS_ElgGraphPinType {

	GENERATED_BODY()

		/** Category of pin type */
	UPROPERTY(BlueprintReadOnly, Category = "FunctionInfo")
		EBPElgKEWPinCategory EPinCategory;

	/** Sub-category of pin type */
	UPROPERTY(BlueprintReadOnly, Category = "FunctionInfo")
		EBPElgKEWPinCategory EPinSubCategory;

	/** Enum used to define what container type a pin represents. */
	UPROPERTY(BlueprintReadOnly, Category = "FunctionInfo")
		EBPElgPinContainerType EPinContainerType;

	/* the real pin type value */
	FEdGraphPinType EdGraphPinType;

};


