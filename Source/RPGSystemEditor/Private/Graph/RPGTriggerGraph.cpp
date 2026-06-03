#include "Graph/RPGTriggerGraph.h"
#include "Graph/RPGTriggerGraphSchema.h"

#include "Data/RPGTriggerData.h"

void URPGTriggerGraph::Initialize(URPGTriggerData* InTriggerData, int32 InStateIndex)
{
	TriggerData = InTriggerData;
	StateIndex = InStateIndex;

	Schema = URPGTriggerGraphSchema::StaticClass();
}

bool URPGTriggerGraph::IsValidGraph() const
{
	return TriggerData != nullptr && StateIndex != INDEX_NONE;
}