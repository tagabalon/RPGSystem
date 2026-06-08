#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"

#include "RPGStartGraphNode.generated.h"

UCLASS()
class RPGSYSTEMEDITOR_API URPGStartGraphNode : public UEdGraphNode
{
	GENERATED_BODY()
public:

	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual bool CanUserDeleteNode() const override;
	virtual bool CanDuplicateNode() const override;
};