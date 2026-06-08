#pragma once

#include "CoreMinimal.h"

#include "SRPGDatabaseEditor.h"

struct FRPGCharacterData;
class URPGCharacters;
class URPGClasses;

class SCharacterDataEditor : public SRPGDatabaseEditor
{
public:
	SLATE_BEGIN_ARGS(SCharacterDataEditor) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, URPGDatabase *InDatabase);

private:
	virtual void SelectData(int32 DataIndex);

	URPGCharacters* CharactersDatabase = nullptr;
	URPGClasses* ClassesDatabase = nullptr;
	FRPGCharacterData* CharacterData = nullptr;

	TArray<TSharedPtr<FName>> ClassOptions;
	TArray<TSharedPtr<FName>> HeadGearOptions;
	TArray<TSharedPtr<FName>> BodyArmorOptions;
	TArray<TSharedPtr<FName>> BootsOptions;
	TArray<TSharedPtr<FName>> AccessoryOptions;
	TArray<TSharedPtr<FName>> WeaponOptions;
	TArray<FName> EquipmentTypes;

	void MarkDirty();
	void RefreshClassOptions();
	TArray<TSharedPtr<FName>>* GetOptionsSourceForType(FName EquipType);

	TSharedRef<SWidget> BuildGeneralSection();
	TSharedRef<SWidget> BuildVisualSection();
	TSharedRef<SWidget> BuildStartingEquipmentSection();
};