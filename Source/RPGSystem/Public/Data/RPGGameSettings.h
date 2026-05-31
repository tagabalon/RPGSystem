#pragma once

#include "CoreMinimal.h"
#include "Data/RPGDatabase.h"
#include "RPGConstants.h"

#include "RPGGameSettings.generated.h"

UCLASS(BlueprintType)
class RPGSYSTEM_API URPGGameSettings : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	URPGGameSettings()
	{
		ArmorTypes =
		{
			TEXT("GeneralArmor"),
			TEXT("MagicArmor"),
			TEXT("LightArmor"),
			TEXT("HeavyArmor"),
			TEXT("SmallShield"),
			TEXT("LargeShield")
		};

		SkillTypes =
		{
			TEXT("Magic"),
			TEXT("Special")
        };

		WeaponTypes =
		{
			TEXT("Dagger"),
			TEXT("Sword"),
			TEXT("Flail"),
			TEXT("Axe"),
			TEXT("Whip"),
			TEXT("Cane"),
			TEXT("Bow"),
			TEXT("Crossbow"),
			TEXT("Gun"),
			TEXT("Claw"),
			TEXT("Glove"),
			TEXT("Spear"),
			TEXT("Shield"),
		};

		EquipmentTypes =
		{
			TEXT("MainWeapon"),
			TEXT("OffhandWeapon"),
			TEXT("HeadGear"),
			TEXT("BodyArmor"),
			TEXT("Gloves"),
			TEXT("Pants"),
			TEXT("Boots"),
			EquipmentType_Accessory,
		};
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (GetOptions = "GetCharacterIdsOptions"), Category = "Starting State")
	TArray<FName> StartingParty;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Starting State")
	TObjectPtr<URPGScenario> StartingScenario = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Starting State")
	int32 StartingGold = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Starting State")
	float CombatStartingStaminaRate = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Colors")
	FLinearColor HealthDisplayColor = FLinearColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Colors")
	FLinearColor ManaDisplayColor = FLinearColor::Blue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Colors")
	FLinearColor StaminaDisplayColor = FLinearColor::Green;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Types")
	TArray<FName> ArmorTypes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Types")
	TArray<FName> SkillTypes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Types")
	TArray<FName> WeaponTypes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Types")
	TArray<FName> EquipmentTypes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Types")
	TArray<FName> ElementTypes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actors")
	TArray<FString> ActorTalents;

	UFUNCTION(BlueprintPure, Category = "Starting State")
	const TArray<FName>& GetStartingParty() const
	{
		return StartingParty;
	}

	UFUNCTION(BlueprintPure, Category = "Starting State")
	URPGScenario* GetStartingScenario() const
	{
		return StartingScenario;
	}

	UFUNCTION(BlueprintPure)
	TArray<FName> GetCharacterIdsOptions() const
	{
        return URPGDatabase::GetRPGCharacterIds();
	}

};
