#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "../Actions/AgentAction.h"
#include "../Goals/Goal.h"
#include "../Utils/Node.h"
#include "../Utils/ObjectPool.h"
#include "GoapPlanner.generated.h"

class AGoapAgent;
class UGoapAgent;

UCLASS(Blueprintable)
class GOAPFRAMEWORK_API UActionPlan : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	UGoal* AgentGoal = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TArray<UAgentAction*> Actions;

	UPROPERTY(BlueprintReadOnly)
	float TotalCost = 0.f;

	void Initialize(UGoal* InGoal, const TArray<UAgentAction*>& InActions, float InTotalCost)
	{
		AgentGoal = InGoal;
		Actions = InActions;
		TotalCost = InTotalCost;
	}

	void Reset()
	{
		Actions.Empty();
		AgentGoal = nullptr;
		TotalCost = 0.f;
	}
};

UCLASS(Blueprintable)
class GOAPFRAMEWORK_API UGoapPlanner : public UObject
{
	GENERATED_BODY()

public:
	// Main entry point for planning
	UFUNCTION(BlueprintCallable, Category = "GOAP")
	UActionPlan* Plan(AGoapAgent* Agent, const TSet<UGoal*>& Goals, UGoal* MostRecentGoal = nullptr);
	void ReturnActionPlanToPool(UActionPlan* Plan);

protected:
	void SetGoalsWithDescendingOrder(const TSet<UGoal*>& Goals, UGoal* MostRecentGoal, TArray<UGoal*>& OutOrderedGoals);
	float CalculateHeuristic(UNode* Node) const;
	float CalculateFScore(UNode* Node) const;
	void ReconstructPath(UNode* Node);
	void RemoveSatisfiedEffects(UNode* Node);
	bool FindPath(UNode* StartNode, const TSet<UAgentAction*>& AvailableActions, TArray<UNode*>& OutPath);
	UNode* CreateGoalNode(UGoal* Goal);
	void CleanupNodes(TArray<UNode*>& Nodes);
	void CleanupGoalResources(UNode* GoalNode);
	UActionPlan* CreateActionPlan(UNode* GoalNode, UGoal* Goal);

	// Pooling hooks (implement pooling as needed)
	UNode* GetNodeFromPool();
	void ReturnNodeToPool(UNode* Node);
	UActionPlan* GetActionPlanFromPool();

private:
	TObjectPool<UNode> NodePool;
	TObjectPool<UActionPlan> ActionPlanPool;

};