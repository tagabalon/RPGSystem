#pragma once

#include "CoreMinimal.h"

#include "Data/RPGDatabase.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Views/STableViewBase.h"

class UPrimaryDataAsset;
class SWidgetSwitcher;
class URPGDataEditorObject;
struct FRPGCharacterData;

class RPGSYSTEMEDITOR_API SRPGDatabaseEditor : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRPGDatabaseEditor) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UPrimaryDataAsset* InDatabaseAsset, FName InTabId);

	static const FName CharactersTabId;
	static const FName ClassesTabId;
	static const FName SkillsTabId;
	static const FName ItemsTabId;
	static const FName WeaponsTabId;
	static const FName ArmorsTabId;
	static const FName MonstersTabId;

protected:
	TSharedRef<SWidget> CreateDataListView();
	virtual void SelectData(int32 DataIndex);
	
	FName TabId;
private:
	int32 SelectedData = 0;
	TArray<TSharedPtr<FString>> DataList;

	UPROPERTY()
	TObjectPtr<URPGDataEditorObject> DataEditorObject;

	TObjectPtr<UPrimaryDataAsset> DatabaseAsset;
	TSharedPtr<SListView<TSharedPtr<FString>>> DataListView;
	TSharedPtr<IDetailsView> DetailsView;

	TObjectPtr<URPGDataEditorObject> CreateDataEditor();

	void SelectCharacter(int32 CharacterIndex);
	void SelectClass(int32 ClassIndex);
	void SelectSkill(int32 SkillIndex);
	void SelectItem(int32 ItemIndex);
	void SelectWeapon(int32 WeaponIndex);
	void SelectArmor(int32 ArmorIndex);
	void SelectMonster(int32 MonsterIndex);
	void OnDoneEditing(const FPropertyChangedEvent& Event);
	FText GetAddButtonLabel() const;
	FReply AddNewData();
	TSharedRef<ITableRow> GenerateDataRow(TSharedPtr<FString> Item, const TSharedRef<STableViewBase>& OwnerTable);
	void OnDataSelected(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo);
	TArray<FString> GetDataList() const;
	FText GetListLabel() const;


protected:
	//Slate drawings
	TSharedRef<STextBlock> SectionLabel(FText Label);
	TSharedRef<SWidget> TextField(const FText& Label, TFunction<FText()> Getter, TFunction<void(const FText&)> Setter);
	TSharedRef<SWidget> OptionsDropdown(TArray<TSharedPtr<FName>>* Options, const FText& Label, TFunction<FName()> Getter, TFunction<void(const FName&)> Setter);
	TSharedRef<SWidget> IntField(const FText& Label, TFunction<int32()> Getter, TFunction<void(int32)> Setter);
	TSharedRef<SWidget> MultiLineTextField(const FText& Label, TFunction<FText()> Getter, TFunction<void(const FText&)> Setter);

};