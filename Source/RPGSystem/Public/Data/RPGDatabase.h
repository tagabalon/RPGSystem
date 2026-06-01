// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BuildData.h"

#include "RPGDatabase.generated.h"

class URPGCharacters;
class URPGClasses;
class URPGItems;
class URPGSkills;
class URPGGameSettings;

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
	TSoftObjectPtr<URPGGameSettings> GameSettings;

	static const URPGDatabase* GetRPGDatabase();

	static const TArray<FName> GetRPGCharacterIds();
	static const TArray<FName> GetRPGClassIds();

};
