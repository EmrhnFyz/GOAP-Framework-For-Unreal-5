#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Templates/SubclassOf.h"

// Template UObject pool
template<typename T>
class TObjectPool
{
public:
	TObjectPool() {}

	// Get an object from the pool or create a new one
	T* Acquire(UObject* Outer)
	{
		if (Pool.Num() > 0)
		{
			T* Obj = Pool.Pop();
			// Optionally reset Obj here
			return Obj;
		}
		return NewObject<T>(Outer);
	}

	// Return an object to the pool
	void Release(T* Obj)
	{
		if (Obj)
		{
			// Optionally reset Obj here
			Pool.Push(Obj);
		}
	}

	// Optional: Clear the pool
	void Clear()
	{
		Pool.Empty();
	}

private:
	TArray<T*> Pool;
};