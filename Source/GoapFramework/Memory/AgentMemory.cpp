#include "AgentMemory.h"

void UAgentMemory::AddMemory(const FMemoryEntry& Entry)
{
	Entries.Add(Entry);
}

TArray<FMemoryEntry> UAgentMemory::GetMemories(EMemoryType Type) const
{
	TArray<FMemoryEntry> Result;
	for (const FMemoryEntry& Entry : Entries)
	{
		if (Entry.Type == Type)
		{
			Result.Add(Entry);
		}
	}
	return Result;
}

void UAgentMemory::RemoveMemory(const FMemoryEntry& Entry)
{
	Entries.RemoveAll([&Entry](const FMemoryEntry& Existing)
	{
		return Existing == Entry;
	});
}

void UAgentMemory::CleanOldMemories(float CurrentTime, float MaxAge)
{
	Entries.RemoveAll([CurrentTime, MaxAge](const FMemoryEntry& Entry)
	{
		return (CurrentTime - Entry.TimeStamp) > MaxAge;
	});
}

void UAgentMemory::ClearMemories()
{
	Entries.Empty();
}
