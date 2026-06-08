#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"

class URPGTriggerData;

class RPGSYSTEMEDITOR_API FRPGTriggerEditor : public FAssetEditorToolkit
{
public:
	void InitRPGTriggerEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, URPGTriggerData* InTriggerData);

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

	virtual void SaveAsset_Execute() override;

private:
	static const FName GraphTabId;

	TObjectPtr<URPGTriggerData> TriggerData;

	TSharedRef<SDockTab> SpawnGraphTab(const FSpawnTabArgs& Args);
};