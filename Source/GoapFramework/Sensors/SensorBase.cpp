#include "SensorBase.h"
#include "TimerManager.h"
#include "../PawnBase.h"
#include "Engine/World.h"

ASensorBase::ASensorBase()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASensorBase::BeginPlay()
{
    Super::BeginPlay();

    SensorOwner = Cast<APawnBase>(GetOwner());
    TrackingComponent = GetRootComponent();

    if (!bManualDetection)
    {
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASensorBase::TimerTick, TimerInterval, true);
    }
}

void ASensorBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Manual detection can call UpdateTargets() as needed
}

void ASensorBase::TimerTick()
{
    if (!bManualDetection)
    {
        UpdateTargets();
    }
}

bool ASensorBase::IsTargetInRange(APawnBase* Pawn) const
{
    if (!TrackingComponent || !Pawn)
    {
        return false;
    }
    return FVector::Dist(TrackingComponent->GetComponentLocation(), Pawn->GetActorLocation()) <= DetectionRadius;
}

FVector ASensorBase::GetClosestTargetPosition() const
{
    if (Targets.Num() == 0 || !TrackingComponent)
    {
        return FVector::ZeroVector;
    }

    float ClosestDist = TNumericLimits<float>::Max();
    FVector ClosestPos = FVector::ZeroVector;
    FVector Origin = TrackingComponent->GetComponentLocation();

    for (APawnBase* Target : Targets)
    {
        if (!Target) continue;
        float Dist = FVector::Dist(Origin, Target->GetActorLocation());
        if (Dist < ClosestDist)
        {
            ClosestDist = Dist;
            ClosestPos = Target->GetActorLocation();
        }
    }
    return ClosestPos;
}

void ASensorBase::SignalTargetsChanged()
{
    OnTargetsChanged.Broadcast();
}

void ASensorBase::AddTarget(APawnBase* Target)
{
    if (!IsValidTarget(Target))
    {
        return;
    }
    
    Targets.Add(Target);
    SignalTargetsChanged();
}

void ASensorBase::RemoveTarget(APawnBase* Target)
{
    if (!Target)
    {
        return;
    }
    Targets.Remove(Target);
    SignalTargetsChanged();
}

void ASensorBase::CleanNullTargets()
{
    Targets.RemoveAll([](APawnBase* Target) { return Target == nullptr; });
}

bool ASensorBase::IsValidTarget(APawnBase* Target) const
{
    if (!Target || Targets.Contains(Target) || !SensorOwner)
    {
        return false;
    }
    // Add your tag/party/ignore logic here as needed
    return true;
}

// Default implementation (must be overridden)
void ASensorBase::UpdateTargets_Implementation()
{
}