#include "Graph/RPGGraphNodeFactory.h"

#include "Graph/RPGCommandGraphNode.h"
#include "Graph/RPGStartGraphNode.h"
#include "Slate/SGraphNode_RPGCommand.h"
#include "Slate/SGraphNode_StartNode.h"

TSharedPtr<SGraphNode> FRPGGraphNodeFactory::CreateNode(UEdGraphNode* Node) const
{
	if (URPGCommandGraphNode* RPGNode = Cast<URPGCommandGraphNode>(Node))
	{
		return SNew(SGraphNode_RPGCommand, RPGNode);
	}
	else if (URPGStartGraphNode* StartNode = Cast<URPGStartGraphNode>(Node))
	{
		// We can create a custom SGraphNode for the start node if we want, but for now we'll just use the default one.
		return SNew(SGraphNode_StartNode, StartNode);
	}

	return nullptr;
}