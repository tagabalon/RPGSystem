// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameSettings.h"
#include "RPGCharacter.h"
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
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Database")
	TArray<URPGCharacter*> Characters;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Database")
	TArray<UBuildData*> Builds;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Database")
	TObjectPtr<UGameSettings> GameSettings;


};
