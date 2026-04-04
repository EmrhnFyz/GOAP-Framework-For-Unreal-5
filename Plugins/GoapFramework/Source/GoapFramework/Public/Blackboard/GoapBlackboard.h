#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GoapBlackboard.generated.h"

UENUM(BlueprintType)
enum class EGoapValueType : uint8
{
	None UMETA(DisplayName = "None"),
	Bool UMETA(DisplayName = "Bool"),
	Int UMETA(DisplayName = "Int"),
	Float UMETA(DisplayName = "Float"),
	String UMETA(DisplayName = "String"),
	Vector3 UMETA(DisplayName = "Vector3"),
	Object UMETA(DisplayName = "Object"),
	Name UMETA(DisplayName = "Name")
};

USTRUCT(BlueprintType)
struct GOAPFRAMEWORK_API FGoapValue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGoapValueType Type = EGoapValueType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Type == EGoapValueType::Bool", EditConditionHides))
	bool BoolValue = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Type == EGoapValueType::Int", EditConditionHides))
	int32 IntValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Type == EGoapValueType::Float", EditConditionHides))
	float FloatValue = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Type == EGoapValueType::String", EditConditionHides))
	FString StringValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Type == EGoapValueType::Vector3", EditConditionHides))
	FVector Vector3Value = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Type == EGoapValueType::Object", EditConditionHides))
	UObject* ObjectValue = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Type == EGoapValueType::Name", EditConditionHides))
	FName NameValue;

	FGoapValue() = default;

	static FGoapValue MakeBool(bool Value)
	{
		FGoapValue V;
		V.Type = EGoapValueType::Bool;
		V.BoolValue = Value;
		return V;
	}

	static FGoapValue MakeInt(int32 Value)
	{
		FGoapValue V;
		V.Type = EGoapValueType::Int;
		V.IntValue = Value;
		return V;
	}

	static FGoapValue MakeFloat(float Value)
	{
		FGoapValue V;
		V.Type = EGoapValueType::Float;
		V.FloatValue = Value;
		return V;
	}

	static FGoapValue MakeString(const FString& Value)
	{
		FGoapValue V;
		V.Type = EGoapValueType::String;
		V.StringValue = Value;
		return V;
	}

	static FGoapValue MakeVector(const FVector& Value)
	{
		FGoapValue V;
		V.Type = EGoapValueType::Vector3;
		V.Vector3Value = Value;
		return V;
	}

	static FGoapValue MakeObject(UObject* Value)
	{
		FGoapValue V;
		V.Type = EGoapValueType::Object;
		V.ObjectValue = Value;
		return V;
	}

	static FGoapValue MakeName(FName Value)
	{
		FGoapValue V;
		V.Type = EGoapValueType::Name;
		V.NameValue = Value;
		return V;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBlackboardValueChanged, FName, Key, const FGoapValue&, Value);

UCLASS(BlueprintType)
class GOAPFRAMEWORK_API UGoapBlackboard : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Blackboard")
	FOnBlackboardValueChanged OnValueChanged;

	UFUNCTION(BlueprintCallable, Category = "Blackboard")
	void SetBoolValue(FName Key, bool Value);

	UFUNCTION(BlueprintCallable, Category = "Blackboard")
	void SetIntValue(FName Key, int32 Value);

	UFUNCTION(BlueprintCallable, Category = "Blackboard")
	void SetFloatValue(FName Key, float Value);

	UFUNCTION(BlueprintCallable, Category = "Blackboard")
	void SetStringValue(FName Key, const FString& Value);

	UFUNCTION(BlueprintCallable, Category = "Blackboard")
	void SetVectorValue(FName Key, FVector Value);

	UFUNCTION(BlueprintCallable, Category = "Blackboard")
	void SetObjectValue(FName Key, UObject* Value);

	UFUNCTION(BlueprintCallable, Category = "Blackboard")
	void SetNameValue(FName Key, FName Value);

	void SetValue(FName Key, const FGoapValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Blackboard")
	bool TryGetBoolValue(FName Key, bool& OutValue) const;

	UFUNCTION(BlueprintCallable, Category = "Blackboard")
	bool TryGetIntValue(FName Key, int32& OutValue) const;

	UFUNCTION(BlueprintCallable, Category = "Blackboard")
	bool TryGetFloatValue(FName Key, float& OutValue) const;

	UFUNCTION(BlueprintCallable, Category = "Blackboard")
	bool TryGetStringValue(FName Key, FString& OutValue) const;

	UFUNCTION(BlueprintCallable, Category = "Blackboard")
	bool TryGetVectorValue(FName Key, FVector& OutValue) const;

	UFUNCTION(BlueprintCallable, Category = "Blackboard")
	bool TryGetObjectValue(FName Key, UObject*& OutValue) const;

	UFUNCTION(BlueprintCallable, Category = "Blackboard")
	bool TryGetNameValue(FName Key, FName& OutValue) const;

	bool TryGetValue(FName Key, FGoapValue& OutValue) const;

	UFUNCTION(BlueprintCallable, Category = "Blackboard")
	bool ContainsKey(FName Key) const;

	UFUNCTION(BlueprintCallable, Category = "Blackboard")
	bool RemoveKey(FName Key);

	UFUNCTION(BlueprintCallable, Category = "Blackboard")
	void Clear();

	const TMap<FName, FGoapValue>& GetAllEntries() const { return Entries; }

private:
	UPROPERTY()
	TMap<FName, FGoapValue> Entries;
};
