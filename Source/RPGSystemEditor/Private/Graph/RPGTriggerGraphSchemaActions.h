#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"

class URPGCommand;

struct FRPGTriggerGraphSchemaAction_NewCommand : public FEdGraphSchemaAction
{
public:
	TSubclassOf<URPGCommand> CommandClass;

	FRPGTriggerGraphSchemaAction_NewCommand()
		: FEdGraphSchemaAction()
	{
	}

	FRPGTriggerGraphSchemaAction_NewCommand(
		const FText& InNodeCategory,
		const FText& InMenuDesc,
		const FText& InToolTip,
		const int32 InGrouping,
		TSubclassOf<URPGCommand> InCommandClass
	)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping)
		, CommandClass(InCommandClass)
	{
	}

	virtual UEdGraphNode* PerformAction(
		UEdGraph* ParentGraph,
		UEdGraphPin* FromPin,
		const FVector2D Location,
		bool bSelectNewNode = true
	) override;
};