#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Layout/SBox.h"

class URPGDatabase;

class RPGSYSTEMEDITOR_API FRPGDatabaseEditor : public FAssetEditorToolkit
{
public:
	void InitRPGDatabaseEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, URPGDatabase* InDatabaseData);

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

	virtual void SaveAsset_Execute() override;

private:
	TObjectPtr<URPGDatabase> DatabaseData;

	TSharedPtr<SDockTab> CharactersDockTab;
	TSharedPtr<SDockTab> ClassesDockTab;
	TSharedPtr<SDockTab> SkillsDockTab;
	TSharedPtr<SDockTab> ItemsDockTab;
	TSharedPtr<SDockTab> WeaponsDockTab;
	TSharedPtr<SDockTab> ArmorsDockTab;
	TSharedPtr<SDockTab> MonstersDockTab;

    FText GetTabName(FTabId TabId) const;
    UClass* GetClassForTab(FTabId TabId) const;
	TSharedRef<SWidget> CreateMissingDatabase(FTabId TabId);
	FString GetDatabaseObjectPath(FTabId TabId) const;
	void SetDatabaseObject(FTabId TabId, const FAssetData& AssetData);

    TSharedRef<SWidget> GetDatabaseTab(FTabId TabId);
	TSharedRef<SDockTab> SpawnDatabaseTabs(const FSpawnTabArgs& Args);
};