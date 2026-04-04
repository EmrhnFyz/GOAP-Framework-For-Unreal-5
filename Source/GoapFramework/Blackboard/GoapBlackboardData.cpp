#include "GoapBlackboardData.h"

void UGoapBlackboardData::SetValuesOnBlackboard(UGoapBlackboard* Blackboard) const
{
	if (!Blackboard) return;

	for (const FBlackboardEntryData& Entry : DefaultEntries)
	{
		Blackboard->SetValue(Entry.Key, Entry.Value);
	}
}

void UGoapBlackboardData::SetDefaultValuesOnBlackboard(UGoapBlackboard* Blackboard) const
{
	if (!Blackboard) return;

	for (const FBlackboardEntryData& Entry : DefaultEntries)
	{
		if (!Blackboard->ContainsKey(Entry.Key))
		{
			Blackboard->SetValue(Entry.Key, Entry.Value);
		}
	}
}
