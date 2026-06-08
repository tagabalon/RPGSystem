#pragma once

#include "CoreMinimal.h"

#include "Data/RPGDatabase.h"

#include "RPGDataEditorWrappers.generated.h"

UCLASS(Abstract)
class RPGSYSTEMEDITOR_API URPGDataEditorObject : public UObject
{
	GENERATED_BODY()

	UFUNCTION()
	TArray<FName> GetClassIdsOptions() const;

	UFUNCTION()
	TArray<FName> GetEquipmentTypeOptions() const;

	UFUNCTION()
	TArray<FName> GetItemOptions() const;
};

UCLASS()
class RPGSYSTEMEDITOR_API URPGCharacterDataEditorObject : public URPGDataEditorObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Character", meta = (ShowOnlyInnerProperties))
	FRPGCharacterData Data;
};

UCLASS()
class RPGSYSTEMEDITOR_API URPGClassDataEditorObject : public URPGDataEditorObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Class", meta = (ShowOnlyInnerProperties))
	FRPGClassData Data;
};

UCLASS()
class RPGSYSTEMEDITOR_API URPGSkillDataEditorObject : public URPGDataEditorObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Skill", meta = (ShowOnlyInnerProperties))
	FRPGSkillData Data;
};

UCLASS()
class RPGSYSTEMEDITOR_API URPGItemDataEditorObject : public URPGDataEditorObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Item", meta = (ShowOnlyInnerProperties))
	FRPGItemData Data;
};

UCLASS()
class RPGSYSTEMEDITOR_API URPGWeaponDataEditorObject : public URPGDataEditorObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (ShowOnlyInnerProperties))
	FRPGWeaponData Data;
};

UCLASS()
class RPGSYSTEMEDITOR_API URPGArmorDataEditorObject : public URPGDataEditorObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Armor", meta = (ShowOnlyInnerProperties))
	FRPGArmorData Data;
};

UCLASS()
class RPGSYSTEMEDITOR_API URPGMonsterDataEditorObject : public URPGDataEditorObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Monster", meta = (ShowOnlyInnerProperties))
	FRPGMonsterData Data;
};
