#pragma once

#include "CoreMinimal.h"
#include "RPGTypes.h"
#include "RPGSkills.h"
#include "RPGMonsters.generated.h"

class UTexture2D;
class UAnimInstance;
class ARPGMonsterCombatCharacter;

UENUM(BlueprintType)
enum class ERPGMonsterDropType : uint8
{
	Item,
	Weapon,
	Armor,
	Crafting
};

UENUM(BlueprintType)
enum class ERPGMonsterActionCondition : uint8
{
	Everytime,
	AtTime,
	WhenHealth,
	WhenMana,
	WhenStamina,
	AtState,
	AtPartyLevel,
	WhenFlag
};

USTRUCT(BlueprintType)
struct FRPGMonsterDropItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop")
	ERPGMonsterDropType ItemType = ERPGMonsterDropType::Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop")
	FName ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float DropRate = 1.0f;
};

USTRUCT(BlueprintType)
struct FRPGMonsterAction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	FName SkillId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
	ERPGMonsterActionCondition UseCondition = ERPGMonsterActionCondition::Everytime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
	int32 Data1 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
	int32 Data2 = 0;
};

USTRUCT(BlueprintType)
struct FRPGMonsterSkillData : public FRPGSkillData
{
	GENERATED_BODY()

	FRPGMonsterSkillData()
	{
		Name = TEXT("Monster Skill");
		Description = TEXT("");
		SkillType = ERPGSkillType::BasicAttack;
		ManaCost = 0;
		StaminaCost = 0;
		SkillMode = ERPGSkillUseMode::CombatMode;
		RangeType = ERPGSkillRange::Melee;
		Speed = 1.0f;
		StaminaGain = 10;
		UseEffect = ERPGUseEffect::PhysicalDamage;
	}

	int32 GetCollider(int32 AnimIndex) const
	{
		if (HitColliders.IsValidIndex(AnimIndex))
		{
			return HitColliders[AnimIndex];
		}

		return 0;
	}
};

USTRUCT(BlueprintType)
struct FRPGMonsterData
{
	GENERATED_BODY()

	FRPGMonsterData()
	{
		BaseStats.Init(10, 6);
		Skills.Add(FRPGMonsterSkillData());
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
	FName MonsterId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity", meta = (MultiLine = true))
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	TObjectPtr<UTexture2D> BattlePortrait = nullptr;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	TSoftClassPtr<ARPGMonsterCombatCharacter> CombatCharacterClass;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TSubclassOf<UAnimInstance> MapAnimationClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TSubclassOf<UAnimInstance> CombatAnimationClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TArray<int32> BaseStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rewards")
	int32 GoldReward = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rewards")
	int32 ExpReward = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rewards")
	TArray<FRPGMonsterDropItem> DropItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Balance")
	int32 ChallengeRating = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	FName EquippedWeaponId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	bool bLoadWeapon = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TArray<FRPGMonsterSkillData> Skills;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death")
	bool bRemoveOnDeath = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 NaturalArmor = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 BaseDamageMin = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 BaseDamageMax = 0;

	const FRPGMonsterSkillData* FindSkillByName(const FString& SkillName) const
	{
		for (const FRPGMonsterSkillData& Skill : Skills)
		{
			if (Skill.Name == SkillName)
			{
				return &Skill;
			}
		}

		return nullptr;
	}

	TArray<FString> GetSkillNames(ERPGSkillType SkillType) const
	{
		TArray<FString> Names;

		for (const FRPGMonsterSkillData& Skill : Skills)
		{
			if (Skill.SkillType == SkillType)
			{
				Names.Add(Skill.Name);
			}
		}

		return Names;
	}

	TArray<FRPGMonsterSkillData> GetSkills(ERPGSkillType SkillType) const
	{
		TArray<FRPGMonsterSkillData> Result;

		for (const FRPGMonsterSkillData& Skill : Skills)
		{
			if (Skill.SkillType == SkillType)
			{
				Result.Add(Skill);
			}
		}

		return Result;
	}
};

UCLASS(BlueprintType)
class RPGSYSTEM_API URPGMonsters : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monsters")
	TArray<FRPGMonsterData> Monsters;

	UFUNCTION(BlueprintPure, Category = "Monsters")
	int32 GetMonsterCount() const
	{
		return Monsters.Num();
	}

	UFUNCTION(BlueprintPure, Category = "Monsters")
	bool GetMonster(FName MonsterId, FRPGMonsterData& OutMonster) const
	{
		if (const FRPGMonsterData* Monster = FindMonsterById(MonsterId))
		{
			OutMonster = *Monster;
			return true;
		}

		return false;
	}

	const FRPGMonsterData* FindMonsterById(FName MonsterId) const
	{
		for (const FRPGMonsterData& Monster : Monsters)
		{
			if (Monster.MonsterId == MonsterId)
			{
				return &Monster;
			}
		}

		return nullptr;
	}

	UFUNCTION(BlueprintCallable, Category = "Monsters")
	int32 AddMonster()
	{
		FRPGMonsterData NewMonster;
		NewMonster.MonsterId = MakeUniqueMonsterId();
		NewMonster.Name = FText::FromName(NewMonster.MonsterId);

		return Monsters.Add(NewMonster);
	}

	UFUNCTION(BlueprintPure, Category = "Monsters")
	TArray<FString> GetMonsterNames() const
	{
		TArray<FString> Names;
		Names.Reserve(Monsters.Num());

		for (const FRPGMonsterData& Monster : Monsters)
		{
			Names.Add(Monster.MonsterId.ToString());
		}

		return Names;
	}

private:

	FName MakeUniqueMonsterId() const
	{
		int32 Index = Monsters.Num() + 1;

		while (true)
		{
			const FName Candidate =
				FName(*FString::Printf(TEXT("Monster_%03d"), Index));

			if (!FindMonsterById(Candidate))
			{
				return Candidate;
			}

			Index++;
		}
	}
};