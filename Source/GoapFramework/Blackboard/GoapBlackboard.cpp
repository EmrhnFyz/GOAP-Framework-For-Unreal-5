#include "GoapBlackboard.h"

void UGoapBlackboard::SetBoolValue(FName Key, bool Value)
{
	SetValue(Key, FGoapValue::MakeBool(Value));
}

void UGoapBlackboard::SetIntValue(FName Key, int32 Value)
{
	SetValue(Key, FGoapValue::MakeInt(Value));
}

void UGoapBlackboard::SetFloatValue(FName Key, float Value)
{
	SetValue(Key, FGoapValue::MakeFloat(Value));
}

void UGoapBlackboard::SetStringValue(FName Key, const FString& Value)
{
	SetValue(Key, FGoapValue::MakeString(Value));
}

void UGoapBlackboard::SetVectorValue(FName Key, FVector Value)
{
	SetValue(Key, FGoapValue::MakeVector(Value));
}

void UGoapBlackboard::SetObjectValue(FName Key, UObject* Value)
{
	SetValue(Key, FGoapValue::MakeObject(Value));
}

void UGoapBlackboard::SetNameValue(FName Key, FName Value)
{
	SetValue(Key, FGoapValue::MakeName(Value));
}

void UGoapBlackboard::SetValue(FName Key, const FGoapValue& Value)
{
	Entries.Add(Key, Value);
	OnValueChanged.Broadcast(Key, Value);
}

bool UGoapBlackboard::TryGetBoolValue(FName Key, bool& OutValue) const
{
	const FGoapValue* Found = Entries.Find(Key);
	if (Found && Found->Type == EGoapValueType::Bool)
	{
		OutValue = Found->BoolValue;
		return true;
	}
	return false;
}

bool UGoapBlackboard::TryGetIntValue(FName Key, int32& OutValue) const
{
	const FGoapValue* Found = Entries.Find(Key);
	if (Found && Found->Type == EGoapValueType::Int)
	{
		OutValue = Found->IntValue;
		return true;
	}
	return false;
}

bool UGoapBlackboard::TryGetFloatValue(FName Key, float& OutValue) const
{
	const FGoapValue* Found = Entries.Find(Key);
	if (Found && Found->Type == EGoapValueType::Float)
	{
		OutValue = Found->FloatValue;
		return true;
	}
	return false;
}

bool UGoapBlackboard::TryGetStringValue(FName Key, FString& OutValue) const
{
	const FGoapValue* Found = Entries.Find(Key);
	if (Found && Found->Type == EGoapValueType::String)
	{
		OutValue = Found->StringValue;
		return true;
	}
	return false;
}

bool UGoapBlackboard::TryGetVectorValue(FName Key, FVector& OutValue) const
{
	const FGoapValue* Found = Entries.Find(Key);
	if (Found && Found->Type == EGoapValueType::Vector3)
	{
		OutValue = Found->Vector3Value;
		return true;
	}
	return false;
}

bool UGoapBlackboard::TryGetObjectValue(FName Key, UObject*& OutValue) const
{
	const FGoapValue* Found = Entries.Find(Key);
	if (Found && Found->Type == EGoapValueType::Object)
	{
		OutValue = Found->ObjectValue;
		return true;
	}
	return false;
}

bool UGoapBlackboard::TryGetNameValue(FName Key, FName& OutValue) const
{
	const FGoapValue* Found = Entries.Find(Key);
	if (Found && Found->Type == EGoapValueType::Name)
	{
		OutValue = Found->NameValue;
		return true;
	}
	return false;
}

bool UGoapBlackboard::TryGetValue(FName Key, FGoapValue& OutValue) const
{
	const FGoapValue* Found = Entries.Find(Key);
	if (Found)
	{
		OutValue = *Found;
		return true;
	}
	return false;
}

bool UGoapBlackboard::ContainsKey(FName Key) const
{
	return Entries.Contains(Key);
}

bool UGoapBlackboard::RemoveKey(FName Key)
{
	return Entries.Remove(Key) > 0;
}

void UGoapBlackboard::Clear()
{
	Entries.Empty();
}
