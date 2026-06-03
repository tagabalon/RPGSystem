#include "Graph/RPGTriggerGraphSchema.h"

#include "Commands/RPGCommand.h"
#include "Graph/RPGTriggerGraphSchemaActions.h"
#include "UObject/UObjectIterator.h"

const FName URPGTriggerGraphSchema::PC_RPGExec(TEXT("RPGExec"));

void URPGTriggerGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	const FText Category = FText::FromString(TEXT("RPG Commands"));

	for (TObjectIterator<UClass> It; It; ++It)
	{
		UClass* Class = *It;

		if (!Class->IsChildOf(URPGCommand::StaticClass()))
		{
			continue;
		}

		if (Class->HasAnyClassFlags(CLASS_Abstract | CLASS_Deprecated | CLASS_NewerVersionExists))
		{
			continue;
		}

		const FText MenuDesc = Class->GetDisplayNameText();
		const FText ToolTip = FText::Format(
			FText::FromString(TEXT("Add {0} command")),
			MenuDesc
		);

		TSharedPtr<FRPGTriggerGraphSchemaAction_NewCommand> Action =
			MakeShared<FRPGTriggerGraphSchemaAction_NewCommand>(
				Category,
				MenuDesc,
				ToolTip,
				0,
				Class
			);

		ContextMenuBuilder.AddAction(Action);
	}
}

const FPinConnectionResponse URPGTriggerGraphSchema::CanCreateConnection(
	const UEdGraphPin* A,
	const UEdGraphPin* B
) const
{
	if (!A || !B)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Invalid pin"));
	}

	if (A == B)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Cannot connect pin to itself"));
	}

	if (A->GetOwningNode() == B->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Cannot connect node to itself"));
	}

	if (A->Direction == B->Direction)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Pins must have opposite directions"));
	}

	if (A->PinType.PinCategory != PC_RPGExec || B->PinType.PinCategory != PC_RPGExec)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Pins are not RPG execution pins"));
	}

	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
}

bool URPGTriggerGraphSchema::TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const
{
	if (!A || !B)
	{
		return false;
	}

	// Keep command flow linear for now:
	// one output can only connect to one input.
	UEdGraphPin* OutputPin = A->Direction == EGPD_Output ? A : B;

	if (OutputPin)
	{
		OutputPin->BreakAllPinLinks();
	}

	return UEdGraphSchema::TryCreateConnection(A, B);
}

void URPGTriggerGraphSchema::BreakNodeLinks(UEdGraphNode& TargetNode) const
{
	Super::BreakNodeLinks(TargetNode);
}

void URPGTriggerGraphSchema::BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotification) const
{
	Super::BreakPinLinks(TargetPin, bSendsNodeNotification);
}

void URPGTriggerGraphSchema::BreakSinglePinLink(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin) const
{
	Super::BreakSinglePinLink(SourcePin, TargetPin);
}