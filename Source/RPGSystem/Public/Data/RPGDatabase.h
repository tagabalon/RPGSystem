// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RPGCharacters.h"
#include "RPGClasses.h"
#include "RPGSkills.h"
#include "BuildData.h"

#include "RPGDatabase.generated.h"


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


};
