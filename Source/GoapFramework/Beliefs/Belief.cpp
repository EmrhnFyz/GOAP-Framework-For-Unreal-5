#include "Belief.h"

UBelief* UBelief::Create(UObject* Outer, EBeliefName InName, TFunction<bool()> InCondition, TFunction<FVector()> InLocation)
{
	UBelief* NewBelief = NewObject<UBelief>(Outer);
	NewBelief->Name = InName;
	NewBelief->Condition = InCondition;
	NewBelief->ObservedLocation = InLocation;
	return NewBelief;
}