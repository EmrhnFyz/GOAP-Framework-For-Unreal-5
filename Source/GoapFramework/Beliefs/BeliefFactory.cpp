#include "BeliefFactory.h"
#include "../GoapAgent.h"
#include "../Sensors/SensorBase.h"

void UBeliefFactory::Initialize(AGoapAgent* InAgent, TMap<EBeliefName, UBelief*>& InBeliefs)
{
	Agent = InAgent;
	Beliefs = &InBeliefs;
}

void UBeliefFactory::AddBelief(EBeliefName Key, TFunction<bool()> Condition)
{
	if (Beliefs)
	{
		Beliefs->Add(Key, UBelief::Create(this, Key, Condition));
	}
}

void UBeliefFactory::AddHasTargetsSensorBelief(EBeliefName Key, ASensorBase* Sensor)
{
	if (Beliefs && Sensor)
	{
		Beliefs->Add(Key, UBelief::Create(this, Key, [Sensor]()
		{
			return Sensor->HasTargets();
		}));
	}
}

void UBeliefFactory::AddLocationBelief(EBeliefName Key, float Distance, FVector LocationCondition)
{
	if (Beliefs)
	{
		(*Beliefs).Add(Key, UBelief::Create(this, Key,
			[this, LocationCondition, Distance]()
			{
				return InRangeOf(LocationCondition, Distance);
			},
			[LocationCondition]()
			{
				return LocationCondition;
			}
		));
	}
}

bool UBeliefFactory::InRangeOf(const FVector& Position, float Range) const
{
	if (!Agent)
	{
		return false;
	}
	return FVector::Dist(Agent->GetActorLocation(), Position) < Range;
}