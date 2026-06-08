#include "Editor/SRPGDatabaseEditor.h"

#include "GraphEditor.h"
#include "PropertyEditorModule.h"
#include "IStructureDetailsView.h"
#include "Editor/RPGDataEditorWrappers.h"

#include "Engine/DataAsset.h"
#include "Modules/ModuleManager.h"
#include "UObject/StructOnScope.h"

#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSplitter.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Views/SListView.h"

#include "IDetailsView.h"

#include "UObject/Class.h"

const FName SRPGDatabaseEditor::CharactersTabId(TEXT("RPGDatabaseEditor_Characters"));
const FName SRPGDatabaseEditor::ClassesTabId(TEXT("RPGDatabaseEditor_Classes"));
const FName SRPGDatabaseEditor::SkillsTabId(TEXT("RPGDatabaseEditor_Skills"));
const FName SRPGDatabaseEditor::ItemsTabId(TEXT("RPGDatabaseEditor_Items"));
const FName SRPGDatabaseEditor::WeaponsTabId(TEXT("RPGDatabaseEditor_Weapons"));
const FName SRPGDatabaseEditor::ArmorsTabId(TEXT("RPGDatabaseEditor_Armors"));
const FName SRPGDatabaseEditor::MonstersTabId(TEXT("RPGDatabaseEditor_Monsters"));

void SRPGDatabaseEditor::Construct(const FArguments& InArgs, UPrimaryDataAsset* InDatabaseAsset, FName InTabId)
{
	DatabaseAsset = InDatabaseAsset;
    TabId = InTabId;
	DataEditorObject = CreateDataEditor();

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bAllowSearch = true;
	DetailsViewArgs.bHideSelectionTip = true;

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObject(nullptr);

	DetailsView->OnFinishedChangingProperties().AddSP(this, &SRPGDatabaseEditor::OnDoneEditing);

	ChildSlot
	[
		SNew(SSplitter)
			.Orientation(Orient_Horizontal)
			+ SSplitter::Slot()
			.Value(0.25f)
			[
				SNew(SBorder)
					.Padding(4.0f)
					[
						CreateDataListView()
					]
			]
		+ SSplitter::Slot()
			.Value(0.75f)
			[
				SNew(SBorder)
					.Padding(4.0f)
					[
						DetailsView.ToSharedRef()
					]
			]
	];

	SelectData(0);
}

TObjectPtr<URPGDataEditorObject> SRPGDatabaseEditor::CreateDataEditor()
{
	if (TabId == CharactersTabId)
	{
		return NewObject<URPGCharacterDataEditorObject>();
	}
	else if (TabId == ClassesTabId)
	{
		return NewObject<URPGClassDataEditorObject>();
	}
	else if (TabId == SkillsTabId)
	{
		return NewObject<URPGSkillDataEditorObject>();
	}
	else if (TabId == ItemsTabId)
	{
		return NewObject<URPGItemDataEditorObject>();
	}
	else if (TabId == WeaponsTabId)
	{
		return NewObject<URPGWeaponDataEditorObject>();
	}
	else if (TabId == ArmorsTabId)
	{
		return NewObject<URPGArmorDataEditorObject>();
	}
	else if (TabId == MonstersTabId)
	{
		return NewObject<URPGMonsterDataEditorObject>();
	}

	return nullptr;
}

void SRPGDatabaseEditor::OnDoneEditing(const FPropertyChangedEvent& Event)
{
	DatabaseAsset->Modify();

	if (TabId == CharactersTabId)
	{
		if (URPGCharacters* CharactersData = Cast<URPGCharacters>(DatabaseAsset))
		{
			if (URPGCharacterDataEditorObject* Editor = Cast<URPGCharacterDataEditorObject>(DataEditorObject))
			{
				CharactersData->Characters[SelectedData] = Editor->Data;
			}
		}
	}
	else if (TabId == ClassesTabId)
	{
		if (URPGClasses* ClassesData = Cast<URPGClasses>(DatabaseAsset))
		{
			if (URPGClassDataEditorObject* Editor = Cast<URPGClassDataEditorObject>(DataEditorObject))
			{
				ClassesData->Classes[SelectedData] = Editor->Data;
			}
		}
	}
	else if (TabId == SkillsTabId)
	{
		if (URPGSkills* SkillsData = Cast<URPGSkills>(DatabaseAsset))
		{
			if (URPGSkillDataEditorObject* Editor = Cast<URPGSkillDataEditorObject>(DataEditorObject))
			{
				SkillsData->Skills[SelectedData] = Editor->Data;
			}
		}
	}
	else if (TabId == ItemsTabId)
	{
		if (URPGItems* ItemsData = Cast<URPGItems>(DatabaseAsset))
		{
			if (URPGItemDataEditorObject* Editor = Cast<URPGItemDataEditorObject>(DataEditorObject))
			{
				ItemsData->Items[SelectedData] = Editor->Data;
			}
		}
	}
	else if (TabId == WeaponsTabId)
	{
		if (URPGWeapons* WeaponsData = Cast<URPGWeapons>(DatabaseAsset))
		{
			if (URPGWeaponDataEditorObject* Editor = Cast<URPGWeaponDataEditorObject>(DataEditorObject))
			{
				WeaponsData->Weapons[SelectedData] = Editor->Data;
			}
		}
	}
	else if (TabId == ArmorsTabId)
	{
		if (URPGArmors* ArmorsData = Cast<URPGArmors>(DatabaseAsset))
		{
			if (URPGArmorDataEditorObject* Editor = Cast<URPGArmorDataEditorObject>(DataEditorObject))
			{
				ArmorsData->Armors[SelectedData] = Editor->Data;
			}
		}
	}
	else if (TabId == MonstersTabId)
	{
		if (URPGMonsters* MonstersData = Cast<URPGMonsters>(DatabaseAsset))
		{
			if (URPGMonsterDataEditorObject* Editor = Cast<URPGMonsterDataEditorObject>(DataEditorObject))
			{
				MonstersData->Monsters[SelectedData] = Editor->Data;
			}
		}
	}

	DatabaseAsset->MarkPackageDirty();
}

TSharedRef<SWidget> SRPGDatabaseEditor::CreateDataListView()
{
	TArray<FString> List = GetDataList();
	for (const FString& Item : List)
	{
		DataList.Add(MakeShared<FString>(Item));
	}

	SAssignNew(DataListView, SListView<TSharedPtr<FString>>)
		.ListItemsSource(&DataList)
		.OnGenerateRow(this, &SRPGDatabaseEditor::GenerateDataRow)
		.OnSelectionChanged(this, &SRPGDatabaseEditor::OnDataSelected);

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Center)
		[
			SNew(STextBlock)
				.Text(GetListLabel())
				.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 20))
		]

		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(16.0f, 4.0f)
		[
			DataListView.ToSharedRef()
		]

		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SButton)
				.Text(GetAddButtonLabel())
				.OnClicked(this, &SRPGDatabaseEditor::AddNewData)
		];
}

void SRPGDatabaseEditor::SelectData(int32 DataIndex)
{
	SelectedData = DataIndex;

	if (DataEditorObject)
	{
		DataEditorObject->Modify();

		if (TabId == CharactersTabId)
		{
			SelectCharacter(DataIndex);
		}
		else if (TabId == ClassesTabId)
		{
			SelectClass(DataIndex);
		}
		else if (TabId == SkillsTabId)
		{
			SelectSkill(DataIndex);
		}
		else if (TabId == ItemsTabId)
		{
			SelectItem(DataIndex);
		}
		else if (TabId == WeaponsTabId)
		{
			SelectWeapon(DataIndex);
		}
		else if (TabId == ArmorsTabId)
		{
			SelectArmor(DataIndex);
		}
		else if (TabId == MonstersTabId)
		{
			SelectMonster(DataIndex);
		}

		DetailsView->SetObject(DataEditorObject);
	}
}

void SRPGDatabaseEditor::SelectCharacter(int32 CharacterIndex)
{
	if (URPGCharacterDataEditorObject* Editor = Cast<URPGCharacterDataEditorObject>(DataEditorObject))
	{
		if (URPGCharacters* CharactersData = Cast<URPGCharacters>(DatabaseAsset))
		{
			Editor->Data = CharactersData->Characters[CharacterIndex];
		}
	}
}

void SRPGDatabaseEditor::SelectClass(int32 ClassIndex)
{
	if (URPGClassDataEditorObject* Editor = Cast<URPGClassDataEditorObject>(DataEditorObject))
	{
		if (URPGClasses* ClassesData = Cast<URPGClasses>(DatabaseAsset))
		{
			Editor->Data = ClassesData->Classes[ClassIndex];
		}
	}
}

void SRPGDatabaseEditor::SelectSkill(int32 SkillIndex)
{
	if (URPGSkillDataEditorObject* Editor = Cast<URPGSkillDataEditorObject>(DataEditorObject))
	{
		if (URPGSkills* SkillsData = Cast<URPGSkills>(DatabaseAsset))
		{
			Editor->Data = SkillsData->Skills[SkillIndex];
		}
	}
}

void SRPGDatabaseEditor::SelectItem(int32 ItemIndex)
{
	if (URPGItemDataEditorObject* Editor = Cast<URPGItemDataEditorObject>(DataEditorObject))
	{
		if (URPGItems* ItemsData = Cast<URPGItems>(DatabaseAsset))
		{
			Editor->Data = ItemsData->Items[ItemIndex];
		}
	}
}

void SRPGDatabaseEditor::SelectWeapon(int32 WeaponIndex)
{
	if (URPGWeaponDataEditorObject* Editor = Cast<URPGWeaponDataEditorObject>(DataEditorObject))
	{
		if (URPGWeapons* Weapons = Cast<URPGWeapons>(DatabaseAsset))
		{
			Editor->Data = Weapons->Weapons[WeaponIndex];
		}
	}
}

void SRPGDatabaseEditor::SelectArmor(int32 ArmorIndex)
{
	if (URPGArmorDataEditorObject* Editor = Cast<URPGArmorDataEditorObject>(DataEditorObject))
	{
		if (URPGArmors* Armors = Cast<URPGArmors>(DatabaseAsset))
		{
			Editor->Data = Armors->Armors[ArmorIndex];
		}
	}
}

void SRPGDatabaseEditor::SelectMonster(int32 MonsterIndex)
{
	if (URPGMonsterDataEditorObject* Editor = Cast<URPGMonsterDataEditorObject>(DataEditorObject))
	{
		if (URPGMonsters* Monsters = Cast<URPGMonsters>(DatabaseAsset))
		{
			if (Monsters->Monsters.IsValidIndex(MonsterIndex))
			{
				Editor->Data = Monsters->Monsters[MonsterIndex];
			}
		}
	}
}

TArray<FString> SRPGDatabaseEditor::GetDataList() const
{
	if (TabId == CharactersTabId)
	{
		if (URPGCharacters* CharactersData = Cast<URPGCharacters>(DatabaseAsset))
		{
			return CharactersData->GetCharacterNames();
		}
	}
	else if (TabId == ClassesTabId)
	{
		if (URPGClasses* ClassesData = Cast<URPGClasses>(DatabaseAsset))
		{
			return ClassesData->GetClassNames();
		}
	}
	else if (TabId == SkillsTabId)
	{
		if (URPGSkills* SkillsData = Cast<URPGSkills>(DatabaseAsset))
		{
			return SkillsData->GetSkillNames();
		}
	}
	else if (TabId == ItemsTabId)
	{
		if (URPGItems* ItemsData = Cast<URPGItems>(DatabaseAsset))
		{
			return ItemsData->GetItemNames();
		}
	}
	else if (TabId == WeaponsTabId)
	{
		if (URPGWeapons* WeaponsData = Cast<URPGWeapons>(DatabaseAsset))
		{
			return WeaponsData->GetWeaponNames();
		}
	}
	else if (TabId == ArmorsTabId)
	{
		if (URPGArmors* ArmorsData = Cast<URPGArmors>(DatabaseAsset))
		{
			return ArmorsData->GetArmorNames();
		}
	}
	else if (TabId == MonstersTabId)
	{
		if (URPGMonsters* MonstersData = Cast<URPGMonsters>(DatabaseAsset))
		{
			return MonstersData->GetMonsterNames();
		}
	}
	
	return TArray<FString>();	
}



TSharedRef<ITableRow> SRPGDatabaseEditor::GenerateDataRow(TSharedPtr<FString> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FString>>, OwnerTable)
		[
			SNew(STextBlock)
				.Text(FText::FromString(*Item))
		];
}

void SRPGDatabaseEditor::OnDataSelected(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo)
{
	if (!Item.IsValid())
	{
		return;
	}

	const int32 Index = DataList.IndexOfByKey(Item);

	SelectData(Index);
}

FReply SRPGDatabaseEditor::AddNewData()
{
	TSharedPtr<FString> NewItem;

	if (TabId == CharactersTabId)
	{
		if (URPGCharacters* CharactersData = Cast<URPGCharacters>(DatabaseAsset))
		{
			int32 NewCharacter = CharactersData->AddCharacter();
			CharactersData->Modify();
			CharactersData->MarkPackageDirty();

			NewItem = MakeShared<FString>(TEXT("New Character"));
			SelectCharacter(NewCharacter);
		}
	}
	else if (TabId == ClassesTabId)
	{
		if (URPGClasses* ClassesData = Cast<URPGClasses>(DatabaseAsset))
		{
			ClassesData->Classes.AddDefaulted();
			ClassesData->Modify();
			ClassesData->MarkPackageDirty();

			NewItem = MakeShared<FString>(TEXT("New Class"));
		}
	}
	else if (TabId == SkillsTabId)
	{
		if (URPGSkills* SkillsData = Cast<URPGSkills>(DatabaseAsset))
		{
			SkillsData->Skills.AddDefaulted();
			SkillsData->Modify();
			SkillsData->MarkPackageDirty();

			NewItem = MakeShared<FString>(TEXT("New Skill"));
		}
	}
	else if (TabId == ItemsTabId)
	{
		if (URPGItems* ItemsData = Cast<URPGItems>(DatabaseAsset))
		{
			ItemsData->Items.AddDefaulted();
			ItemsData->Modify();
			ItemsData->MarkPackageDirty();

			NewItem = MakeShared<FString>(TEXT("New Item"));
		}
	}
	else if (TabId == WeaponsTabId)
	{
		if (URPGWeapons* WeaponsData = Cast<URPGWeapons>(DatabaseAsset))
		{
			WeaponsData->Weapons.AddDefaulted();
			WeaponsData->Modify();
			WeaponsData->MarkPackageDirty();

			NewItem = MakeShared<FString>(TEXT("New Weapon"));
		}
	}
	else if (TabId == ArmorsTabId)
	{
		if (URPGArmors* ArmorsData = Cast<URPGArmors>(DatabaseAsset))
		{
			ArmorsData->Armors.AddDefaulted();
			ArmorsData->Modify();
			ArmorsData->MarkPackageDirty();

			NewItem = MakeShared<FString>(TEXT("New Armor"));
		}
	}
	else if (TabId == MonstersTabId)
	{
		if (URPGMonsters* MonstersData = Cast<URPGMonsters>(DatabaseAsset))
		{
			MonstersData->Monsters.AddDefaulted();
			MonstersData->Modify();
			MonstersData->MarkPackageDirty();

			NewItem = MakeShared<FString>(TEXT("New Monster"));
		}
	}

	if (NewItem.IsValid() && DataListView.IsValid())
	{
		DataListView->RequestListRefresh();
		DataListView->SetSelection(NewItem);

		DataListView->RequestScrollIntoView(NewItem);
	}

	return FReply::Handled();
}

FText SRPGDatabaseEditor::GetListLabel() const
{
	if (TabId == CharactersTabId)
	{
		return FText::FromString(TEXT("CHARACTERS"));
	}
	else if (TabId == ClassesTabId)
	{
		return FText::FromString(TEXT("CLASSES"));
	}
	else if (TabId == SkillsTabId)
	{
		return FText::FromString(TEXT("SKILLS"));
	}
	else if (TabId == ItemsTabId)
	{
		return FText::FromString(TEXT("ITEMS"));
	}
	else if (TabId == WeaponsTabId)
	{
		return FText::FromString(TEXT("WEAPONS"));
	}
	else if (TabId == ArmorsTabId)
	{
		return FText::FromString(TEXT("ARMORS"));
	}
	else if (TabId == MonstersTabId)
	{
		return FText::FromString(TEXT("MONSTERS"));
	}

	return FText::GetEmpty();
}

FText SRPGDatabaseEditor::GetAddButtonLabel() const
{
	if (TabId == CharactersTabId)
	{
		return FText::FromString(TEXT("Add Character"));
	}
	else if (TabId == ClassesTabId)
	{
		return FText::FromString(TEXT("Add Class"));
	}
	else if (TabId == SkillsTabId)
	{
		return FText::FromString(TEXT("Add Skill"));
	}
	else if (TabId == ItemsTabId)
	{
		return FText::FromString(TEXT("Add Item"));
	}
	else if (TabId == WeaponsTabId)
	{
		return FText::FromString(TEXT("Add Weapon"));
	}
	else if (TabId == ArmorsTabId)
	{
		return FText::FromString(TEXT("Add Armor"));
	}
	else if (TabId == MonstersTabId)
	{
		return FText::FromString(TEXT("Add Monster"));
	}
	return FText::FromString(TEXT("Add Data"));
}

//Slate drawings
TSharedRef<STextBlock> SRPGDatabaseEditor::SectionLabel(FText Label)
{
	return
		SNew(STextBlock)
		.Text(Label)
		.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 16));
}

TSharedRef<SWidget> SRPGDatabaseEditor::TextField(const FText& Label, TFunction<FText()> Getter, TFunction<void(const FText&)> Setter)
{
	return SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.FillWidth(0.3f)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
				.Text(Label)
		]

		+ SHorizontalBox::Slot()
		.FillWidth(0.7f)
		[
			SNew(SEditableTextBox)
				.Text_Lambda([Getter]() { return Getter(); })
				.OnTextCommitted_Lambda([Setter](const FText& NewText, ETextCommit::Type)
					{
						Setter(NewText);
					})
		];
}

TSharedRef<SWidget> SRPGDatabaseEditor::OptionsDropdown(TArray<TSharedPtr<FName>>* Options, const FText& Label, TFunction<FName()> Getter, TFunction<void(const FName&)> Setter)
{
	return SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.FillWidth(0.3f)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
				.Text(Label)
		]

		+ SHorizontalBox::Slot()
		.FillWidth(0.7f)
		[
			SNew(SComboBox<TSharedPtr<FName>>)
				.OptionsSource(Options)
				.OnGenerateWidget_Lambda([](TSharedPtr<FName> Option)
					{
						FText ItemText = Option.IsValid() ? FText::FromName(*Option) : FText::GetEmpty();
						return SNew(STextBlock).Text(ItemText);
					})
				.OnSelectionChanged_Lambda([Setter](TSharedPtr<FName> NewSelection, ESelectInfo::Type)
					{
						if (NewSelection.IsValid())
						{
							Setter(*NewSelection);
						}
						else
						{
							Setter(*NewSelection);
						}
					})
				[
					SNew(STextBlock)
						.Text_Lambda([Getter]()
							{
								FName Value = Getter();
								return Value.IsValid() ? FText::FromName(Value) : FText::FromString(TEXT("Select option..."));
							})
				]
		];
}

TSharedRef<SWidget> SRPGDatabaseEditor::IntField(const FText& Label, TFunction<int32()> Getter, TFunction<void(int32)> Setter)
{
	return SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.FillWidth(0.3f)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
				.Text(Label)
		]

		+ SHorizontalBox::Slot()
		.FillWidth(0.7f)
		[
			SNew(SNumericEntryBox<int32>)
				.Value_Lambda([Getter]() { return Getter(); })
				.OnValueChanged_Lambda([Setter](int32 NewValue)
					{
						Setter(NewValue);
					})
		];
}

TSharedRef<SWidget> SRPGDatabaseEditor::MultiLineTextField(const FText& Label, TFunction<FText()> Getter, TFunction<void(const FText&)> Setter)
{
	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.FillWidth(0.3f)
		.VAlign(VAlign_Top)
		[

			SNew(STextBlock)
				.Text(Label)
		]

		+ SHorizontalBox::Slot()
		.FillWidth(0.7f)
		[
			SNew(SMultiLineEditableTextBox)
				.Text_Lambda([Getter]() { return Getter(); })
				.OnTextCommitted_Lambda([Setter](const FText& NewText, ETextCommit::Type)
					{
						Setter(NewText);
					})
		];
}

