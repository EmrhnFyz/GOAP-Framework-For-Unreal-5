#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "../Beliefs/Belief.h"
#include "../Actions/IActionStrategy.h"
#include "AgentAction.generated.h"

UENUM(BlueprintType)
enum class EActionName : uint8
{
    // Add your action names here
    None UMETA(DisplayName = "None")
};

UCLASS(Blueprintable)
class GOAPFRAMEWORK_API UAgentAction : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EActionName Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Cost = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSet<UBelief*> Preconditions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSet<UBelief*> Effects;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TScriptInterface<IActionStrategy> Strategy;

    UFUNCTION(BlueprintCallable, Category = "GOAP")
    bool IsComplete() const
    {
        return Strategy ? Strategy->IsComplete() : false;
    }

    UFUNCTION(BlueprintCallable, Category = "GOAP")
    bool CanPerform() const
    {
        return Strategy ? Strategy->CanPerform() : false;
    }

    UFUNCTION(BlueprintCallable, Category = "GOAP")
    void Start()
    {
        if (Strategy) Strategy->Start();
    }

    UFUNCTION(BlueprintCallable, Category = "GOAP")
    void Update(float DeltaTime)
    {
        if (Strategy && CanPerform())
        {
            Strategy->Update(DeltaTime);
        }
        if (!IsComplete())
        {
            return;
        }
        for (UBelief* Effect : Effects)
        {
            if (Effect) Effect->Evaluate();
        }
    }

    UFUNCTION(BlueprintCallable, Category = "GOAP")
    void Stop()
    {
        if (Strategy) Strategy->Stop();
    }

    // Builder pattern for C++
    class Builder
    {
    public:
        Builder(UObject* Outer, EActionName InName);

        Builder& WithCost(float InCost);
        Builder& WithStrategy(TScriptInterface<IActionStrategy> InStrategy);
        Builder& AddPrecondition(UBelief* Precondition);
        Builder& AddEffect(UBelief* Effect);

        UAgentAction* Build();

    private:
        UAgentAction* Action;
    };

    static UAgentAction* Create(UObject* Outer, EActionName InName);
};