// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/RPGDatabase.h"

#include "Data/RPGCharacters.h"
#include "Data/RPGClasses.h"
#include "RPGSettings.h"

const URPGDatabase* URPGDatabase::GetRPGDatabase()
{
	if (const URPGSettings* RPGSettings = GetDefault<URPGSettings>())
	{
		return RPGSettings->GameDatabaseAsset.LoadSynchronous();
	}
	else
	{
		static TWeakObjectPtr<URPGDatabase> CachedDatabase;
		if (CachedDatabase.IsValid())
		{
			return CachedDatabase.Get();
		}

		const FSoftObjectPath AssetPath(
			TEXT("/Plugin/RPGSystem/Data/DA_RPGDatabase.DA_RPGDatabase")
		);

		URPGDatabase* LoadedDatabase = Cast<URPGDatabase>(AssetPath.TryLoad());
		CachedDatabase = LoadedDatabase;
		return LoadedDatabase;
	}
}

const TArray<FName> URPGDatabase::GetRPGCharacterIds()
{
	if (const URPGDatabase* Database = GetRPGDatabase())
	{
		if (const URPGCharacters* CharacterDatabase = Database->CharactersDatabase.LoadSynchronous())
		{
			return CharacterDatabase->GetCharacterIds();
		}
	}
	TArray<FName> CharacterNames;
    return CharacterNames;
}

const TArray<FName> URPGDatabase::GetRPGClassIds()
{
	if (const URPGDatabase* Database = GetRPGDatabase())
	{
		if (const URPGClasses* ClassDatabase = Database->ClassesDatabase.LoadSynchronous())
		{
			return ClassDatabase->GetClassIds();
		}
	}

	TArray<FName> ClassNames;
	return ClassNames;
}