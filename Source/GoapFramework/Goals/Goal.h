#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "../Beliefs/Belief.h"
#include "Goal.generated.h"

UENUM(BlueprintType)
enum class EGoalName : uint8
{
	// Add your goal names here
	Example UMETA(DisplayName = "Example")
};

UINTERFACE(BlueprintType)
class GOAPFRAMEWORK_API UPrioritizable : public UInterface
{
	GENERATED_BODY()
};

class GOAPFRAMEWORK_API IPrioritizable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    float GetPriority() const;
};

UCLASS(Blueprintable)
class GOAPFRAMEWORK_API UGoal : public UObject, public IPrioritizable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGoalName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Priority = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSet<UBelief*> DesiredEffects;

	UFUNCTION(BlueprintCallable, Category = "GOAP")
	static UGoal* CreateGoal(UObject* Outer, EGoalName InName);

	// IPrioritizable
	virtual float GetPriority_Implementation() const override { return Priority; }

	// Builder pattern
	class Builder
	{
	public:
		Builder(UObject* Outer, EGoalName InName);

		Builder& WithPriority(float InPriority);
		Builder& WithDesiredEffect(UBelief* Belief);

		UGoal* Build();

	private:
		UGoal* Goal;
	};
};