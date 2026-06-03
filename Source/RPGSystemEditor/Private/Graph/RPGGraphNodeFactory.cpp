#include "Graph/RPGGraphNodeFactory.h"

#include "Graph/RPGCommandGraphNode.h"
#include "Slate/SGraphNode_RPGCommand.h"

TSharedPtr<SGraphNode> FRPGGraphNodeFactory::CreateNode(UEdGraphNode* Node) const
{
	if (URPGCommandGraphNode* RPGNode = Cast<URPGCommandGraphNode>(Node))
	{
		return SNew(SGraphNode_RPGCommand, RPGNode);
	}

	return nullptr;
}