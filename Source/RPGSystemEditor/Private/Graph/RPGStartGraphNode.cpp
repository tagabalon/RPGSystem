#include "Graph/RPGStartGraphNode.h"

#include "Commands/RPGCommand.h"
#include "Graph/RPGTriggerGraphSchema.h"


void URPGStartGraphNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Output, URPGTriggerGraphSchema::PC_RPGExec, TEXT("Out"));
}

FText URPGStartGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Start"));
}

FLinearColor URPGStartGraphNode::GetNodeTitleColor() const
{
	return FLinearColor(0.08f, 0.18f, 0.35f);
}

bool URPGStartGraphNode::CanUserDeleteNode() const
{
	return false;
}

bool URPGStartGraphNode::CanDuplicateNode() const
{
	return false;
}