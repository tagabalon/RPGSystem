// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Data/RPGArmors.h"
#include "Data/RPGCharacters.h"
#include "Data/RPGClasses.h"
#include "Data/RPGItems.h"
#include "Data/RPGMonsters.h"
#include "Data/RPGSkills.h"
#include "Data/RPGWeapons.h"

#include "Engine/DataAsset.h"
#include "BuildData.h"

#include "RPGDatabase.generated.h"

class URPGArmors;
class URPGCharacters;
class URPGClasses;
class URPGGameSettings;
class URPGItems;
class URPGMonsters;
class URPGSkills;
class URPGWeapons;

/**
 * 
 */
UCLASS()
class RPGSYSTEM_API URPGDatabase : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Databases")
	TSoftObjectPtr<URPGCharacters> CharactersDatabase;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Databases")
	TSoftObjectPtr<URPGClasses> ClassesDatabase;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Databases")
	TSoftObjectPtr<URPGSkills> SkillsDatabase;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Databases")
	TSoftObjectPtr<URPGItems> ItemsDatabase;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Databases")
    TSoftObjectPtr<URPGWeapons> WeaponsDatabase;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Databases")
    TSoftObjectPtr<URPGArmors> ArmorsDatabase;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Databases")
    TSoftObjectPtr<URPGMonsters> MonstersDatabase;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Databases")
	TSoftObjectPtr<URPGGameSettings> GameSettings;

	static const URPGDatabase* GetRPGDatabase();

	static const TArray<FName> GetRPGCharacterIds();
	static const TArray<FName> GetRPGClassIds();

};
