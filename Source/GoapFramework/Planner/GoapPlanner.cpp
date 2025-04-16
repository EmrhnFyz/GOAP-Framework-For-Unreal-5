#include "GoapPlanner.h"
#include "../GoapAgent.h"
#include "../Utils/PriorityQueue.h"

UActionPlan* UGoapPlanner::Plan(AGoapAgent* Agent, const TSet<UGoal*>& Goals, UGoal* MostRecentGoal)
{
    TArray<UGoal*> OrderedGoals;
    SetGoalsWithDescendingOrder(Goals, MostRecentGoal, OrderedGoals);

    for (UGoal* Goal : OrderedGoals)
    {
        UNode* GoalNode = CreateGoalNode(Goal);

        TArray<UNode*> SolutionPath;
        if (FindPath(GoalNode, Agent->Actions, SolutionPath))
        {
            UActionPlan* Plan = CreateActionPlan(GoalNode, Goal);
            CleanupNodes(SolutionPath);
            return Plan;
        }
        CleanupGoalResources(GoalNode);
    }
    return nullptr;
}

void UGoapPlanner::SetGoalsWithDescendingOrder(const TSet<UGoal*>& Goals, UGoal* MostRecentGoal, TArray<UGoal*>& OutOrderedGoals)
{
    OutOrderedGoals.Empty();
    for (UGoal* Goal : Goals)
    {
        bool bHasUnmetBelief = false;
        for (UBelief* Belief : Goal->DesiredEffects)
        {
            if (!Belief->Evaluate())
            {
                bHasUnmetBelief = true;
                break;
            }
        }
        if (bHasUnmetBelief)
        {
            OutOrderedGoals.Add(Goal);
        }
    }
    // Sort by priority descending (implement your own sorter if needed)
    OutOrderedGoals.Sort([](const UGoal& A, const UGoal& B) {
        return A.GetPriority() > B.GetPriority();
    });
}

float UGoapPlanner::CalculateHeuristic(UNode* Node) const
{
    return Node->RequiredEffects.Num();
}

float UGoapPlanner::CalculateFScore(UNode* Node) const
{
    return Node->Cost + CalculateHeuristic(Node);
}

void UGoapPlanner::ReconstructPath(UNode* Node)
{
    UNode* Current = Node;
    while (Current->Parent)
    {
        Current->Parent->Leaves.Empty();
        Current->Parent->Leaves.Add(Current);
        Current = Current->Parent;
    }
}

void UGoapPlanner::RemoveSatisfiedEffects(UNode* Node)
{
    TSet<UBelief*> Satisfied;
    for (UBelief* Effect : Node->RequiredEffects)
    {
        if (Effect->Evaluate())
        {
            Satisfied.Add(Effect);
        }
    }
    for (UBelief* Effect : Satisfied)
    {
        Node->RequiredEffects.Remove(Effect);
    }
}

bool UGoapPlanner::FindPath(UNode* StartNode, const TSet<UAgentAction*>& AvailableActions, TArray<UNode*>& OutPath)
{
    TPriorityQueue<UNode*> OpenSet;
    TSet<UNode*> ClosedSet;
    TArray<UNode*> CreatedNodes;
    CreatedNodes.Add(StartNode);

    OpenSet.Enqueue(StartNode, CalculateFScore(StartNode));

    while (!OpenSet.IsEmpty())
    {
        UNode* Current = OpenSet.Dequeue();
        ClosedSet.Add(Current);

        RemoveSatisfiedEffects(Current);

        if (Current->RequiredEffects.Num() == 0)
        {
            ReconstructPath(Current);
            OutPath = CreatedNodes;
            return true;
        }

        for (UAgentAction* Action : AvailableActions)
        {
            // Check if action is relevant
            bool bRelevant = false;
            for (UBelief* Effect : Action->Effects)
            {
                if (Current->RequiredEffects.Contains(Effect))
                {
                    bRelevant = true;
                    break;
                }
            }
            if (!bRelevant) continue;

            // Create neighbor node
            TSet<UBelief*> NewRequiredEffects = Current->RequiredEffects;
            for (UBelief* Effect : Action->Effects)
            {
                NewRequiredEffects.Remove(Effect);
            }
            for (UBelief* Precondition : Action->Preconditions)
            {
                NewRequiredEffects.Add(Precondition);
            }

            UNode* Neighbor = GetNodeFromPool();
            Neighbor->Initialize(Current, Action, NewRequiredEffects, Current->Cost + Action->Cost, true);
            CreatedNodes.Add(Neighbor);

            if (ClosedSet.Contains(Neighbor))
            {
                ReturnNodeToPool(Neighbor);
                CreatedNodes.Pop();
                continue;
            }

            float FScore = Neighbor->Cost + CalculateHeuristic(Neighbor);
            OpenSet.Enqueue(Neighbor, FScore);
        }
    }

    CleanupNodes(CreatedNodes);
    return false;
}

UNode* UGoapPlanner::CreateGoalNode(UGoal* Goal)
{
    TSet<UBelief*> DesiredEffects = Goal->DesiredEffects;
    UNode* GoalNode = GetNodeFromPool();
    GoalNode->Initialize(nullptr, nullptr, DesiredEffects, 0.f, true);
    return GoalNode;
}

void UGoapPlanner::CleanupNodes(TArray<UNode*>& Nodes)
{
    for (UNode* Node : Nodes)
    {
        ReturnNodeToPool(Node);
    }
}

void UGoapPlanner::CleanupGoalResources(UNode* GoalNode)
{
    ReturnNodeToPool(GoalNode);
}

UActionPlan* UGoapPlanner::CreateActionPlan(UNode* GoalNode, UGoal* Goal)
{
    TArray<UAgentAction*> ActionStack;
    UNode* Current = GoalNode;
    while (Current->Leaves.Num() > 0)
    {
        UNode* CheapestLeaf = Current->Leaves[0];
        float MinCost = CheapestLeaf->Cost;
        for (UNode* Leaf : Current->Leaves)
        {
            if (Leaf->Cost < MinCost)
            {
                MinCost = Leaf->Cost;
                CheapestLeaf = Leaf;
            }
        }
        Current = CheapestLeaf;
        ActionStack.Add(Current->Action);
    }
    UActionPlan* Plan = GetActionPlanFromPool();
    Plan->Initialize(Goal, ActionStack, GoalNode->Cost);
    return Plan;
}

// Pooling hooks (implement pooling as needed)
UNode* UGoapPlanner::GetNodeFromPool()
{
    return NodePool.Acquire(this);
}

void UGoapPlanner::ReturnNodeToPool(UNode* Node)
{
    if (Node)
    {
        Node->Reset();
        NodePool.Release(Node);
    }
}

UActionPlan* UGoapPlanner::GetActionPlanFromPool()
{
    return ActionPlanPool.Acquire(this);
}

void UGoapPlanner::ReturnActionPlanToPool(UActionPlan* Plan)
{
    if (Plan)
    {
        Plan->Reset();
        ActionPlanPool.Release(Plan);
    }
}