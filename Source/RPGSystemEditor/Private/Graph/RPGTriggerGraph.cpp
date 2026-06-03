#include "Graph/RPGTriggerGraph.h"

#include "Graph/RPGCommandGraphNode.h"
#include "Graph/RPGTriggerGraphSchema.h"

#include "Data/RPGTriggerData.h"
#include "Commands/RPGCommand.h"

void URPGTriggerGraph::Initialize(URPGTriggerData* InTriggerData, int32 InStateIndex)
{
	TriggerData = InTriggerData;
	StateIndex = InStateIndex;

	Schema = URPGTriggerGraphSchema::StaticClass();

	RebuildFromStateCommands();
}

void URPGTriggerGraph::RebuildFromStateCommands()
{
	Nodes.Reset();

	if (!TriggerData || !TriggerData->States.IsValidIndex(StateIndex))
	{
		return;
	}

	const FRPGTriggerState& State = TriggerData->States[StateIndex];

	const int32 NodeSpacingY = 140;
	const int32 StartX = 200;
	const int32 StartY = 100;

	URPGCommandGraphNode* PreviousNode = nullptr;

	for (int32 CommandIndex = 0; CommandIndex < State.Commands.Num(); ++CommandIndex)
	{
		URPGCommand* Command = State.Commands[CommandIndex];
		if (!Command)
		{
			continue;
		}

		URPGCommandGraphNode* NewNode = NewObject<URPGCommandGraphNode>(
			this,
			URPGCommandGraphNode::StaticClass(),
			NAME_None,
			RF_Transactional
		);

		NewNode->Initialize(Command);
		NewNode->CreateNewGuid();
		NewNode->NodePosX = StartX;
		NewNode->NodePosY = StartY + CommandIndex * NodeSpacingY;

		AddNode(NewNode, false, false);

		NewNode->AllocateDefaultPins();

		if (PreviousNode)
		{
			UEdGraphPin* PreviousOut = PreviousNode->FindPin(TEXT("Out"));
			UEdGraphPin* CurrentIn = NewNode->FindPin(TEXT("In"));

			if (PreviousOut && CurrentIn)
			{
				PreviousOut->MakeLinkTo(CurrentIn);
			}
		}

		PreviousNode = NewNode;
	}
}

bool URPGTriggerGraph::IsValidGraph() const
{
	return TriggerData != nullptr && StateIndex != INDEX_NONE;
}