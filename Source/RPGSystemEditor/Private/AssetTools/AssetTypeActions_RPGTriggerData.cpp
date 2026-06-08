#include "AssetTools/AssetTypeActions_RPGTriggerData.h"

#include "Data/RPGTriggerData.h"
#include "Editor/RPGTriggerEditor.h"

FText FAssetTypeActions_RPGTriggerData::GetName() const
{
	return FText::FromString(TEXT("RPG Trigger Data"));
}

FColor FAssetTypeActions_RPGTriggerData::GetTypeColor() const
{
	return FColor(80, 140, 255);
}

UClass* FAssetTypeActions_RPGTriggerData::GetSupportedClass() const
{
	return URPGTriggerData::StaticClass();
}

uint32 FAssetTypeActions_RPGTriggerData::GetCategories()
{
	return EAssetTypeCategories::Gameplay;
}

void FAssetTypeActions_RPGTriggerData::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	for (UObject* Object : InObjects)
	{
		URPGTriggerData* TriggerData = Cast<URPGTriggerData>(Object);
		if (!TriggerData)
		{
			continue;
		}

		TSharedRef<FRPGTriggerEditor> Editor = MakeShared<FRPGTriggerEditor>();

		Editor->InitRPGTriggerEditor(EToolkitMode::Standalone, EditWithinLevelEditor, TriggerData);
	}
}