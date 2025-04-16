// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "../Actions/AgentAction.h"
#include "../Beliefs/Belief.h"
#include "Node.generated.h"

UCLASS(BlueprintType)
class GOAPFRAMEWORK_API UNode : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	UNode* Parent = nullptr;

	UPROPERTY(BlueprintReadOnly)
	UAgentAction* Action = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TSet<UBelief*> RequiredEffects;

	UPROPERTY(BlueprintReadOnly)
	TArray<UNode*> Leaves;

	UPROPERTY(BlueprintReadOnly)
	float Cost = 0.f;

	// Is this node a dead leaf (no children and no action)?
	UFUNCTION(BlueprintPure)
	bool IsLeafDead() const
	{
		return Leaves.Num() == 0 && Action == nullptr;
	}

	// Reset for pool reuse
    UFUNCTION(BlueprintCallable)
	void Reset();

	// Initialize for pooled instances
    UFUNCTION(BlueprintCallable)
	void Initialize(UNode* InParent, UAgentAction* InAction, const TSet<UBelief*>& Effects, float InCost, bool bReuseEffects = false);
};