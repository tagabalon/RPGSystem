#pragma once

#include "CoreMinimal.h"

#include "RPGDerivedStats.generated.h"

UENUM(BlueprintType)
enum class ERPGDerivedStat : uint8
{
	MaxHealth,
	MaxMana,
	MaxStamina,
	MaxCombo,
	PhysicalDamage,
	PhysicalDefense,
	MagicalDamage,
	MagicalDefense,

	Attack,
	Evasion,
	MoveSpeed,
	Critical,
	Reflect,
	Parry,
	HPRegen,
	MPRegen,
	SPRegen,
	Aggro
};

UENUM(BlueprintType)
enum class ERPGModifierSource : uint8
{
	Constant,
	Level,
	Strength,
	Agility,
	Fortitude,
	Intelligence,
	Cunning,
	Luck
};

USTRUCT(BlueprintType)
struct FRPGDerivedStatModifier
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
	ERPGDerivedStat Stat = ERPGDerivedStat::MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
	ERPGModifierSource Source = ERPGModifierSource::Constant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
	int32 IntValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
	float FloatValue = 1.0f;

	FString ToString() const
	{
		switch (Stat)
		{
		case ERPGDerivedStat::Attack:
		case ERPGDerivedStat::Evasion:
			return FString::Printf(TEXT("%.2f%%"), FloatValue * 100.0f);

		default:
			return FString::FromInt(IntValue);
		}
	}
};

USTRUCT(BlueprintType)
struct FRPGDerivedStats
{
	GENERATED_BODY()

	static constexpr int32 WholeStatCount = 8;
	static constexpr int32 DecimalStatCount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TArray<int32> WholeStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TArray<float> DecimalStats;

	FRPGDerivedStats()
	{
		Reset();
	}

	void Reset()
	{
		WholeStats.Init(0, WholeStatCount);
		DecimalStats.Init(1.0f, DecimalStatCount);
	}

	void InitializeWithBaseStats(const TArray<int32>& BaseStats, int32 Level)
	{
		if (BaseStats.Num() < 6)
		{
			UE_LOG(LogTemp, Warning, TEXT("BaseStats array must have at least 6 elements."));
			return;
		}
		Initialize(
			Level,
			BaseStats[0],
			BaseStats[1],
			BaseStats[2],
			BaseStats[3],
			BaseStats[4],
			BaseStats[5]
		);
    }

	void Initialize(
		int32 Level,
		int32 Strength,
		int32 Agility,
		int32 Fortitude,
		int32 Intelligence,
		int32 Cunning,
		int32 Luck
	)
	{
		WholeStats.Init(0, WholeStatCount);

		WholeStats[(int32)ERPGDerivedStat::MaxHealth] =
			FMath::RoundToInt((Fortitude * 16.0f) + (Level * 40.0f));

		WholeStats[(int32)ERPGDerivedStat::MaxMana] =
			(Intelligence * 12) + (Level * 9);

		WholeStats[(int32)ERPGDerivedStat::MaxStamina] =
			(Cunning * 11) + (Level * 8);

		WholeStats[(int32)ERPGDerivedStat::MaxCombo] =
			FMath::RoundToInt((Agility * 0.7f + 0.29f) + (Level * 0.11f + 0.89f));

		WholeStats[(int32)ERPGDerivedStat::PhysicalDamage] =
			FMath::RoundToInt((Strength * 1.04f + 6.96f) + (Level * 0.22f + 1.78f));

		WholeStats[(int32)ERPGDerivedStat::PhysicalDefense] =
			FMath::RoundToInt(
				(Strength * 0.22f + 2.77f) +
				(Fortitude * 0.88f + 3.12f) +
				(Level * 0.14f + 0.86f)
			) / 4;

		WholeStats[(int32)ERPGDerivedStat::MagicalDamage] =
			FMath::RoundToInt((Intelligence * 1.41f + 10.6f) + (Level * 0.38f + 2.61f));

		WholeStats[(int32)ERPGDerivedStat::MagicalDefense] =
			FMath::RoundToInt(
				(Fortitude * 0.6f + 0.4f) +
				(Level * 0.22f + 0.77f) +
				(Intelligence * 0.98f + 3.02f)
			);

		DecimalStats.Init(1.0f, DecimalStatCount);

		DecimalStats[(int32)ERPGDerivedStat::Attack - WholeStatCount] =
			((Luck * 1.58f + 43.42f) + (Cunning * 0.97f + 4.03f)) / 100.0f;

		DecimalStats[(int32)ERPGDerivedStat::Evasion - WholeStatCount] =
			FMath::Max(
				(((Luck * 0.51f + 49.49f) + (Agility * 0.76f + 24.23f)) / 100.0f) - 0.65f,
				0.01f
			);

		DecimalStats[(int32)ERPGDerivedStat::MoveSpeed - WholeStatCount] =
			((75.0f * Agility + 9725.0f) / 98.0f) / 100.0f;

		DecimalStats[(int32)ERPGDerivedStat::Critical - WholeStatCount] =
			(Luck * 3.05f - 2.05f) / 100.0f;
	}

	int32 GetInt(ERPGDerivedStat Stat) const
	{
		const int32 Index = (int32)Stat;

		if (Index < WholeStatCount)
		{
			return WholeStats.IsValidIndex(Index) ? WholeStats[Index] : 0;
		}

		const int32 DecimalIndex = Index - WholeStatCount;
		return DecimalStats.IsValidIndex(DecimalIndex)
			? FMath::RoundToInt(DecimalStats[DecimalIndex] * 100.0f)
			: 0;
	}

	float GetFloat(ERPGDerivedStat Stat) const
	{
		const int32 Index = (int32)Stat;

		if (Index < WholeStatCount)
		{
			return WholeStats.IsValidIndex(Index) ? WholeStats[Index] : 0.0f;
		}

		const int32 DecimalIndex = Index - WholeStatCount;
		return DecimalStats.IsValidIndex(DecimalIndex) ? DecimalStats[DecimalIndex] : 0.0f;
	}

	void AddModifier(const FRPGDerivedStatModifier& Modifier)
	{
		const int32 Index = (int32)Modifier.Stat;

		if (Index < WholeStatCount)
		{
			if (WholeStats.IsValidIndex(Index))
			{
				WholeStats[Index] += Modifier.IntValue;
			}
		}
		else
		{
			const int32 DecimalIndex = Index - WholeStatCount;

			if (DecimalStats.IsValidIndex(DecimalIndex))
			{
				DecimalStats[DecimalIndex] *= Modifier.FloatValue;
			}
		}
	}

	static FString GetStatName(ERPGDerivedStat Stat)
	{
		switch (Stat)
		{
		case ERPGDerivedStat::Attack: return TEXT("ATK");
		case ERPGDerivedStat::Evasion: return TEXT("EVA");
		case ERPGDerivedStat::MagicalDamage: return TEXT("M-DMG");
		case ERPGDerivedStat::MagicalDefense: return TEXT("M-DEF");
		case ERPGDerivedStat::MaxCombo: return TEXT("COMBO");
		case ERPGDerivedStat::MaxHealth: return TEXT("MAX-HP");
		case ERPGDerivedStat::MaxMana: return TEXT("MAX-MP");
		case ERPGDerivedStat::MaxStamina: return TEXT("MAX-SP");
		case ERPGDerivedStat::PhysicalDamage: return TEXT("DMG");
		case ERPGDerivedStat::PhysicalDefense: return TEXT("DEF");
		case ERPGDerivedStat::MoveSpeed: return TEXT("SPEED");
		default: return TEXT("STAT");
		}
	}
};
