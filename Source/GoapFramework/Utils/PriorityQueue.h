#pragma once

#include "CoreMinimal.h"

// Min-heap priority queue for any type T
template<typename T>
class TPriorityQueue
{
public:
    // Pair of item and its priority
    struct FElement
    {
        T Item;
        float Priority;

        FElement(const T& InItem, float InPriority)
            : Item(InItem), Priority(InPriority) {}
    };

    // Add an item with a priority
    void Enqueue(const T& Item, float Priority)
    {
        Elements.Emplace(Item, Priority);
        int32 ChildIndex = Elements.Num() - 1;
        while (ChildIndex > 0)
        {
            int32 ParentIndex = (ChildIndex - 1) / 2;
            if (Elements[ParentIndex].Priority <= Elements[ChildIndex].Priority)
            {
                break;
            }
            Swap(Elements[ParentIndex], Elements[ChildIndex]);
            ChildIndex = ParentIndex;
        }
    }

    // Remove and return the item with the lowest priority
    T Dequeue()
    {
        check(Elements.Num() > 0);
        T Result = Elements[0].Item;
        Elements[0] = Elements.Last();
        Elements.Pop();

        int32 ParentIndex = 0;
        while (true)
        {
            int32 MinIndex = ParentIndex;
            int32 LeftChild = 2 * ParentIndex + 1;
            int32 RightChild = 2 * ParentIndex + 2;

            if (LeftChild < Elements.Num() && Elements[LeftChild].Priority < Elements[MinIndex].Priority)
            {
                MinIndex = LeftChild;
            }
            if (RightChild < Elements.Num() && Elements[RightChild].Priority < Elements[MinIndex].Priority)
            {
                MinIndex = RightChild;
            }
            if (MinIndex == ParentIndex)
            {
                break;
            }
            Swap(Elements[ParentIndex], Elements[MinIndex]);
            ParentIndex = MinIndex;
        }

        return Result;
    }

    // Number of items in the queue
    int32 Num() const
    {
        return Elements.Num();
    }

    // Remove all items
    void Clear()
    {
        Elements.Empty();
    }

    // Is the queue empty?
    bool IsEmpty() const
    {
        return Elements.Num() == 0;
    }

private:
    TArray<FElement> Elements;
};