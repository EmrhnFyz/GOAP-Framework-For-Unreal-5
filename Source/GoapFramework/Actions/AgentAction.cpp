#include "AgentAction.h"

UAgentAction* UAgentAction::Create(UObject* Outer, EActionName InName)
{
	UAgentAction* NewAction = NewObject<UAgentAction>(Outer);
	NewAction->Name = InName;
	return NewAction;
}

UAgentAction::Builder::Builder(UObject* Outer, EActionName InName)
{
	Action = Create(Outer, InName);
}

UAgentAction::Builder& UAgentAction::Builder::WithCost(float InCost)
{
	Action->Cost = InCost;
	return *this;
}

UAgentAction::Builder& UAgentAction::Builder::WithStrategy(TScriptInterface<IActionStrategy> InStrategy)
{
	Action->Strategy = InStrategy;
	return *this;
}

UAgentAction::Builder& UAgentAction::Builder::AddPrecondition(UBelief* Precondition)
{
	if (Precondition)
	{
		Action->Preconditions.Add(Precondition);
	}
	return *this;
}

UAgentAction::Builder& UAgentAction::Builder::AddEffect(UBelief* Effect)
{
	if (Effect)
	{
		Action->Effects.Add(Effect);
	}
	return *this;
}

UAgentAction* UAgentAction::Builder::Build() const
{
	return Action;
}
