#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Belief.h"
#include "BeliefFactory.generated.h"

class AGoapAgent;
class ASensorBase;

UCLASS()
class GOAPFRAMEWORK_API UBeliefFactory : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(AGoapAgent* InAgent, TMap<EBeliefName, UBelief*>& InBeliefs);

	void AddBelief(EBeliefName Key, TFunction<bool()> Condition);
	void AddHasTargetsSensorBelief(EBeliefName Key, ASensorBase* Sensor);
	void AddLocationBelief(EBeliefName Key, float Distance, FVector LocationCondition);

private:
	AGoapAgent* Agent;
	TMap<EBeliefName, UBelief*>* Beliefs;

	bool InRangeOf(const FVector& Position, float Range) const;
};