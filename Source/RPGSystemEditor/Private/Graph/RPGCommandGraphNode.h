#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "RPGCommandGraphNode.generated.h"

class URPGCommand;

UCLASS()
class RPGSYSTEMEDITOR_API URPGCommandGraphNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<URPGCommand> Command;

	void Initialize(URPGCommand* InCommand);

	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual bool CanUserDeleteNode() const override;
};