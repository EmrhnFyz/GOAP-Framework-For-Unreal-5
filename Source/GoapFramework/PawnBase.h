#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PawnBase.generated.h"

UENUM(BlueprintType)
enum class EPawnParty : uint8
{
	None,
	PartyA,
	PartyB,
	// Add more as needed
};

USTRUCT(BlueprintType)
struct GOAPFRAMEWORK_API FRuntimeStatus
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PartyID = 0;
};

UCLASS(Blueprintable)
class GOAPFRAMEWORK_API APawnBase : public ACharacter
{
	GENERATED_BODY()

public:
	APawnBase();

	// Runtime status (health, party, etc.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
	FRuntimeStatus RuntimeStatus;

	// Tags for filtering (expand as needed)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
	TSet<FName> PawnTags;

	// Called to check if this pawn has a specific tag
	UFUNCTION(BlueprintCallable, Category = "GOAP")
	bool HasPawnTag(FName Tag) const { return PawnTags.Contains(Tag); }

	// Called to check if this pawn is alive
	UFUNCTION(BlueprintCallable, Category = "GOAP")
	bool IsAlive() const { return RuntimeStatus.CurrentHealth > 0; }

	// Extendable: Called when the pawn takes damage
	UFUNCTION(BlueprintNativeEvent, Category = "GOAP")
	void OnTakeDamage(int32 Amount);

	// Extendable: Called when the pawn dies
	UFUNCTION(BlueprintNativeEvent, Category = "GOAP")
	void OnDeath();
};