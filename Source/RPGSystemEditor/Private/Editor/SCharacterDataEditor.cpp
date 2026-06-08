#include "SCharacterDataEditor.h"

#include "Data/RPGDatabase.h"
#include "Actors/RPGFieldCharacter.h"

#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/Input/SComboBox.h"

#include "PropertyCustomizationHelpers.h"

void SCharacterDataEditor::Construct(const FArguments& InArgs, URPGDatabase* InDatabase)
{
	if (!InDatabase)
	{
		return;
	}
	TabId = CharactersTabId;

	CharactersDatabase = InDatabase->CharactersDatabase.LoadSynchronous();
	ClassesDatabase = InDatabase->ClassesDatabase.LoadSynchronous();

	SelectData(0);
	RefreshClassOptions();

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
							SNew(SGridPanel)
							+ SGridPanel::Slot(0, 0)
							.Padding(6)
							[
								BuildGeneralSection()
							]

							+ SGridPanel::Slot(0, 1)
							.Padding(6)
							[
								BuildVisualSection()
							]

							+ SGridPanel::Slot(1, 0)
							.Padding(6)
							[
								BuildStartingEquipmentSection()
							]
						]
				]
		];

}

void SCharacterDataEditor::SelectData(int32 DataIndex)
{
	if (CharactersDatabase)
	{
		CharacterData = &CharactersDatabase->Characters[DataIndex];
	}
}

void SCharacterDataEditor::MarkDirty()
{
	if (CharactersDatabase)
	{
		CharactersDatabase->Modify();
		CharactersDatabase->MarkPackageDirty();
	}
}

void SCharacterDataEditor::RefreshClassOptions()
{
	ClassOptions.Reset();

	if (!ClassesDatabase)
	{
		return;
	}

	// Replace with your real array/member names.
	for (const FRPGClassData& ClassData : ClassesDatabase->Classes)
	{
		ClassOptions.Add(MakeShared<FName>(ClassData.ClassId));
	}
}

TArray<TSharedPtr<FName>>* SCharacterDataEditor::GetOptionsSourceForType(FName EquipType)
{
	return &HeadGearOptions;
}

TSharedRef<SWidget> SCharacterDataEditor::BuildGeneralSection()
{
	return SNew(SBorder)
		.Padding(8)
		[
			SNew(SVerticalBox)

				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 0, 0, 8)
				[
					SectionLabel(FText::FromString(TEXT("General Settings")))
				]

				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					TextField(
						FText::FromString(TEXT("Unique Id")),
						[this]() { return FText::FromName(CharacterData->RPGCharacterId); },
						[this](const FText& NewText)
						{
							MarkDirty();
							CharacterData->RPGCharacterId = FName(*NewText.ToString());
						}
					)
				]

			+ SVerticalBox::Slot()
				.AutoHeight()
				[
					TextField(
						FText::FromString(TEXT("Name")),
						[this]() { return FText::FromString(CharacterData->RPGCharacterName); },
						[this](const FText& NewText)
						{
							MarkDirty();
							CharacterData->RPGCharacterName = NewText.ToString();
						}
					)
				]

			+ SVerticalBox::Slot()
				.AutoHeight()
				[
					TextField(
						FText::FromString(TEXT("Nickname")),
						[this]() { return FText::FromString(CharacterData->Nickname); },
						[this](const FText& NewText)
						{
							MarkDirty();
							CharacterData->Nickname = NewText.ToString();
						}
					)
				]

			+ SVerticalBox::Slot()
				.AutoHeight()
				[
					OptionsDropdown(
						&ClassOptions,
						FText::FromString(TEXT("Class")),
						[this]() {
							return CharacterData->RPGClassId;
						},
						[this](const FName& ClassId)
						{
							MarkDirty();
							CharacterData->RPGClassId = ClassId;
						}
					)
				]

				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					IntField(
						FText::FromString(TEXT("Initial Level")),
						[this]() { return CharacterData->InitialLevel; },
						[this](int32 NewValue)
						{
							MarkDirty();
							CharacterData->InitialLevel = NewValue;
						}
					)
				]

			+ SVerticalBox::Slot()
				.AutoHeight()
				[
					IntField(
						FText::FromString(TEXT("Max Level")),
						[this]() { return CharacterData->MaxLevel; },
						[this](int32 NewValue)
						{
							MarkDirty();
							CharacterData->MaxLevel = NewValue;
						}
					)
				]

			+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 6, 0, 0)
				[
					MultiLineTextField(
						FText::FromString(TEXT("Profile")),
						[this]() { return FText::FromString(CharacterData->Profile); },
						[this](const FText& NewText)
						{
							MarkDirty();
							CharacterData->Profile = NewText.ToString();
						}
					)
				]
		];
}

TSharedRef<SWidget> SCharacterDataEditor::BuildVisualSection()
{
	return SNew(SBorder)
		.Padding(8)
		[
			SNew(SVerticalBox)

				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 0, 0, 8)
				[
					SectionLabel(FText::FromString(TEXT("Graphics & Mesh")))
				]

				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SObjectPropertyEntryBox)
						.AllowedClass(UTexture2D::StaticClass())
						.ObjectPath_Lambda([this]()
							{
								return CharacterData->CharacterSprite
									? CharacterData->CharacterSprite->GetPathName()
									: FString();
							})
						.OnObjectChanged_Lambda([this](const FAssetData& AssetData)
							{
								MarkDirty();
								CharacterData->CharacterSprite = Cast<UTexture2D>(AssetData.GetAsset());
							})
				]

			+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 6, 0, 0)
				[
					SNew(SObjectPropertyEntryBox)
						.AllowedClass(UTexture2D::StaticClass())
						.ObjectPath_Lambda([this]()
							{
								return CharacterData->BattlePortrait
									? CharacterData->BattlePortrait->GetPathName()
									: FString();
							})
						.OnObjectChanged_Lambda([this](const FAssetData& AssetData)
							{
								MarkDirty();
								CharacterData->BattlePortrait = Cast<UTexture2D>(AssetData.GetAsset());
							})
				]

			+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 6, 0, 0)
				[
					SNew(SObjectPropertyEntryBox)
						.AllowedClass(UTexture2D::StaticClass())
						.ObjectPath_Lambda([this]()
							{
								return CharacterData->SmallIcon
									? CharacterData->SmallIcon->GetPathName()
									: FString();
							})
						.OnObjectChanged_Lambda([this](const FAssetData& AssetData)
							{
								MarkDirty();
								CharacterData->SmallIcon = Cast<UTexture2D>(AssetData.GetAsset());
							})
				]

			+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 6, 0, 0)
				[
					SNew(SObjectPropertyEntryBox)
						.AllowedClass(ARPGFieldCharacter::StaticClass())
						.ObjectPath_Lambda([this]()
							{
								return CharacterData->FieldCharacterClass
									? CharacterData->FieldCharacterClass->GetPathName()
									: FString();
							})
						.OnObjectChanged_Lambda([this](const FAssetData& AssetData)
							{
								MarkDirty();
								CharacterData->FieldCharacterClass = Cast<ARPGFieldCharacter>(AssetData.GetAsset());
							})
				]

			+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 6, 0, 0)
				[
					SNew(SObjectPropertyEntryBox)
						.AllowedClass(ACharacter::StaticClass())
						.ObjectPath_Lambda([this]()
							{
								return CharacterData->CombatCharacterClass
									? CharacterData->CombatCharacterClass->GetPathName()
									: FString();
							})
						.OnObjectChanged_Lambda([this](const FAssetData& AssetData)
							{
								MarkDirty();
								CharacterData->CombatCharacterClass = Cast<ACharacter>(AssetData.GetAsset());
							})
				]
		];
}

TSharedRef<SWidget> SCharacterDataEditor::BuildStartingEquipmentSection()
{
	TSharedRef<SVerticalBox> StartingEquip = SNew(SVerticalBox);

	StartingEquip->AddSlot()
		.AutoHeight()
		.Padding(0, 0, 0, 8)
		[
			SectionLabel(FText::FromString(TEXT("Starting Equipment")))
		];

	for (FName EquipType : EquipmentTypes)
	{
		FText SlotLabel = FText::FromName(EquipType);
		TArray<TSharedPtr<FName>>* CurrentOptionsSource = GetOptionsSourceForType(EquipType);

		StartingEquip->AddSlot()
			.AutoHeight()
			[
				OptionsDropdown(
					CurrentOptionsSource,
					SlotLabel,
					[this, EquipType]() -> FName
					{
						if (const FEquipItem* Equip = CharacterData->GetStartingEquipment(EquipType))
						{
							return Equip->ItemId;
						}
						return NAME_None;
					},
					[this, EquipType](const FName& ItemId)
					{
						MarkDirty();
						CharacterData->SetStartingEquipment(EquipType, ItemId);
					}
				)
			];
	}

	return SNew(SBorder)
		.Padding(8)
		[
			StartingEquip
		];
}