#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GoapBlackboard.h"
#include "GoapBlackboardData.generated.h"

USTRUCT(BlueprintType)
struct GOAPFRAMEWORK_API FBlackboardEntryData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGoapValue Value;
};

UCLASS(BlueprintType)
class GOAPFRAMEWORK_API UGoapBlackboardData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	TArray<FBlackboardEntryData> DefaultEntries;

	UFUNCTION(BlueprintCallable, Category = "Blackboard")
	void SetValuesOnBlackboard(UGoapBlackboard* Blackboard) const;

	UFUNCTION(BlueprintCallable, Category = "Blackboard")
	void SetDefaultValuesOnBlackboard(UGoapBlackboard* Blackboard) const;
};
