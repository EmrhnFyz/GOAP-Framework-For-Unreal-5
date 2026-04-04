// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IActionStrategy.generated.h"

UINTERFACE(BlueprintType)
class GOAPFRAMEWORK_API UActionStrategy : public UInterface
{
	GENERATED_BODY()
};

class GOAPFRAMEWORK_API IActionStrategy
{
	GENERATED_BODY()

public:
	// Can the action be performed now?
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GOAP")
	bool CanPerform() const;

	// Is the action complete?
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GOAP")
	bool IsComplete() const;

	// Called when the action starts
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GOAP")
	void Start();

	// Called every tick/update
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GOAP")
	void Update(float DeltaTime);

	// Called when the action stops
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GOAP")
	void Stop();
};