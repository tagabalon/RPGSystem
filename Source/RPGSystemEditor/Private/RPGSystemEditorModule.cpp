#include "RPGSystemEditorModule.h"

#include "AssetTools/AssetTypeActions_RPGTriggerData.h"
#include "AssetTools/AssetTypeActions_RPGDatabaseData.h"
#include "Data/RPGCharacters.h"
#include "Details/RPGCharacterDataCustomization.h"

#include "PropertyEditorModule.h"
#include "AssetToolsModule.h"
#include "EdGraphUtilities.h"
#include "Graph/RPGGraphNodeFactory.h"
#include "IAssetTools.h"

#define LOCTEXT_NAMESPACE "FRPGEditorModule"

void FRPGEditorModule::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	RegisterAssetTypeAction(AssetTools, MakeShared<FAssetTypeActions_RPGTriggerData>());
	RegisterAssetTypeAction(AssetTools, MakeShared<FAssetTypeActions_RPGDatabaseData>());

	GraphNodeFactory = MakeShared<FRPGGraphNodeFactory>();
	FEdGraphUtilities::RegisterVisualNodeFactory(GraphNodeFactory);

	/*FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout(FRPGCharacterData::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FRPGCharacterDataCustomization::MakeInstance));

	PropertyModule.NotifyCustomizationModuleChanged();*/
}

void FRPGEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();

		for (const TSharedRef<IAssetTypeActions>& Action : RegisteredAssetTypeActions)
		{
			AssetTools.UnregisterAssetTypeActions(Action);
		}
	}

	RegisteredAssetTypeActions.Empty();

	if (GraphNodeFactory.IsValid())
	{
		FEdGraphUtilities::UnregisterVisualNodeFactory(GraphNodeFactory);
		GraphNodeFactory.Reset();
	}

	/*FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.UnregisterCustomPropertyTypeLayout(FRPGCharacterData::StaticStruct()->GetFName());*/
}

void FRPGEditorModule::RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
	AssetTools.RegisterAssetTypeActions(Action);
	RegisteredAssetTypeActions.Add(Action);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRPGEditorModule, RPGSystemEditor)