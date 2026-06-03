#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SGraphEditor;
class URPGTriggerData;
class URPGTriggerGraph;

class RPGSYSTEMEDITOR_API SRPGTriggerGraphEditor : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRPGTriggerGraphEditor) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, URPGTriggerData* InTriggerData, int32 InStateIndex);

private:
	TObjectPtr<URPGTriggerData> TriggerData;
	TObjectPtr<URPGTriggerGraph> Graph;

	TSharedPtr<SGraphEditor> GraphEditor;

	URPGTriggerGraph* CreateGraph(URPGTriggerData* InTriggerData, int32 InStateIndex);
};