#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"
#include "SNodePanel.h"

class UStartGraphNode;

class SGraphNode_StartNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SGraphNode_StartNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, URPGStartGraphNode* InNode);

	virtual void UpdateGraphNode() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;

};