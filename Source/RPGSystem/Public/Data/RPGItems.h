#pragma once

#include "CoreMinimal.h"
#include "RPGItems.generated.h"

class UTexture2D;

UENUM(BlueprintType)
enum class ERPGItemType : uint8
{
	Normal,
	Consumable,
	Equipment,
	QuestItem,
	AlchemyProduct,
	Herb,
	Spoil,
	Metal,
	Miscellaneous
};

UENUM(BlueprintType)
enum class ERPGItemUseScope : uint8
{
	None,
	OneEnemy,
	AllEnemies,
	RandomEnemy,
	OneAlly,
	AllAllies,
	OneDeadAlly,
	AllDeadAllies,
	User
};

UENUM(BlueprintType)
enum class ERPGItemUseMode : uint8
{
	Always,
	CombatMode,
	MapMode,
	Never
};

USTRUCT(BlueprintType)
struct FRPGItemRecipe
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	FName ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recipe")
	int32 Count = 1;
};

USTRUCT(BlueprintType)
struct FRPGItemEffect
{
	GENERATED_BODY()

	// TODO: Replace with your converted GameEffect type.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FName EffectId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	int32 IntValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float FloatValue = 0.0f;
};

USTRUCT(BlueprintType)
struct FRPGItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
	FName ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity", meta = (MultiLine = true))
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type")
	ERPGItemType ItemType = ERPGItemType::Normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Economy")
	int32 Price = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Economy")
	int32 Weight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
	bool bCraftable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
	TArray<FRPGItemRecipe> Components;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Use")
	ERPGItemUseScope Scope = ERPGItemUseScope::OneAlly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Use")
	ERPGItemUseMode Mode = ERPGItemUseMode::Always;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Use")
	TArray<FRPGItemEffect> Effects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	int32 Speed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	int32 SuccessRate = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	int32 Repeats = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	FName ElementId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	FString Formula;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	int32 Variance = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	bool bCanCritical = false;

	bool IsStackable() const
	{
		return true;
	}

	bool IsConsumable() const
	{
		return ItemType == ERPGItemType::Consumable;
	}
};

UCLASS(BlueprintType)
class RPGSYSTEM_API URPGItems : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items")
	TArray<FRPGItemData> Items;

	UFUNCTION(BlueprintPure, Category = "Items")
	int32 GetItemCount() const
	{
		return Items.Num();
	}

	UFUNCTION(BlueprintPure, Category = "Items")
	bool GetItem(FName ItemId, FRPGItemData& OutItem) const
	{
		if (const FRPGItemData* Item = FindItemById(ItemId))
		{
			OutItem = *Item;
			return true;
		}

		return false;
	}

	const FRPGItemData* FindItemById(FName ItemId) const
	{
		for (const FRPGItemData& Item : Items)
		{
			if (Item.ItemId == ItemId)
			{
				return &Item;
			}
		}

		return nullptr;
	}

	UFUNCTION(BlueprintCallable, Category = "Items")
	int32 AddItem()
	{
		FRPGItemData NewItem;
		NewItem.ItemId = MakeUniqueItemId();

		return Items.Add(NewItem);
	}

	UFUNCTION(BlueprintPure, Category = "Items")
	bool GetItemByOrder(int32 Index, FRPGItemData& OutItem) const
	{
		if (!Items.IsValidIndex(Index))
		{
			return false;
		}

		OutItem = Items[Index];
		return true;
	}

	UFUNCTION(BlueprintPure, Category = "Items")
	TArray<FString> GetItemNameOptions() const
	{
		TArray<FString> Names;
		Names.Reserve(Items.Num());

		for (const FRPGItemData& Item : Items)
		{
			Names.Add(Item.ItemId.ToString());
		}

		return Names;
	}

private:

	FName MakeUniqueItemId() const
	{
		int32 Index = Items.Num() + 1;

		while (true)
		{
			const FName Candidate = FName(*FString::Printf(TEXT("Item_%03d"), Index));

			if (!FindItemById(Candidate))
			{
				return Candidate;
			}

			Index++;
		}
	}
};