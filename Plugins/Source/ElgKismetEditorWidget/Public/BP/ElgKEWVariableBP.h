// Copyright 2019-2023 ElgSoft. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ElgKEWEnum.h"
#include "ElgKEWVariableBP.generated.h"

class UElgBESGraphVariable;
/**
 * 
 */
UCLASS()
class ELGKISMETEDITORWIDGET_API UElgKEWVariableBP : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/* Sort the variables on the name */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable")
		static void SortVariableByName(TArray<UElgBESGraphVariable*> InVariables);

	/* Find a variable by its name in the array of variables */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable")
		static bool FindVariableName(TArray<UElgBESGraphVariable*> InVariables, const FName InName, UElgBESGraphVariable*& OutVariable);

	/* Find a variable by its Guid in the array of variables */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable")
		static bool FindVariableGuid(TArray<UElgBESGraphVariable*> InVariables, const FGuid InGuid, UElgBESGraphVariable*& OutVariable);

	/* Return only the variables that are exposed on spawn */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable|Filter")
		static TArray<UElgBESGraphVariable*> FilterVariableExposeOnSpawn(TArray<UElgBESGraphVariable*> InVariables);

	/* Return only the variables that are saved */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable|Filter")
		static TArray<UElgBESGraphVariable*> FilterVariableSaveGame(TArray<UElgBESGraphVariable*> InVariables);

	/* Return only the variables inside the category */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable|Filter")
		static TArray<UElgBESGraphVariable*> FilterVariableCategory(TArray<UElgBESGraphVariable*> InVariables, const FString InCategory);

	/* Return only the variables off the right type */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Variable|Filter")
		static TArray<UElgBESGraphVariable*> FilterVariableType(TArray<UElgBESGraphVariable*> InVariables, EBPElgKEWPinCategory InType);



	/* TipText representing the variable's "edit-on-instance" state. */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "GetExposedTipText", CompactNodeTitle = "ExposedTipText"))
		static FText GetVariableExposedTipText(const bool bExposedState);

	/* Icon representing the variable's "edit-on-instance" state. */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "GetStateIcon", CompactNodeTitle = "ExposedStateIcon"))
		static void GetVariableExposedIcon(const bool bExposedState, FSlateBrush& OutBrush);

	/* Color representing the variable's "edit-on-instance" state. Pastel yellow if Exposed otherwise it's Gray.  */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Variable", meta = (DisplayName = "GetStateColor", CompactNodeTitle = "ExposedStateColor"))
		static void GetVariableExposedColor(const bool bExposedState, FLinearColor& OutColor);


};
