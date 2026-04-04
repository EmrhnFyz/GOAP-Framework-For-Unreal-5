#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AgentMemory.generated.h"

UENUM(BlueprintType)
enum class EMemoryType : uint8
{
	None = 0 UMETA(DisplayName = "None")
	// Add your memory types here
};

USTRUCT(BlueprintType)
struct GOAPFRAMEWORK_API FMemoryEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMemoryType Type = EMemoryType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Position = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeStamp = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* ExtraData = nullptr;

	FMemoryEntry() = default;

	FMemoryEntry(EMemoryType InType, FVector InPosition, float InTimeStamp, UObject* InExtraData = nullptr)
		: Type(InType), Position(InPosition), TimeStamp(InTimeStamp), ExtraData(InExtraData)
	{
	}

	bool operator==(const FMemoryEntry& Other) const
	{
		return Type == Other.Type
			&& Position.Equals(Other.Position)
			&& FMath::IsNearlyEqual(TimeStamp, Other.TimeStamp)
			&& ExtraData == Other.ExtraData;
	}
};

UCLASS(BlueprintType)
class GOAPFRAMEWORK_API UAgentMemory : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Memory")
	void AddMemory(const FMemoryEntry& Entry);

	UFUNCTION(BlueprintCallable, Category = "Memory")
	TArray<FMemoryEntry> GetMemories(EMemoryType Type) const;

	UFUNCTION(BlueprintCallable, Category = "Memory")
	void RemoveMemory(const FMemoryEntry& Entry);

	UFUNCTION(BlueprintCallable, Category = "Memory")
	void CleanOldMemories(float CurrentTime, float MaxAge);

	UFUNCTION(BlueprintCallable, Category = "Memory")
	void ClearMemories();

	UFUNCTION(BlueprintCallable, Category = "Memory")
	int32 GetMemoryCount() const { return Entries.Num(); }

private:
	UPROPERTY()
	TArray<FMemoryEntry> Entries;
};
