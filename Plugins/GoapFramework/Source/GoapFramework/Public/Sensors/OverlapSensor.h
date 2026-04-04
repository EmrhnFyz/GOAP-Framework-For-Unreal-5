#pragma once

#include "CoreMinimal.h"
#include "SensorBase.h"
#include "OverlapSensor.generated.h"

UCLASS(Blueprintable)
class GOAPFRAMEWORK_API AOverlapSensor : public ASensorBase
{
	GENERATED_BODY()

public:
	AOverlapSensor();

	virtual void UpdateTargets_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Sensor")
	void UpdateTargetsAtPosition(FVector Position);

	UFUNCTION(BlueprintCallable, Category = "Sensor")
	bool HasTargetsInArea(FVector Position, float Radius) const;

	UFUNCTION(BlueprintCallable, Category = "Sensor")
	TArray<APawnBase*> GetTargetsInArea(FVector Position, float Radius) const;

	UFUNCTION(BlueprintCallable, Category = "Sensor")
	void RemoveOldTargetsOutOfRange();

	UFUNCTION(BlueprintCallable, Category = "Sensor")
	void ClearTargets();

private:
	TArray<APawnBase*> PerformOverlap(FVector Origin, float Radius) const;
};
