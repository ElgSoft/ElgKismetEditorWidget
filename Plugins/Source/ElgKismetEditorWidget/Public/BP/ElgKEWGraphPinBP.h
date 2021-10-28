// Copyright 2019-2021 ElgSoft. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ElgKEWStructs.h"
#include "ElgKEWEnum.h"
#include "ElgKEWGraphPinBP.generated.h"

/**
 * 
 */
UCLASS()
class ELGKISMETEDITORWIDGET_API UElgKEWGraphPinBP : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Pin")
		static void DivideByPinDirection(const TArray<UElgBESGraphPin*> InPins, TArray<UElgBESGraphPin*>& OutInputPins, TArray<UElgBESGraphPin*>& OutOutputPins);

#pragma region PinFilters

	/* Filter to return any pins of the direction */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Pin")
		static TArray<UElgBESGraphPin*> FilterPinDirection(const TArray<UElgBESGraphPin*> InPins, EBPElgKEWPinDirection InDirection);

	/* Filter to return any pins of the right type/Category */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Pin")
		static TArray<UElgBESGraphPin*> FilterPinCategory(const TArray<UElgBESGraphPin*> InPins, const FName InCategory);
	
	/* Filter to return any pins of the right type */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Pin")
		static TArray<UElgBESGraphPin*> FilterPinType(const TArray<UElgBESGraphPin*> InPins, const EBPElgKEWPinCategory InType);

	/* Only return any visible pins */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Pin")
		static TArray<UElgBESGraphPin*> FilterPinVisible(const TArray<UElgBESGraphPin*> InPins);

	/* Filter out any Exec pins */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Pin")
		static TArray<UElgBESGraphPin*> FilterPinData(const TArray<UElgBESGraphPin*> InPins);

	/* Filter out any Exec and hidden pins */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Pin")
		static TArray<UElgBESGraphPin*> FilterPinVisibleData(const TArray<UElgBESGraphPin*> InPins);


#pragma endregion

};
