#include "Editor/RPGTriggerEditor.h"

#include "Editor/SRPGTriggerGraphEditor.h"
#include "Data/RPGTriggerData.h"

#include "Widgets/Docking/SDockTab.h"

const FName FRPGTriggerEditor::GraphTabId(TEXT("RPGTriggerEditor_Graph"));

void FRPGTriggerEditor::InitRPGTriggerEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, URPGTriggerData* InTriggerData)
{
	TriggerData = InTriggerData;

	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("RPGTriggerEditor_Layout_v1")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->AddTab(GraphTabId, ETabState::OpenedTab)
				->SetHideTabWell(true)
			)
		);

	InitAssetEditor(
		Mode,
		InitToolkitHost,
		FName(TEXT("RPGTriggerEditor")),
		Layout,
		true,
		true,
		InTriggerData
	);

	RegenerateMenusAndToolbars();
}

FName FRPGTriggerEditor::GetToolkitFName() const
{
	return FName(TEXT("RPGTriggerEditor"));
}

FText FRPGTriggerEditor::GetBaseToolkitName() const
{
	return FText::FromString(TEXT("RPG Trigger Editor"));
}

FString FRPGTriggerEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("RPG Trigger");
}

FLinearColor FRPGTriggerEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.1f, 0.2f, 0.4f, 0.5f);
}

TSharedRef<SDockTab> FRPGTriggerEditor::SpawnGraphTab(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == GraphTabId);

	return SNew(SDockTab)
		.Label(FText::FromString(TEXT("Graph")))
		[
			SNew(SRPGTriggerGraphEditor, TriggerData)
		];
}

void FRPGTriggerEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(
		GraphTabId,
		FOnSpawnTab::CreateRaw(this, &FRPGTriggerEditor::SpawnGraphTab)
	)
		.SetDisplayName(FText::FromString(TEXT("Graph")))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef());
}

void FRPGTriggerEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(GraphTabId);
}