#include "Editor/SRPGTriggerGraphEditor.h"

#include "Data/RPGTriggerData.h"
#include "Graph/RPGTriggerGraph.h"

#include "GraphEditor.h"
#include "Widgets/Layout/SBorder.h"

void SRPGTriggerGraphEditor::Construct(const FArguments& InArgs, URPGTriggerData* InTriggerData, int32 InStateIndex)
{
	TriggerData = InTriggerData;
	Graph = CreateGraph(InTriggerData, InStateIndex);

	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = FText::FromString(TEXT("RPGSystem Trigger Editor"));

	SGraphEditor::FGraphEditorEvents GraphEvents;

	ChildSlot
		[
			SNew(SBorder)
				[
					SAssignNew(GraphEditor, SGraphEditor)
						.Appearance(AppearanceInfo)
						.GraphToEdit(Graph)
						.GraphEvents(GraphEvents)
						.AutoExpandActionMenu(true)
				]
		];
}

URPGTriggerGraph* SRPGTriggerGraphEditor::CreateGraph(URPGTriggerData* InTriggerData, int32 InStateIndex
)
{
	URPGTriggerGraph* NewGraph = NewObject<URPGTriggerGraph>(
		InTriggerData,
		URPGTriggerGraph::StaticClass(),
		NAME_None,
		RF_Transactional
	);

	NewGraph->Initialize(InTriggerData, InStateIndex);

	return NewGraph;
}