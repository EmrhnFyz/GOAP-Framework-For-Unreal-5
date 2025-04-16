// Node.cpp
#include "Node.h"

void UNode::Reset()
{
	Parent = nullptr;
	Action = nullptr;
	RequiredEffects.Empty();
	Leaves.Empty();
	Cost = 0.f;
}

void UNode::Initialize(UNode* InParent, UAgentAction* InAction, const TSet<UBelief*>& Effects, float InCost, bool bReuseEffects)
{
	Parent = InParent;
	Action = InAction;
	RequiredEffects = bReuseEffects ? Effects : TSet<UBelief*>(Effects);
	Leaves.Empty();
	Cost = InCost;
}