#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SGraphEditor;
class SWidgetSwitcher;
class URPGTriggerData;
class URPGTriggerGraph;

class RPGSYSTEMEDITOR_API SRPGTriggerGraphEditor : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRPGTriggerGraphEditor) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, URPGTriggerData* InTriggerData);

private:
	TObjectPtr<URPGTriggerData> TriggerData;

	TSharedPtr<SWidgetSwitcher> StatePageSwitcher;
	
	TArray<TObjectPtr<URPGTriggerGraph>> StateGraphs;

	TSharedRef<SWidget> BuildStateTabs();
	TSharedRef<SWidget> BuildStatePage(int32 StateIndex);

	void SelectState(int32 StateIndex);

	URPGTriggerGraph* CreateGraphForState(int32 StateIndex);
	TSharedRef<SWidget> CreateStateDetailsView(int32 StateIndex);
};