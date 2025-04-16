#pragma once

#include "CoreMinimal.h"

// Interface requirement: T must have float GetPriority() const;
template<typename T>
class PriorityBasedRadixSorter
{
public:
    // Sorts an array of prioritizables in place. Optionally, recentItem can be deprioritized slightly.
    void Sort(TArray<T>& Items, const T* RecentItem = nullptr, bool bDescending = true)
    {
        if (Items.Num() <= 1)
        {
            return;
        }

        TArray<int32> Priorities;
        Priorities.SetNumUninitialized(Items.Num());
        for (int32 i = 0; i < Items.Num(); i++)
        {
            float Priority = (&Items[i] == RecentItem) ? Items[i].GetPriority() - 0.01f : Items[i].GetPriority();
            Priorities[i] = static_cast<int32>(Priority * 100);
        }

        int32 MaxPriority = Priorities[0];
        for (int32 i = 1; i < Priorities.Num(); i++)
        {
            if (Priorities[i] > MaxPriority)
            {
                MaxPriority = Priorities[i];
            }
        }

        for (int32 Exp = 1; MaxPriority / Exp > 0; Exp *= 10)
        {
            CountingSortByDigit(Items, Priorities, Exp);
        }

        if (bDescending)
        {
            Algo::Reverse(Items);
        }
    }

private:
    void CountingSortByDigit(TArray<T>& Items, const TArray<int32>& Priorities, int32 Exp)
    {
        int32 N = Items.Num();
        TArray<T> Output;
        Output.SetNumUninitialized(N);
        int32 Count[10] = {0};

        for (int32 i = 0; i < N; i++)
        {
            int32 Index = (Priorities[i] / Exp) % 10;
            Index = FMath::Clamp(Index, 0, 9);
            Count[Index]++;
        }

        for (int32 i = 1; i < 10; i++)
        {
            Count[i] += Count[i - 1];
        }

        for (int32 i = N - 1; i >= 0; i--)
        {
            int32 Index = (Priorities[i] / Exp) % 10;
            Index = FMath::Clamp(Index, 0, 9);
            Output[Count[Index] - 1] = Items[i];
            Count[Index]--;
        }

        for (int32 i = 0; i < N; i++)
        {
            Items[i] = Output[i];
        }
    }
};