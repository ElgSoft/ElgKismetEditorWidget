// Copyright 2019-2023 ElgSoft. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ElgKEWGraphNodeBP.generated.h"


class UElgBESGraphNode;
/**
 * 
 */
UCLASS()
class ELGKISMETEDITORWIDGET_API UElgKEWGraphNodeBP : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	/* Sort the node list by there name */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Node")
		static void SortNodeList(UPARAM(ref) TArray<UElgBESGraphNode*>& InNodes);


#pragma region NodeFilters

	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Node")
		static bool FilterNodeGuid(const TArray<UElgBESGraphNode*> InNodes, FGuid InGuid, UElgBESGraphNode*& OutNode);

	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Node")
		static TArray<UElgBESGraphNode*> FilterNodeTypeName(const TArray<UElgBESGraphNode*> InNodes, const FString InTypeName);

	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Node")
		static TArray<UElgBESGraphNode*> FilterNodeTypeNames(const TArray<UElgBESGraphNode*> InNodes, const TArray<FString> InTypeNames);

	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Node")
		static TArray<UElgBESGraphNode*> FilterNodeMemberName(const TArray<UElgBESGraphNode*> InNodes, const FString InMemberName);

	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Node")
		static TArray<UElgBESGraphNode*> FilterNodeMemberParent(const TArray<UElgBESGraphNode*> InNodes, const FString InMemberParent);

	/* filter the nodes based on the node name */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Node")
		static TArray<UElgBESGraphNode*> FilterNodeName(const TArray<UElgBESGraphNode*> InNodes, const FString InFilterName);

	/* filter get all variable nodes, getter and setters */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Node")
		static TArray<UElgBESGraphNode*> FilterNodeVariables(const TArray<UElgBESGraphNode*> InNodes);

#pragma endregion

};

