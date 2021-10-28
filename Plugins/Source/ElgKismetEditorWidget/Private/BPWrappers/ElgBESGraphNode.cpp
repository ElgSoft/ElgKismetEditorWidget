// Copyright 2019-2021 ElgSoft. All rights reserved. 


#include "ElgBESGraphNode.h"
#include "ElgKEWUtils.h"
#include <Kismet2/KismetEditorUtilities.h>
#include <EdGraph/EdGraphPin.h>
#include <BPWrappers/ElgBESGraphPin.h>
#include "ElgKEW_Log.h"


void UElgBESGraphNode::SelectNode()
{
	if (!ValidateNode()) return;
	FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(NodePtr, false);
}

void UElgBESGraphNode::GetNodeInputPins(TArray<UElgBESGraphPin*>& OutPins)
{
	if (!ValidateNode()) return;
	for (UEdGraphPin* pin : NodePtr->Pins) {
		if (pin->Direction == EEdGraphPinDirection::EGPD_Input) {
			if (!pin->bHidden && pin->PinType.PinCategory != UEdGraphSchema_K2::PC_Exec) {
				OutPins.Add(UElgBESGraphPin::MakeGraphPinObject(pin));
			}			
		}
	}
}

void UElgBESGraphNode::GetNodeOutputPins(TArray<UElgBESGraphPin*>& OutPins)
{
	if (!ValidateNode()) return;
	for (UEdGraphPin* pin : NodePtr->Pins) {
		if (pin->Direction == EEdGraphPinDirection::EGPD_Output) {
			if (!pin->bHidden && pin->PinType.PinCategory != UEdGraphSchema_K2::PC_Exec) {
				OutPins.Add(UElgBESGraphPin::MakeGraphPinObject(pin));
			}
		}
	}
}


#pragma region NodeGetters


void UElgBESGraphNode::GetTintColor(struct FLinearColor& OutTint)
{
	if (!ValidateNode()) return;
	NodePtr->GetIconAndTint(OutTint);
}

FGuid UElgBESGraphNode::GetNodeGuid()
{
	if (!ValidateNode()) return Guid;
	return NodePtr->NodeGuid;
}

FString UElgBESGraphNode::GetNodeGuidAsString()
{
	if (!ValidateNode()) return FString();
	return NodePtr->NodeGuid.ToString();
}

FString UElgBESGraphNode::GetNodeMemberName()
{
	if (!ValidateNode()) return FString();
	return MemberName;
}

FString UElgBESGraphNode::GetNodeMemberParent()
{
	if (!ValidateNode()) return FString();
	return ParentName;
}

FString UElgBESGraphNode::GetNodeMemberParentPath()
{
	if (!ValidateNode()) return FString();
	return ParentPath;
}

FString UElgBESGraphNode::GetNodeMemberPath()
{
	if (!ValidateNode()) return FString();
	if (MemberName.Equals("")) return FString();	
	return FString();//ParentPath + "::" + MemberName;
}

FString UElgBESGraphNode::GetNodeType()
{
	if (!ValidateNode()) return FString();
	return NodePtr->GetClass()->GetName();
}

void UElgBESGraphNode::GetNodePins(TArray<UElgBESGraphPin*>& OutPins)
{
	if (!ValidateNode()) return;
	for (UEdGraphPin* pin : NodePtr->Pins) {
		OutPins.Add(UElgBESGraphPin::MakeGraphPinObject(pin));
	}
}

FVector2D UElgBESGraphNode::GetNodePosition()
{
	if (!ValidateNode()) return FVector2D();
	return FVector2D(NodePtr->NodePosX, NodePtr->NodePosY);
}

FString UElgBESGraphNode::GetNodeTipText()
{
	if (!ValidateNode()) return FString();
	return NodePtr->GetTooltipText().ToString();
}

UEdGraph* UElgBESGraphNode::GetGraph()
{
	if (!ValidateNode()) nullptr;
	return NodePtr->GetGraph();
}

FString UElgBESGraphNode::GetGraphName()
{
	if (UEdGraph* graph = GetGraph()) {
		return graph->GetName();
	}
	return FString();
}


bool UElgBESGraphNode::GetIsValid()
{
	if (NodePtr && Guid.IsValid() && NodePtr->NodeGuid == Guid) {
		return true;
	}
	return false;
}

void UElgBESGraphNode::IsValidBranch(EBPElgKEWWResult& Branch)
{
	Branch = GetIsValid() ? EBPElgKEWWResult::outTrue : EBPElgKEWWResult::outFalse;
}


bool UElgBESGraphNode::IsConnected()
{
	return FElgKEWUtils::IsNodeConnected(NodePtr);
}

void UElgBESGraphNode::IsConnectedBranch(EBPElgKEWWResult& Branch)
{	
	if (FElgKEWUtils::IsNodeConnected(NodePtr)) {
		Branch = EBPElgKEWWResult::outTrue;
	}  else {
		Branch = EBPElgKEWWResult::outFalse;
	}
}


#pragma endregion

FName UElgBESGraphNode::GetNameInternal()
{
	if (!ValidateNode()) return NAME_None;
	NodeName = NodePtr->GetNodeTitle(ENodeTitleType::ListView).ToString();
	return FName(NodeName);
}


void UElgBESGraphNode::GetIconInternal(struct FSlateBrush& OutBrush)
{
	if (!ValidateNode()) return;
	FLinearColor tint = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	FSlateIcon icon = NodePtr->GetIconAndTint(tint);
	OutBrush = *icon.GetOptionalIcon();
	OutBrush.TintColor = tint;
}


void UElgBESGraphNode::Setup(UEdGraphNode* InNode)
{
	NodePtr = InNode;
	Guid = NodePtr->NodeGuid;
	NodeName = NodePtr->GetNodeTitle(ENodeTitleType::ListView).ToString();
	FElgKEWUtils::GetNodeMemberName(InNode, MemberName, ParentName, ParentPath);
}

bool UElgBESGraphNode::ValidateNode()
{
	if (NodePtr && NodePtr->NodeGuid == Guid) {
		return true;
	}
	UE_LOG(LogElgKismetEditorWidget, Warning, TEXT("Node are no longer valid!"));
	return false;
}




UElgBESGraphNode* UElgBESGraphNode::MakeGraphNodeObject(UEdGraphNode* InNode)
{
	UElgBESGraphNode* bpNode = NewObject<UElgBESGraphNode>();
	bpNode->Setup(InNode);
	return bpNode;
}
