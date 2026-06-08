#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "RPGTriggerGraph.generated.h"

class URPGTriggerData;

UCLASS()
class RPGSYSTEMEDITOR_API URPGTriggerGraph : public UEdGraph
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<URPGTriggerData> TriggerData;

	UPROPERTY()
	int32 StateIndex = INDEX_NONE;

	void Initialize(URPGTriggerData* InTriggerData, int32 InStateIndex);
	void RebuildFromStateCommands();

	bool IsValidGraph() const;

	virtual void AddNode(UEdGraphNode* NodeToAdd, bool bUserAction = false, bool bSelectNewNode = true) override;
};