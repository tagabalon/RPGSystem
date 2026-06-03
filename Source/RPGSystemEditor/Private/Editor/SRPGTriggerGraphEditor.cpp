#include "Editor/SRPGTriggerGraphEditor.h"

#include "Data/RPGTriggerData.h"
#include "Graph/RPGTriggerGraph.h"

#include "GraphEditor.h"
#include "PropertyEditorModule.h"
#include "IStructureDetailsView.h"

#include "Modules/ModuleManager.h"
#include "UObject/StructOnScope.h"

#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSplitter.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
#include "Widgets/Text/STextBlock.h"

void SRPGTriggerGraphEditor::Construct(const FArguments& InArgs, URPGTriggerData* InTriggerData)
{
	TriggerData = InTriggerData;

	ChildSlot
		[
			SNew(SVerticalBox)

				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					BuildStateTabs()
				]

				+ SVerticalBox::Slot()
				.FillHeight(1.0f)
				[
					SAssignNew(StatePageSwitcher, SWidgetSwitcher)
				]
		];

	if (!TriggerData)
	{
		return;
	}

	for (int32 StateIndex = 0; StateIndex < TriggerData->States.Num(); ++StateIndex)
	{
		StateGraphs.Add(CreateGraphForState(StateIndex));

		StatePageSwitcher->AddSlot()
			[
				BuildStatePage(StateIndex)
			];
	}

	SelectState(0);
}

TSharedRef<SWidget> SRPGTriggerGraphEditor::BuildStateTabs()
{
	TSharedRef<SScrollBox> ScrollBox =
		SNew(SScrollBox)
		.Orientation(Orient_Horizontal);

	if (!TriggerData)
	{
		return ScrollBox;
	}

	for (int32 StateIndex = 0; StateIndex < TriggerData->States.Num(); ++StateIndex)
	{
		const int32 CapturedStateIndex = StateIndex;

		ScrollBox->AddSlot()
			.Padding(2.0f)
			[
				SNew(SButton)
					.Text(FText::Format(
						FText::FromString(TEXT("State {0}")),
						FText::AsNumber(StateIndex)
					))
					.OnClicked_Lambda([this, CapturedStateIndex]()
						{
							SelectState(CapturedStateIndex);
							return FReply::Handled();
						})
			];
	}

	return ScrollBox;
}

TSharedRef<SWidget> SRPGTriggerGraphEditor::BuildStatePage(int32 StateIndex)
{
	URPGTriggerGraph* Graph = StateGraphs.IsValidIndex(StateIndex)
		? StateGraphs[StateIndex]
		: nullptr;

	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = FText::Format(
		FText::FromString(TEXT("Trigger State {0}")),
		FText::AsNumber(StateIndex)
	);

	SGraphEditor::FGraphEditorEvents GraphEvents;

	return SNew(SSplitter)

		+ SSplitter::Slot()
		.Value(0.25f)
		[
			SNew(SBorder)
				.Padding(4.0f)
				[
					CreateStateDetailsView(StateIndex)
				]
		]

	+ SSplitter::Slot()
		.Value(0.75f)
		[
			SNew(SBorder)
				[
					SNew(SGraphEditor)
						.Appearance(AppearanceInfo)
						.GraphToEdit(Graph)
						.GraphEvents(GraphEvents)
						.AutoExpandActionMenu(true)
				]
		];
}

void SRPGTriggerGraphEditor::SelectState(int32 StateIndex)
{
	if (StatePageSwitcher.IsValid())
	{
		StatePageSwitcher->SetActiveWidgetIndex(StateIndex);
	}
}

URPGTriggerGraph* SRPGTriggerGraphEditor::CreateGraphForState(int32 StateIndex)
{
	URPGTriggerGraph* NewGraph = NewObject<URPGTriggerGraph>(
		TriggerData,
		URPGTriggerGraph::StaticClass(),
		NAME_None,
		RF_Transactional
	);

	NewGraph->Initialize(TriggerData, StateIndex);

	return NewGraph;
}

TSharedRef<SWidget> SRPGTriggerGraphEditor::CreateStateDetailsView(int32 StateIndex)
{
	if (!TriggerData || !TriggerData->States.IsValidIndex(StateIndex))
	{
		return SNew(STextBlock)
			.Text(FText::FromString(TEXT("Invalid state")));
	}

	FPropertyEditorModule& PropertyEditorModule =
		FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bAllowSearch = true;
	DetailsViewArgs.bHideSelectionTip = true;
	DetailsViewArgs.bShowOptions = false;

	FStructureDetailsViewArgs StructureViewArgs;
	StructureViewArgs.bShowObjects = true;
	StructureViewArgs.bShowAssets = true;
	StructureViewArgs.bShowClasses = true;
	StructureViewArgs.bShowInterfaces = true;

	TSharedRef<FStructOnScope> StructData = MakeShared<FStructOnScope>(
		FRPGTriggerState::StaticStruct(),
		reinterpret_cast<uint8*>(&TriggerData->States[StateIndex])
	);

	TSharedRef<IStructureDetailsView> StructureDetailsView =
		PropertyEditorModule.CreateStructureDetailView(
			DetailsViewArgs,
			StructureViewArgs,
			StructData
		);

	return StructureDetailsView->GetWidget().ToSharedRef();
}