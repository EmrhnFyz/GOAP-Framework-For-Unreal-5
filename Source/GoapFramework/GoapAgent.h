#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "./Goals/Goal.h"
#include "./Actions/AgentAction.h"
#include "./Beliefs/Belief.h"
#include "./Sensors/SensorBase.h"
#include "./Planner/GoapPlanner.h"
#include "GoapAgent.generated.h"


UCLASS(Blueprintable)
class GOAPFRAMEWORK_API AGoapAgent : public ACharacter
{
    GENERATED_BODY()

public:
    AGoapAgent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
    int32 Id = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
    APawnBase* Pawn = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "GOAP")
    bool bIsAlive = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
    TArray<FSensorData> Sensors;

    UPROPERTY(BlueprintReadOnly, Category = "GOAP")
    TMap<ESensorName, ASensorBase*> SensorMap;

    UPROPERTY(BlueprintReadOnly, Category = "GOAP")
    UGoal* LastGoal = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "GOAP")
    UGoal* CurrentGoal = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "GOAP")
    UActionPlan* ActionPlan = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "GOAP")
    UAgentAction* CurrentAction = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "GOAP")
    TMap<EBeliefName, UBelief*> Beliefs;

    UPROPERTY(BlueprintReadOnly, Category = "GOAP")
    TSet<UAgentAction*> Actions;

    UPROPERTY(BlueprintReadOnly, Category = "GOAP")
    TSet<UGoal*> Goals;

    UPROPERTY()
    UGoapPlanner* Planner = nullptr;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    virtual void SetupDictionaries();
    virtual void SetupTimers();
    virtual void SetupBeliefs();
    virtual void SetupActions();
    virtual void SetupGoals();

    void CalculatePlan();

    UFUNCTION(BlueprintCallable, Category = "GOAP")
    ASensorBase* GetSensor(ESensorName SensorName) const;

    // Pooling hooks for goals (implement as needed)
    static TSet<UGoal*> GetGoalSetFromPool();
    static void ReturnGoalSetToPool(TSet<UGoal*>& Set);
};