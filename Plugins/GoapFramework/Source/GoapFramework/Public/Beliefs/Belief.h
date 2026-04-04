// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Belief.generated.h"

UENUM(BlueprintType)
enum class EBeliefName : uint8
{
	// Add your belief names here
	None UMETA(DisplayName = "None")
};

UCLASS(BlueprintType)
class GOAPFRAMEWORK_API UBelief : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EBeliefName Name;

	// Delegate for condition evaluation
	TFunction<bool()> Condition;

	// Optional: For location-based beliefs
	TFunction<FVector()> ObservedLocation;

	UFUNCTION(BlueprintCallable)
	bool Evaluate() const
	{
		return Condition ? Condition() : false;
	}

	UFUNCTION(BlueprintCallable)
	FVector GetLocation() const
	{
		return ObservedLocation ? ObservedLocation() : FVector::ZeroVector;
	}

	// Builder pattern for easy construction
	static UBelief* Create(UObject* Outer, EBeliefName InName, TFunction<bool()> InCondition, TFunction<FVector()> InLocation = nullptr);
};