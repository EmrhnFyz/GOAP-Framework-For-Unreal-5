#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "SensorBase.generated.h"

UENUM(BlueprintType)
enum class ESensorName : uint8
{
    // Add your Sensor names here
    None UMETA(DisplayName = "None")
};

USTRUCT(BlueprintType)
struct FSensorData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESensorName Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ASensorBase* Sensor = nullptr;
};

UENUM(BlueprintType, meta = (Bitflags))
enum class ESensorDetectionType : uint8
{
    None         = 0 UMETA(DisplayName = "None"),
    Enemy        = 1 << 0 UMETA(DisplayName = "Enemy"),
    Friendly     = 1 << 1 UMETA(DisplayName = "Friendly"),
    Neutral      = 1 << 2 UMETA(DisplayName = "Neutral"),
    Destructible = 1 << 3 UMETA(DisplayName = "Destructible"),
    Gate         = 1 << 4 UMETA(DisplayName = "Gate"),
    All          = 0xFF   UMETA(DisplayName = "All")
};
ENUM_CLASS_FLAGS(ESensorDetectionType);

class APawnBase; // Forward declare your Pawn class

UCLASS(Abstract, Blueprintable)
class GOAPFRAMEWORK_API ASensorBase : public AActor
{
    GENERATED_BODY()

public:
    ASensorBase();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensor")
    float TimerInterval = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensor")
    float DetectionRadius = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensor")
    FVector BoxSize = FVector(1.f, 1.f, 1.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensor")
    TEnumAsByte<ECollisionChannel> DetectionChannel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensor")
    bool bManualDetection = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensor", meta = (Bitmask, BitmaskEnum = "ESensorDetectionType"))
    int32 DetectionType = (int32)ESensorDetectionType::All;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensor", meta = (Bitmask, BitmaskEnum = "ESensorDetectionType"))
    int32 IgnoreType = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Sensor")
    TArray<APawnBase*> Targets;

    UPROPERTY(BlueprintReadOnly, Category = "Sensor")
    APawnBase* SensorOwner = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "Sensor")
    USceneComponent* TrackingComponent = nullptr;

    UFUNCTION(BlueprintCallable, Category = "Sensor")
    virtual bool HasTargets() const { return Targets.Num() > 0; }

    UFUNCTION(BlueprintCallable, Category = "Sensor")
    const TArray<APawnBase*>& GetTargets() const { return Targets; }

    UFUNCTION(BlueprintCallable, Category = "Sensor")
    void SetTrackingComponent(USceneComponent* Component) { TrackingComponent = Component; }

    UFUNCTION(BlueprintCallable, Category = "Sensor")
    void SetDetectionRadius(float Radius) { DetectionRadius = Radius; }

    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTargetsChanged);

    UPROPERTY(BlueprintAssignable, Category = "Sensor")
    FOnTargetsChanged OnTargetsChanged;
    
    // Main update for targets (to be implemented in child)
    UFUNCTION(BlueprintNativeEvent, Category = "Sensor")
    void UpdateTargets();

    // Utility
    UFUNCTION(BlueprintCallable, Category = "Sensor")
    bool IsTargetInRange(APawnBase* Pawn) const;

    UFUNCTION(BlueprintCallable, Category = "Sensor")
    FVector GetClosestTargetPosition() const;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    void SignalTargetsChanged();

    void AddTarget(APawnBase* Target);
    void RemoveTarget(APawnBase* Target);
    void CleanNullTargets();

    // Timer handle for periodic updates
    FTimerHandle TimerHandle;

    void TimerTick();

    // Filtering
    bool IsValidTarget(APawnBase* Target) const;
};