#include "PawnBase.h"

APawnBase::APawnBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APawnBase::OnTakeAIDamage_Implementation(int32 Amount)
{
	RuntimeStatus.CurrentHealth = FMath::Max(0, RuntimeStatus.CurrentHealth - Amount);
	if (RuntimeStatus.CurrentHealth == 0)
	{
		OnDeath();
	}
}

void APawnBase::OnDeath_Implementation()
{
	// Default: do nothing. Extend in Blueprint or C++.
}