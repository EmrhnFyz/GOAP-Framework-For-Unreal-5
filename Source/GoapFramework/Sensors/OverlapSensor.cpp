#include "OverlapSensor.h"
#include "../PawnBase.h"
#include "Engine/World.h"
#include "Engine/OverlapResult.h"

AOverlapSensor::AOverlapSensor()
{
}

void AOverlapSensor::UpdateTargets_Implementation()
{
	if (!TrackingComponent) return;

	CleanNullTargets();

	TArray<APawnBase*> Found = PerformOverlap(TrackingComponent->GetComponentLocation(), DetectionRadius);

	// Add new targets
	for (APawnBase* Pawn : Found)
	{
		if (!Targets.Contains(Pawn))
		{
			AddTarget(Pawn);
		}
	}

	// Remove targets no longer in range
	for (int32 i = Targets.Num() - 1; i >= 0; --i)
	{
		if (Targets[i] && !Found.Contains(Targets[i]))
		{
			RemoveTarget(Targets[i]);
		}
	}
}

void AOverlapSensor::UpdateTargetsAtPosition(FVector Position)
{
	CleanNullTargets();

	TArray<APawnBase*> Found = PerformOverlap(Position, DetectionRadius);

	for (APawnBase* Pawn : Found)
	{
		if (!Targets.Contains(Pawn))
		{
			AddTarget(Pawn);
		}
	}

	for (int32 i = Targets.Num() - 1; i >= 0; --i)
	{
		if (Targets[i] && !Found.Contains(Targets[i]))
		{
			RemoveTarget(Targets[i]);
		}
	}
}

bool AOverlapSensor::HasTargetsInArea(FVector Position, float Radius) const
{
	TArray<APawnBase*> Found = PerformOverlap(Position, Radius);
	return Found.Num() > 0;
}

TArray<APawnBase*> AOverlapSensor::GetTargetsInArea(FVector Position, float Radius) const
{
	return PerformOverlap(Position, Radius);
}

void AOverlapSensor::RemoveOldTargetsOutOfRange()
{
	if (!TrackingComponent) return;

	for (int32 i = Targets.Num() - 1; i >= 0; --i)
	{
		if (Targets[i] && !IsTargetInRange(Targets[i]))
		{
			RemoveTarget(Targets[i]);
		}
	}
}

void AOverlapSensor::ClearTargets()
{
	Targets.Empty();
	SignalTargetsChanged();
}

TArray<APawnBase*> AOverlapSensor::PerformOverlap(FVector Origin, float Radius) const
{
	TArray<APawnBase*> Result;

	if (!GetWorld()) return Result;

	TArray<FOverlapResult> OverlapResults;
	FCollisionShape Shape = FCollisionShape::MakeSphere(Radius);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	if (SensorOwner)
	{
		QueryParams.AddIgnoredActor(SensorOwner);
	}

	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(DetectionChannel);

	GetWorld()->OverlapMultiByObjectType(OverlapResults, Origin, FQuat::Identity, ObjectParams, Shape, QueryParams);

	for (const FOverlapResult& Overlap : OverlapResults)
	{
		AActor* HitActor = Overlap.GetActor();
		if (!HitActor) continue;

		APawnBase* PawnTarget = Cast<APawnBase>(HitActor);
		if (PawnTarget && IsValidTarget(PawnTarget))
		{
			Result.AddUnique(PawnTarget);
		}
	}

	return Result;
}
