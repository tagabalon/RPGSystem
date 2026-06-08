#include "AssetTools/AssetTypeActions_RPGDatabaseData.h"

#include "Data/RPGDatabase.h"
#include "Editor/RPGDatabaseEditor.h"

FText FAssetTypeActions_RPGDatabaseData::GetName() const
{
	return FText::FromString(TEXT("RPG Database"));
}

FColor FAssetTypeActions_RPGDatabaseData::GetTypeColor() const
{
	return FColor(80, 140, 255);
}

UClass* FAssetTypeActions_RPGDatabaseData::GetSupportedClass() const
{
	return URPGDatabase::StaticClass();
}

uint32 FAssetTypeActions_RPGDatabaseData::GetCategories()
{
	return EAssetTypeCategories::Gameplay;
}

void FAssetTypeActions_RPGDatabaseData::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	for (UObject* Object : InObjects)
	{
		URPGDatabase* Database = Cast<URPGDatabase>(Object);
		if (!Database)
		{
			continue;
		}

		TSharedRef<FRPGDatabaseEditor> Editor = MakeShared<FRPGDatabaseEditor>();

		Editor->InitRPGDatabaseEditor(EToolkitMode::Standalone, EditWithinLevelEditor, Database);
	}
}