#pragma once

#include "CoreMinimal.h"
#include "RPGItems.h"
#include "RPGDerivedStats.h"
#include "RPGArmors.generated.h"

UENUM(BlueprintType)
enum class ERPGArmorType : uint8
{
	HeadGear,
	BodyArmor,
	Gloves,
	Pants,
	Boots,
	Accessory
};

USTRUCT(BlueprintType)
struct FRPGArmorData : public FRPGItemData
{
	GENERATED_BODY()

	FRPGArmorData()
	{
		ItemType = ERPGItemType::Equipment;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
	ERPGArmorType ArmorType = ERPGArmorType::HeadGear;

	// Useful if you want designer-defined armor slots from RPGGameSettings.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
	int32 ArmorTypeId = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TArray<FRPGDerivedStatModifier> Modifiers;

	bool IsStackable() const
	{
		return false;
	}

	const FRPGDerivedStatModifier* GetModifier(
		ERPGDerivedStat Stat,
		int32& OutIndex
	) const
	{
		for (int32 i = 0; i < Modifiers.Num(); ++i)
		{
			if (Modifiers[i].Stat == Stat)
			{
				OutIndex = i;
				return &Modifiers[i];
			}
		}

		OutIndex = INDEX_NONE;
		return nullptr;
	}
};

UCLASS(BlueprintType)
class RPGSYSTEM_API URPGArmors : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Armors")
	TArray<FRPGArmorData> Armors;

	UFUNCTION(BlueprintPure, Category = "Armors")
	int32 GetArmorCount() const
	{
		return Armors.Num();
	}

	UFUNCTION(BlueprintPure, Category = "Armors")
	bool GetArmor(FName ItemId, FRPGArmorData& OutArmor) const
	{
		if (const FRPGArmorData* Armor = FindArmorById(ItemId))
		{
			OutArmor = *Armor;
			return true;
		}

		return false;
	}

	const FRPGArmorData* FindArmorById(FName ItemId) const
	{
		for (const FRPGArmorData& Armor : Armors)
		{
			if (Armor.ItemId == ItemId)
			{
				return &Armor;
			}
		}

		return nullptr;
	}

	UFUNCTION(BlueprintPure, Category = "Armors")
	bool GetArmorByOrder(int32 Index, FRPGArmorData& OutArmor) const
	{
		if (!Armors.IsValidIndex(Index))
		{
			return false;
		}

		OutArmor = Armors[Index];
		return true;
	}

	UFUNCTION(BlueprintCallable, Category = "Armors")
	int32 AddArmor()
	{
		FRPGArmorData NewArmor;
		NewArmor.ItemId = MakeUniqueArmorId();
		NewArmor.Name = FText::FromName(NewArmor.ItemId);

		return Armors.Add(NewArmor);
	}

	UFUNCTION(BlueprintPure, Category = "Armors")
	TArray<FString> GetArmorNames() const
	{
		TArray<FString> Names;
		Names.Reserve(Armors.Num());

		for (const FRPGArmorData& Armor : Armors)
		{
			Names.Add(Armor.ItemId.ToString());
		}

		return Names;
	}

	UFUNCTION(BlueprintPure, Category = "Armors")
	TArray<FRPGArmorData> GetArmorsByType(ERPGArmorType ArmorType) const
	{
		TArray<FRPGArmorData> Result;

		for (const FRPGArmorData& Armor : Armors)
		{
			if (Armor.ArmorType == ArmorType)
			{
				Result.Add(Armor);
			}
		}

		return Result;
	}

	UFUNCTION(BlueprintPure, Category = "Armors")
	TArray<FRPGArmorData> GetArmorsByTypeId(int32 ArmorTypeId) const
	{
		TArray<FRPGArmorData> Result;

		for (const FRPGArmorData& Armor : Armors)
		{
			if (Armor.ArmorTypeId == ArmorTypeId)
			{
				Result.Add(Armor);
			}
		}

		return Result;
	}

private:

	FName MakeUniqueArmorId() const
	{
		int32 Index = Armors.Num() + 1;

		while (true)
		{
			const FName Candidate =
				FName(*FString::Printf(TEXT("Armor_%03d"), Index));

			if (!FindArmorById(Candidate))
			{
				return Candidate;
			}

			Index++;
		}
	}
};