// Copyright 2019-2021 ElgSoft. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ElgKEWStructs.h"
#include "ElgKEWBlueprintEditorBP.generated.h"

class UElgEditorContext_BlueprintEditor;




/**
 *
 */
UCLASS()
class ELGKISMETEDITORWIDGET_API UElgKEWBlueprintEditorBP : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/* Get the KismetEditorWidget context for this graph */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget", meta = (DefaultToSelf = "InSelf", AdvancedDisplay = "InSelf", CompactNodeTitle="BPEditor"))
		static UElgEditorContext_BlueprintEditor* GetKismetContext(UObject* InSelf);

	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget", meta = (DefaultToSelf = "InSelf", AdvancedDisplay = "InSelf", CompactNodeTitle = "GraphType"))
		static EBPElgKEWGraphType GetGraphType(UEdGraph* InGraph);

	/* Check if the context is valid */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget", meta = (DefaultToSelf = "InSelf", AdvancedDisplay = "InSelf", ExpandEnumAsExecs = "Branch"))
		static void IsContextValid(UObject* InSelf, EBPElgKEWWResult& Branch);


	/* Make a PinType */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget")
		static void MakePinType(EBPElgKEWPinCategory InCategory, EBPElgPinContainerType InContainterType, FS_ElgGraphPinType& OutPinType);


	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget", meta = (DefaultToSelf = "InSelf"))
		static void TestFindBlueprintEditor(UObject* InSelf);

};
