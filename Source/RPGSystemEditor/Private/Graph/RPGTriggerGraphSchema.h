#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "RPGTriggerGraphSchema.generated.h"

UCLASS()
class RPGSYSTEMEDITOR_API URPGTriggerGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()

public:
	static const FName PC_RPGExec;

	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;

	virtual const FPinConnectionResponse CanCreateConnection(
		const UEdGraphPin* A,
		const UEdGraphPin* B
	) const override;

	virtual bool TryCreateConnection(
		UEdGraphPin* A,
		UEdGraphPin* B
	) const override;

	virtual void BreakNodeLinks(UEdGraphNode& TargetNode) const override;
	virtual void BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotification) const override;
	virtual void BreakSinglePinLink(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin) const override;
};