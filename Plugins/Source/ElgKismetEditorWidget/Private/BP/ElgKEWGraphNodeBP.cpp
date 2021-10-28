// Copyright 2019-2021 ElgSoft. All rights reserved. 


#include "ElgKEWGraphNodeBP.h"
#include "ElgKEWStructs.h"
#include <ElgKEWEnum.h>
#include <Kismet2/KismetEditorUtilities.h>
#include <Math/Color.h>
#include <Textures/SlateIcon.h>
#include <K2Node_Variable.h>
#include <ElgBESGraphNode.h>



void UElgKEWGraphNodeBP::SortNodeList(TArray<UElgBESGraphNode*>& InNodes)
{
	InNodes.Sort([&](const UElgBESGraphNode& nodeA, const UElgBESGraphNode& nodeB) {
		return nodeA.NodeName < nodeB.NodeName;
		});
}



#pragma region NodeFilters

bool UElgKEWGraphNodeBP::FilterNodeGuid(const TArray<UElgBESGraphNode*> InNodes, FGuid InGuid, UElgBESGraphNode*& OutNode)
{
	for (UElgBESGraphNode* node : InNodes) {
		if (node->Guid == InGuid) {
			OutNode = node;
			return true;
		}
	}
	OutNode = nullptr;
	return false;
}


TArray<UElgBESGraphNode*> UElgKEWGraphNodeBP::FilterNodeTypeName(const TArray<UElgBESGraphNode*> InNodes, const FString InTypeName)
{
	TArray<UElgBESGraphNode*> outNodes;
	for (UElgBESGraphNode* node : InNodes) {
		if (node->GetNodeType().Equals(InTypeName)) {
			outNodes.Add(node);
		}
	}
	return outNodes;
}


TArray<UElgBESGraphNode*> UElgKEWGraphNodeBP::FilterNodeTypeNames(const TArray<UElgBESGraphNode*> InNodes, const TArray<FString> InTypeNames)
{
	TArray<UElgBESGraphNode*> outNodes;
	for (UElgBESGraphNode* node : InNodes) {
		FString nodeType = node->GetNodeType();
		if (InTypeNames.Contains(nodeType)) {
			outNodes.Add(node);
		}
	}
	return outNodes;
}



TArray<UElgBESGraphNode*> UElgKEWGraphNodeBP::FilterNodeMemberName(const TArray<UElgBESGraphNode*> InNodes, const FString InMemberName)
{
	TArray<UElgBESGraphNode*> outNodes;
	for (UElgBESGraphNode* node : InNodes) {
		if (node->GetNodeMemberName().Equals(InMemberName)) {
			outNodes.Add(node);
		}
	}
	return outNodes;
}


TArray<UElgBESGraphNode*> UElgKEWGraphNodeBP::FilterNodeMemberParent(const TArray<UElgBESGraphNode*> InNodes, const FString InMemberParent)
{
	TArray<UElgBESGraphNode*> outNodes;
	for (UElgBESGraphNode* node : InNodes) {
		if (node->GetNodeMemberParent().Equals(InMemberParent)) {
			outNodes.Add(node);
		}
	}
	return outNodes;
}


TArray<UElgBESGraphNode*> UElgKEWGraphNodeBP::FilterNodeName(const TArray<UElgBESGraphNode*> InNodes, const FString InFilterName)
{
	TArray<UElgBESGraphNode*> outNodes;
	for (UElgBESGraphNode* node : InNodes) {
		if (node->GetNameAsString().Contains(InFilterName)) {
			outNodes.Add(node);
		}		
	}
	return outNodes;
}

TArray<UElgBESGraphNode*> UElgKEWGraphNodeBP::FilterNodeVariables(const TArray<UElgBESGraphNode*> InNodes)
{
	TArray<UElgBESGraphNode*> outNodes;
	for (UElgBESGraphNode* node : InNodes) {
		if (node->NodePtr->IsA<UK2Node_Variable>()) {
			outNodes.Add(node);
		}
	}
	return outNodes;
}

#pragma endregion


