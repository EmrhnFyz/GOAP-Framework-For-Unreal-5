#include "GoapAgent.h"
#include "PawnBase.h"

AGoapAgent::AGoapAgent()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AGoapAgent::BeginPlay()
{
    Super::BeginPlay();

    SetupDictionaries();
    SetupTimers();
    SetupBeliefs();
    SetupActions();
    SetupGoals();

    Planner = NewObject<UGoapPlanner>(this);
}

void AGoapAgent::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    bIsAlive = Pawn && Pawn->IsAlive();
    if (!bIsAlive)
    {
        return;
    }

    CalculatePlan();

    // Plan execution logic
    if (!CurrentAction)
    {
        if (ActionPlan && ActionPlan->Actions.Num() > 0)
        {
            CurrentGoal = ActionPlan->AgentGoal;
            CurrentAction = ActionPlan->Actions.Pop();

            // Check preconditions
            bool bAllPreconditionsTrue = true;
            for (UBelief* Precondition : CurrentAction->Preconditions)
            {
                if (!Precondition->Evaluate())
                {
                    bAllPreconditionsTrue = false;
                    break;
                }
            }

            if (bAllPreconditionsTrue)
            {
                CurrentAction->Start();
            }
            else
            {
                CurrentAction = nullptr;
                CurrentGoal = nullptr;
            }
        }
    }

    if (ActionPlan && CurrentAction)
    {
        CurrentAction->Update(DeltaTime);

        if (!CurrentAction->CanPerform())
        {
            CurrentAction->Stop();
            CurrentAction = nullptr;
            if (ActionPlan->Actions.Num() == 0)
            {
                LastGoal = CurrentGoal;
                CurrentGoal = nullptr;
            }
            return;
        }

        if (CurrentGoal && CurrentGoal != ActionPlan->AgentGoal)
        {
            CurrentAction->Stop();
            CurrentAction = nullptr;
            return;
        }

        if (CurrentAction->IsComplete())
        {
            CurrentAction->Stop();
            CurrentAction = nullptr;
            if (ActionPlan->Actions.Num() == 0)
            {
                LastGoal = CurrentGoal;
                CurrentGoal = nullptr;
            }
        }
    }
}

void AGoapAgent::SetupDictionaries()
{
    SensorMap.Empty();
    for (const FSensorData& SensorData : Sensors)
    {
        SensorMap.Add(SensorData.Name, SensorData.Sensor);
    }
}

void AGoapAgent::SetupTimers() {}
void AGoapAgent::SetupBeliefs() { Beliefs.Empty(); }
void AGoapAgent::SetupActions() { Actions.Empty(); }
void AGoapAgent::SetupGoals() { Goals.Empty(); }

ASensorBase* AGoapAgent::GetSensor(ESensorName SensorName) const
{
    if (SensorMap.Contains(SensorName))
    {
        return SensorMap[SensorName];
    }
    return nullptr;
}

void AGoapAgent::CalculatePlan()
{
    if (Goals.Num() == 0)
    {
        return;
    }

    TSet<UGoal*> GoalsToCheck;
    bool bUsingPooledSet = false;

    if (CurrentGoal)
    {
        float PriorityLevel = CurrentGoal->Priority;
        bool bHasHigherPriorityGoals = false;
        for (UGoal* Goal : Goals)
        {
            if (Goal->Priority > PriorityLevel)
            {
                bHasHigherPriorityGoals = true;
                break;
            }
        }
        if (!bHasHigherPriorityGoals)
        {
            return;
        }

        GoalsToCheck = GetGoalSetFromPool();
        bUsingPooledSet = true;

        for (UGoal* Goal : Goals)
        {
            if (Goal->Priority > PriorityLevel)
            {
                GoalsToCheck.Add(Goal);
            }
        }
    }
    else
    {
        GoalsToCheck = Goals;
    }

    UActionPlan* PotentialPlan = Planner->Plan(this, GoalsToCheck, LastGoal);

    if (PotentialPlan)
    {
        if (ActionPlan)
        {
            Planner->ReturnActionPlanToPool(ActionPlan);
        }
        ActionPlan = PotentialPlan;
    }

    if (bUsingPooledSet)
    {
        ReturnGoalSetToPool(GoalsToCheck);
    }
}

// Pooling hooks (implement pooling as needed)
TSet<UGoal*> AGoapAgent::GetGoalSetFromPool()
{
    // Replace with your pooling logic if needed
    return TSet<UGoal*>();
}

void AGoapAgent::ReturnGoalSetToPool(TSet<UGoal*>& Set)
{
    // Replace with your pooling logic if needed
    Set.Empty();
}