#pragma once

#include "CoreMinimal.h"
#include "Data/RPGDerivedStats.h"
#include "Gameplay/RPGInventory.h"

#include "RPGPartyMember.generated.h"

USTRUCT(BlueprintType)
struct FRPGEquipmentSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SlotId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRPGInventoryItem InventoryItem;
};

USTRUCT(BlueprintType)
struct FRPGCharacterEquipment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRPGEquipmentSlot> EquipmentSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRPGDerivedStats CharacterStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MainWeaponSlotIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 OffHandSlotIndex = -1;
};

USTRUCT(BlueprintType)
struct FRPGPartyMember
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CharacterId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ClassId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText AltName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentXP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> BaseStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRPGDerivedStats DerivedStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentHP = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentMP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentStamina = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRPGCharacterEquipment Equipment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PartyOrder = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsMainActor = false;
};