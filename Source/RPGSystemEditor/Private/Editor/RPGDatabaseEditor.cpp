#include "Editor/RPGDatabaseEditor.h"

#include "Data/RPGArmors.h"
#include "Data/RPGCharacters.h"
#include "Data/RPGClasses.h"
#include "Data/RPGDatabase.h"
#include "Data/RPGItems.h"
#include "Data/RPGMonsters.h"
#include "Data/RPGSkills.h"
#include "Data/RPGWeapons.h"

#include "SRPGDatabaseEditor.h"
#include "SCharacterDataEditor.h"

#include "PropertyCustomizationHelpers.h"
#include "Widgets/Docking/SDockTab.h"

void FRPGDatabaseEditor::InitRPGDatabaseEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, URPGDatabase* InDatabaseData)
{
	DatabaseData = InDatabaseData;

	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("RPGDatabaseEditor_Layout_v1")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Horizontal)
			->Split
			(
				FTabManager::NewStack()
					->AddTab(SRPGDatabaseEditor::CharactersTabId, ETabState::OpenedTab)
					->AddTab(SRPGDatabaseEditor::ClassesTabId, ETabState::OpenedTab)
					->AddTab(SRPGDatabaseEditor::SkillsTabId, ETabState::OpenedTab)
					->AddTab(SRPGDatabaseEditor::ItemsTabId, ETabState::OpenedTab)
					->AddTab(SRPGDatabaseEditor::WeaponsTabId, ETabState::OpenedTab)
					->AddTab(SRPGDatabaseEditor::ArmorsTabId, ETabState::OpenedTab)
					->AddTab(SRPGDatabaseEditor::MonstersTabId, ETabState::OpenedTab)
					->SetForegroundTab(SRPGDatabaseEditor::CharactersTabId)
					->SetHideTabWell(true)
			)
		);		

	InitAssetEditor(Mode, InitToolkitHost, GetToolkitFName(), Layout, true, true, InDatabaseData);

	RegenerateMenusAndToolbars();
}

FName FRPGDatabaseEditor::GetToolkitFName() const
{
	return FName(TEXT("RPGDatabaseEditor"));
}

FText FRPGDatabaseEditor::GetBaseToolkitName() const
{
	return FText::FromString(TEXT("RPG Database Editor"));
}

FString FRPGDatabaseEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("RPG Database");
}

FLinearColor FRPGDatabaseEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.1f, 0.2f, 0.4f, 0.5f);
}

FText FRPGDatabaseEditor::GetTabName(FTabId TabId) const
{
	if (TabId == SRPGDatabaseEditor::CharactersTabId)
	{
		return FText::FromString(TEXT("Characters"));
	}
	else if (TabId == SRPGDatabaseEditor::ClassesTabId)
	{
		return FText::FromString(TEXT("Classes"));
	}
	else if (TabId == SRPGDatabaseEditor::SkillsTabId)
	{
		return FText::FromString(TEXT("Skills"));
	}
	else if (TabId == SRPGDatabaseEditor::ItemsTabId)
	{
		return FText::FromString(TEXT("Items"));
	}
	else if (TabId == SRPGDatabaseEditor::WeaponsTabId)
	{
		return FText::FromString(TEXT("Weapons"));
	}
	else if (TabId == SRPGDatabaseEditor::ArmorsTabId)
	{
		return FText::FromString(TEXT("Armors"));
	}
	else if (TabId == SRPGDatabaseEditor::MonstersTabId)
	{
		return FText::FromString(TEXT("Monsters"));
	}
    return FText::FromString(TEXT("Database"));
}

UClass* FRPGDatabaseEditor::GetClassForTab(FTabId TabId) const
{
	if (TabId == SRPGDatabaseEditor::CharactersTabId)
	{
		return URPGCharacters::StaticClass();
	}
	else if (TabId == SRPGDatabaseEditor::ClassesTabId)
	{
		return URPGClasses::StaticClass();
	}
	else if (TabId == SRPGDatabaseEditor::SkillsTabId)
	{
		return URPGSkills::StaticClass();
	}
	else if (TabId == SRPGDatabaseEditor::ItemsTabId)
	{
		return URPGItems::StaticClass();
	}
	else if (TabId == SRPGDatabaseEditor::WeaponsTabId)
	{
		return URPGWeapons::StaticClass();
	}
	else if (TabId == SRPGDatabaseEditor::ArmorsTabId)
	{
		return URPGArmors::StaticClass();
	}
	else if (TabId == SRPGDatabaseEditor::MonstersTabId)
	{
		return URPGMonsters::StaticClass();
	}
    return nullptr;
}

FString FRPGDatabaseEditor::GetDatabaseObjectPath(FTabId TabId) const
{
	if (!DatabaseData)
	{
        return FString();
	}

	if (TabId == SRPGDatabaseEditor::CharactersTabId)
	{
		if (URPGCharacters* Characters = DatabaseData->CharactersDatabase.Get())
		{
			return Characters->GetPathName();
		}
    }
	else if (TabId == SRPGDatabaseEditor::ClassesTabId)
	{
		if (URPGClasses* Classes = DatabaseData->ClassesDatabase.Get())
		{
			return Classes->GetPathName();
        }
	}
	else if (TabId == SRPGDatabaseEditor::SkillsTabId)
	{
		if (URPGSkills* Skills = DatabaseData->SkillsDatabase.Get())
		{
			return Skills->GetPathName();
		}
	}
	else if (TabId == SRPGDatabaseEditor::ItemsTabId)
	{
		if (URPGItems* Items = DatabaseData->ItemsDatabase.Get())
		{
			return Items->GetPathName();
		}
	}
	else if (TabId == SRPGDatabaseEditor::WeaponsTabId)
	{
		if (URPGWeapons* Weapons = DatabaseData->WeaponsDatabase.Get())
		{
			return Weapons->GetPathName();
		}
	}
	else if (TabId == SRPGDatabaseEditor::ArmorsTabId)
	{
		if (URPGArmors* Armors = DatabaseData->ArmorsDatabase.Get())
		{
			return Armors->GetPathName();
		}
	}
	else if (TabId == SRPGDatabaseEditor::MonstersTabId)
	{
		if (URPGMonsters* Monsters = DatabaseData->MonstersDatabase.Get())
		{
			return Monsters->GetPathName();
		}
    }
	return FString();
}

void FRPGDatabaseEditor::SetDatabaseObject(FTabId TabId, const FAssetData& AssetData)
{
	DatabaseData->Modify();

	if (TabId == SRPGDatabaseEditor::CharactersTabId)
	{
		DatabaseData->CharactersDatabase = TSoftObjectPtr<URPGCharacters>(AssetData.ToSoftObjectPath());			

		if (URPGCharacters* Characters = DatabaseData->CharactersDatabase.LoadSynchronous())
		{
			if (CharactersDockTab.IsValid())
			{
				CharactersDockTab->SetContent(
					GetDatabaseTab(TabId)
				);
			}
		}

	}
	else if (TabId == SRPGDatabaseEditor::ClassesTabId)
	{
		DatabaseData->ClassesDatabase = TSoftObjectPtr<URPGClasses>(AssetData.ToSoftObjectPath());

		if (URPGClasses* Classes = DatabaseData->ClassesDatabase.LoadSynchronous())
		{
			if (ClassesDockTab.IsValid())
			{
				ClassesDockTab->SetContent(
					GetDatabaseTab(TabId)
				);
			}
		}
	}
	else if (TabId == SRPGDatabaseEditor::SkillsTabId)
	{
		DatabaseData->SkillsDatabase = TSoftObjectPtr<URPGSkills>(AssetData.ToSoftObjectPath());

		if (URPGSkills* Skills = DatabaseData->SkillsDatabase.LoadSynchronous())
		{
			if (SkillsDockTab.IsValid())
			{
				SkillsDockTab->SetContent(
					GetDatabaseTab(TabId)
				);
			}
        }
	}
	else if (TabId == SRPGDatabaseEditor::ItemsTabId)
	{
		DatabaseData->ItemsDatabase = TSoftObjectPtr<URPGItems>(AssetData.ToSoftObjectPath());

		if (URPGItems* Items = DatabaseData->ItemsDatabase.LoadSynchronous())
		{
			if (ItemsDockTab.IsValid())
			{
				ItemsDockTab->SetContent(
					GetDatabaseTab(TabId)
				);
			}
        }
	}
	else if (TabId == SRPGDatabaseEditor::WeaponsTabId)
	{
		DatabaseData->WeaponsDatabase = TSoftObjectPtr<URPGWeapons>(AssetData.ToSoftObjectPath());

		if (URPGWeapons* Weapons = DatabaseData->WeaponsDatabase.LoadSynchronous())
		{
			if (WeaponsDockTab.IsValid())
			{
				WeaponsDockTab->SetContent(
					GetDatabaseTab(TabId)
				);
			}
        }
	}
	else if (TabId == SRPGDatabaseEditor::ArmorsTabId)
	{
		DatabaseData->ArmorsDatabase = TSoftObjectPtr<URPGArmors>(AssetData.ToSoftObjectPath());

		if (URPGArmors* Armors = DatabaseData->ArmorsDatabase.LoadSynchronous())
		{
			if (ArmorsDockTab.IsValid())
			{
				ArmorsDockTab->SetContent(
					GetDatabaseTab(TabId)
				);
			}
        }
	}
	else if (TabId == SRPGDatabaseEditor::MonstersTabId)
	{
		DatabaseData->MonstersDatabase = TSoftObjectPtr<URPGMonsters>(AssetData.ToSoftObjectPath());

		if (URPGMonsters* Monsters = DatabaseData->MonstersDatabase.LoadSynchronous())
		{
			if (MonstersDockTab.IsValid())
			{
				MonstersDockTab->SetContent(
					GetDatabaseTab(TabId)
				);
            }
		}
    }

	DatabaseData->MarkPackageDirty();
}

TSharedRef<SWidget> FRPGDatabaseEditor::CreateMissingDatabase(FTabId TabId)
{
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("No Database assigned. Please select an asset to edit.")))
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5)
		[
			SNew(SObjectPropertyEntryBox)
			.AllowedClass(GetClassForTab(TabId))
			.ObjectPath_Lambda([this, TabId]() { return GetDatabaseObjectPath(TabId); })
			.OnObjectChanged_Lambda([this, TabId](const FAssetData& AssetData) { SetDatabaseObject(TabId, AssetData); })
		];
}

TSharedRef<SWidget> FRPGDatabaseEditor::GetDatabaseTab(FTabId TabId)
{
	if (TabId == SRPGDatabaseEditor::CharactersTabId)
	{
		if (!DatabaseData->CharactersDatabase)
		{
			return CreateMissingDatabase(TabId);
		}

		return SNew(SCharacterDataEditor, DatabaseData);
	}
	else if (TabId == SRPGDatabaseEditor::ClassesTabId)
	{
		if (!DatabaseData->ClassesDatabase.LoadSynchronous())
		{
			return CreateMissingDatabase(TabId);
		}

		return SNew(SRPGDatabaseEditor, DatabaseData->ClassesDatabase.Get(), SRPGDatabaseEditor::ClassesTabId);
	}
	else if (TabId == SRPGDatabaseEditor::SkillsTabId)
	{
		if (!DatabaseData->SkillsDatabase.LoadSynchronous())
		{
			return CreateMissingDatabase(TabId);
		}
		return SNew(SRPGDatabaseEditor, DatabaseData->SkillsDatabase.Get(), SRPGDatabaseEditor::SkillsTabId);
	}
	else if (TabId == SRPGDatabaseEditor::ItemsTabId)
	{
		if (!DatabaseData->ItemsDatabase.LoadSynchronous())
		{
			return CreateMissingDatabase(TabId);
		}
		return SNew(SRPGDatabaseEditor, DatabaseData->ItemsDatabase.Get(), SRPGDatabaseEditor::ItemsTabId);
	}
	else if (TabId == SRPGDatabaseEditor::WeaponsTabId)
	{
		if (!DatabaseData->WeaponsDatabase.LoadSynchronous())
		{
			return CreateMissingDatabase(TabId);
		}
		return SNew(SRPGDatabaseEditor, DatabaseData->WeaponsDatabase.Get(), SRPGDatabaseEditor::WeaponsTabId);
	}
	else if (TabId == SRPGDatabaseEditor::ArmorsTabId)
	{
		if (!DatabaseData->ArmorsDatabase.LoadSynchronous())
		{
			return CreateMissingDatabase(TabId);
		}
		return SNew(SRPGDatabaseEditor, DatabaseData->ArmorsDatabase.Get(), SRPGDatabaseEditor::ArmorsTabId);
	}
	else if (TabId == SRPGDatabaseEditor::MonstersTabId)
	{
		if (!DatabaseData->MonstersDatabase.LoadSynchronous())
		{
			return CreateMissingDatabase(TabId);
		}
		return SNew(SRPGDatabaseEditor, DatabaseData->MonstersDatabase.Get(), SRPGDatabaseEditor::MonstersTabId);
    }

	return SNew(STextBlock)
		.Text(FText::FromString(TEXT("Select a tab to edit the corresponding database.")));
}

TSharedRef<SDockTab> FRPGDatabaseEditor::SpawnDatabaseTabs(const FSpawnTabArgs& Args)
{
	if (Args.GetTabId() == SRPGDatabaseEditor::CharactersTabId)
	{
		CharactersDockTab = SNew(SDockTab)
			.TabRole(ETabRole::PanelTab)
			.OnCanCloseTab_Lambda([]() { return false; })
			.Label(GetTabName(Args.GetTabId()))
			[
				GetDatabaseTab(Args.GetTabId())
			];

		return CharactersDockTab.ToSharedRef();
	}
	else if (Args.GetTabId() == SRPGDatabaseEditor::ClassesTabId)
	{
		ClassesDockTab = SNew(SDockTab)
			.TabRole(ETabRole::PanelTab)
			.OnCanCloseTab_Lambda([]() { return false; })
			.Label(GetTabName(Args.GetTabId()))
			[
				GetDatabaseTab(Args.GetTabId())
			];

		return ClassesDockTab.ToSharedRef();
	}
	else if (Args.GetTabId() == SRPGDatabaseEditor::SkillsTabId)
	{
		SkillsDockTab = SNew(SDockTab)
			.TabRole(ETabRole::PanelTab)
			.OnCanCloseTab_Lambda([]() { return false; })
			.Label(GetTabName(Args.GetTabId()))
			[
				GetDatabaseTab(Args.GetTabId())
			];

		return SkillsDockTab.ToSharedRef();
	}
	else if (Args.GetTabId() == SRPGDatabaseEditor::ItemsTabId)
	{
		ItemsDockTab = SNew(SDockTab)
			.TabRole(ETabRole::PanelTab)
			.OnCanCloseTab_Lambda([]() { return false; })
			.Label(GetTabName(Args.GetTabId()))
			[
				GetDatabaseTab(Args.GetTabId())
			];

		return ItemsDockTab.ToSharedRef();
	}
	else if (Args.GetTabId() == SRPGDatabaseEditor::WeaponsTabId)
	{
		WeaponsDockTab = SNew(SDockTab)
			.TabRole(ETabRole::PanelTab)
			.OnCanCloseTab_Lambda([]() { return false; })
			.Label(GetTabName(Args.GetTabId()))
			[
				GetDatabaseTab(Args.GetTabId())
			];

		return WeaponsDockTab.ToSharedRef();
	}
	else if (Args.GetTabId() == SRPGDatabaseEditor::ArmorsTabId)
	{
		ArmorsDockTab = SNew(SDockTab)
			.TabRole(ETabRole::PanelTab)
			.OnCanCloseTab_Lambda([]() { return false; })
			.Label(GetTabName(Args.GetTabId()))
			[
				GetDatabaseTab(Args.GetTabId())
			];

		return ArmorsDockTab.ToSharedRef();
	}
	else if (Args.GetTabId() == SRPGDatabaseEditor::MonstersTabId)
	{
		MonstersDockTab = SNew(SDockTab)
			.TabRole(ETabRole::PanelTab)
			.OnCanCloseTab_Lambda([]() { return false; })
			.Label(GetTabName(Args.GetTabId()))
			[
				GetDatabaseTab(Args.GetTabId())
			];

		return MonstersDockTab.ToSharedRef();
	}

	return SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		.Label(GetTabName(Args.GetTabId()))
		[
			SNew(STextBlock)
				.Text(FText::FromString(TEXT("Select a tab to edit the corresponding database.")))
		];
}

void FRPGDatabaseEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(SRPGDatabaseEditor::CharactersTabId, FOnSpawnTab::CreateRaw(this, &FRPGDatabaseEditor::SpawnDatabaseTabs))
		.SetDisplayName(FText::FromString(TEXT("Characters")))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef());

	InTabManager->RegisterTabSpawner(SRPGDatabaseEditor::ClassesTabId, FOnSpawnTab::CreateRaw(this, &FRPGDatabaseEditor::SpawnDatabaseTabs))
		.SetDisplayName(FText::FromString(TEXT("Classes")))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef());

	InTabManager->RegisterTabSpawner(SRPGDatabaseEditor::SkillsTabId, FOnSpawnTab::CreateRaw(this, &FRPGDatabaseEditor::SpawnDatabaseTabs))
		.SetDisplayName(FText::FromString(TEXT("Skills")))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef());

	InTabManager->RegisterTabSpawner(SRPGDatabaseEditor::ItemsTabId, FOnSpawnTab::CreateRaw(this, &FRPGDatabaseEditor::SpawnDatabaseTabs))
		.SetDisplayName(FText::FromString(TEXT("Items")))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef());

	InTabManager->RegisterTabSpawner(SRPGDatabaseEditor::WeaponsTabId, FOnSpawnTab::CreateRaw(this, &FRPGDatabaseEditor::SpawnDatabaseTabs))
		.SetDisplayName(FText::FromString(TEXT("Weapons")))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef());

	InTabManager->RegisterTabSpawner(SRPGDatabaseEditor::ArmorsTabId, FOnSpawnTab::CreateRaw(this, &FRPGDatabaseEditor::SpawnDatabaseTabs))
		.SetDisplayName(FText::FromString(TEXT("Armors")))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef());

	InTabManager->RegisterTabSpawner(SRPGDatabaseEditor::MonstersTabId, FOnSpawnTab::CreateRaw(this, &FRPGDatabaseEditor::SpawnDatabaseTabs))
		.SetDisplayName(FText::FromString(TEXT("Monsters")))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef());
}

void FRPGDatabaseEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(SRPGDatabaseEditor::CharactersTabId);
	InTabManager->UnregisterTabSpawner(SRPGDatabaseEditor::ClassesTabId);
}

void FRPGDatabaseEditor::SaveAsset_Execute()
{
	if (DatabaseData)
	{
		DatabaseData->Modify();
		DatabaseData->MarkPackageDirty();

		if (URPGCharacters* Characters = DatabaseData->CharactersDatabase.Get())
		{
			Characters->Modify();
			Characters->MarkPackageDirty();
		}
		
		if (URPGClasses* Classes = DatabaseData->ClassesDatabase.Get())
		{
			Classes->Modify();
			Classes->MarkPackageDirty();
		}
		
		if (URPGSkills* Skills = DatabaseData->SkillsDatabase.Get())
		{
			Skills->Modify();
			Skills->MarkPackageDirty();
		}
		
		if (URPGItems* Items = DatabaseData->ItemsDatabase.Get())
		{
			Items->Modify();
			Items->MarkPackageDirty();
		}
		
		if (URPGWeapons* Weapons = DatabaseData->WeaponsDatabase.Get())
		{
			Weapons->Modify();
			Weapons->MarkPackageDirty();
		}

		if (URPGArmors* Armors = DatabaseData->ArmorsDatabase.Get())
		{
			Armors->Modify();
			Armors->MarkPackageDirty();
		}
		
		if (URPGMonsters* Monsters = DatabaseData->MonstersDatabase.Get())
		{
			Monsters->Modify();
			Monsters->MarkPackageDirty();
		}
	}

	FAssetEditorToolkit::SaveAsset_Execute();
}