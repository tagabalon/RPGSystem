#include "Graph/RPGCommandGraphNode.h"


#include "Commands/RPGCommand.h"
#include "Graph/RPGTriggerGraphSchema.h"

void URPGCommandGraphNode::Initialize(URPGCommand* InCommand)
{
	Command = InCommand;
}

void URPGCommandGraphNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, URPGTriggerGraphSchema::PC_RPGExec, TEXT("In"));
	CreatePin(EGPD_Output, URPGTriggerGraphSchema::PC_RPGExec, TEXT("Out"));
}

FText URPGCommandGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (Command)
	{
		return Command->GetClass()->GetDisplayNameText();
	}

	return FText::FromString(TEXT("RPG Command"));
}

FLinearColor URPGCommandGraphNode::GetNodeTitleColor() const
{
	return FLinearColor(0.08f, 0.18f, 0.35f);
}

bool URPGCommandGraphNode::CanUserDeleteNode() const
{
	return true;
}