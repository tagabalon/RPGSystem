#include "Graph/RPGTriggerGraphSchemaActions.h"

#include "Commands/RPGCommand.h"
#include "Graph/RPGCommandGraphNode.h"

#include "EdGraph/EdGraph.h"
#include "ScopedTransaction.h"

UEdGraphNode* FRPGTriggerGraphSchemaAction_NewCommand::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	if (!ParentGraph || !CommandClass)
	{
		return nullptr;
	}

	const FScopedTransaction Transaction(NSLOCTEXT("RPGTriggerEditor", "AddCommandNode", "Add RPG Command Node"));

	ParentGraph->Modify();

	URPGCommand* NewCommand = NewObject<URPGCommand>(ParentGraph, CommandClass, NAME_None, RF_Transactional);

	URPGCommandGraphNode* NewNode = NewObject<URPGCommandGraphNode>(ParentGraph, URPGCommandGraphNode::StaticClass(), NAME_None, RF_Transactional);

	NewNode->Initialize(NewCommand);
	NewNode->NodePosX = Location.X;
	NewNode->NodePosY = Location.Y;

	ParentGraph->AddNode(NewNode, true, bSelectNewNode);

	NewNode->CreateNewGuid();
	NewNode->AllocateDefaultPins();
	NewNode->AutowireNewNode(FromPin);

	return NewNode;
}