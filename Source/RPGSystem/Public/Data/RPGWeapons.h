#pragma once

#include "CoreMinimal.h"
#include "RPGItems.h"
#include "RPGDerivedStats.h"
#include "RPGWeapons.generated.h"

class AActor;

UENUM(BlueprintType)
enum class ERPGWeaponTier : uint8
{
	Poor,
	Common,
	Standard,
	Heroic,
	Rare,
	Legendary,
	Immortal
};

UENUM(BlueprintType)
enum class ERPGWeaponType : uint8
{
	None,
	Sword,
	Spear,
	Axe,
	Bow,
	Sling,
	Dagger,
	Glaive,
	GreatSword,
	Club,
	Warhammer,
	Staff,
	Javelin,
	GreatAxe,
	Shield
};

UENUM(BlueprintType)
enum class ERPGWeaponGrade : uint8
{
	D,
	C,
	B,
	A,
	S
};

UENUM(BlueprintType)
enum class ERPGDamageElement : uint8
{
	None,
	Bludgeoning,
	Piercing,
	Slashing,
	Fire,
	Ice,
	Lightning,
	Poison,
	Light,
	Darkness
};

USTRUCT(BlueprintType)
struct FRPGWeaponData : public FRPGItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	ERPGWeaponType WeaponType = ERPGWeaponType::Sword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	ERPGWeaponTier Tier = ERPGWeaponTier::Common;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	int32 MinDamage = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	int32 MaxDamage = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	ERPGDamageElement DamageElement = ERPGDamageElement::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TArray<FRPGDerivedStatModifier> Modifiers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TSoftClassPtr<AActor> FieldMeshClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TSoftClassPtr<AActor> CombatMeshClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grade")
	TArray<int32> MinDamages = { 50, 40, 30, 20, 10 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grade")
	TArray<int32> MaxDamages = { 75, 60, 45, 30, 15 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grade")
	TArray<int32> PriceBase = { 1000, 750, 500, 250, 100 };

	FRPGWeaponData()
	{
		ItemType = ERPGItemType::Equipment;
		Price = 20;
		bCraftable = true;
	}

	bool IsStackable() const
	{
		return false;
	}

	float GetRange() const
	{
		return WeaponType == ERPGWeaponType::Sword ? 1.5f : 1.0f;
	}

	int32 RollDamage() const
	{
		return FMath::RandRange(MinDamage, MaxDamage);
	}
};

UCLASS(BlueprintType)
class RPGSYSTEM_API URPGWeapons : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
	TArray<FRPGWeaponData> Weapons;

	UFUNCTION(BlueprintPure, Category = "Weapons")
	int32 GetWeaponCount() const
	{
		return Weapons.Num();
	}

	UFUNCTION(BlueprintPure, Category = "Weapons")
	bool GetWeapon(FName ItemId, FRPGWeaponData& OutWeapon) const
	{
		if (const FRPGWeaponData* Weapon = FindWeaponById(ItemId))
		{
			OutWeapon = *Weapon;
			return true;
		}

		return false;
	}

	const FRPGWeaponData* FindWeaponById(FName ItemId) const
	{
		for (const FRPGWeaponData& Weapon : Weapons)
		{
			if (Weapon.ItemId == ItemId)
			{
				return &Weapon;
			}
		}

		return nullptr;
	}

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	int32 AddWeapon()
	{
		FRPGWeaponData NewWeapon;
		NewWeapon.ItemId = MakeUniqueWeaponId();
		NewWeapon.Name = FText::FromName(NewWeapon.ItemId);

		return Weapons.Add(NewWeapon);
	}

	UFUNCTION(BlueprintPure, Category = "Weapons")
	TArray<FString> GetWeaponNames() const
	{
		TArray<FString> Names;

		for (const FRPGWeaponData& Weapon : Weapons)
		{
			Names.Add(Weapon.ItemId.ToString());
		}

		return Names;
	}

	UFUNCTION(BlueprintPure, Category = "Weapons")
	TArray<FRPGWeaponData> GetFilteredWeapons(const TArray<ERPGWeaponType>& TypeFilter) const
	{
		TArray<FRPGWeaponData> Result;

		for (const ERPGWeaponType Type : TypeFilter)
		{
			for (const FRPGWeaponData& Weapon : Weapons)
			{
				if (Weapon.WeaponType == Type)
				{
					Result.Add(Weapon);
				}
			}
		}

		return Result;
	}

private:

	FName MakeUniqueWeaponId() const
	{
		int32 Index = Weapons.Num() + 1;

		while (true)
		{
			const FName Candidate =
				FName(*FString::Printf(TEXT("Weapon_%03d"), Index));

			if (!FindWeaponById(Candidate))
			{
				return Candidate;
			}

			Index++;
		}
	}
};