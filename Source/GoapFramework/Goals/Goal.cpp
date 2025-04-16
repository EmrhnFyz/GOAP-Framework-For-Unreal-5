#include "Goal.h"

UGoal* UGoal::CreateGoal(UObject* Outer, EGoalName InName)
{
	UGoal* NewGoal = NewObject<UGoal>(Outer);
	NewGoal->Name = InName;
	return NewGoal;
}

// Builder Implementation
UGoal::Builder::Builder(UObject* Outer, EGoalName InName)
{
	Goal = UGoal::CreateGoal(Outer, InName);
}

UGoal::Builder& UGoal::Builder::WithPriority(float InPriority)
{
	Goal->Priority = InPriority;
	return *this;
}

UGoal::Builder& UGoal::Builder::WithDesiredEffect(UBelief* Belief)
{
	if (Belief)
	{
		Goal->DesiredEffects.Add(Belief);
	}
	return *this;
}

UGoal* UGoal::Builder::Build()
{
	return Goal;
}