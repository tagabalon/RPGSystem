#pragma once

#include "CoreMinimal.h"
#include "Curves/CurveFloat.h"
#include "RPGTypes.h"

#include "RPGClasses.generated.h"

class UTexture2D;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None
	// TODO
};

UENUM(BlueprintType)
enum class ESkillElement : uint8
{
	None
	// TODO
};

USTRUCT(BlueprintType)
struct FResistance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillElement Element = ESkillElement::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Rate = 1.0f;
};

USTRUCT(BlueprintType)
struct FSkillLearned
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Skill = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level = 1;
};

//
// Replace this later with your actual modifier struct.
//
USTRUCT(BlueprintType)
struct FDerivedStatModifier
{
	GENERATED_BODY()
};

//
// Replace this later with your actual skill data struct/class.
//
USTRUCT(BlueprintType)
struct FSkillData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name = TEXT("Basic Attack");
};

USTRUCT(BlueprintType)
struct FRPGClassData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ClassId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxLevel = 175;

	//
	// One curve per stat.
	// Index matches EBaseStat.
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UCurveFloat>> BaseStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCurveFloat> XPGain = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EWeaponType> AllowedWeapons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FResistance> Resistances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDerivedStatModifier> Modifiers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSkillLearned> SkillsLearned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSkillData BasicAttack;

	FRPGClassData()
	{
		BaseStats.SetNum(6);
		BasicAttack.Name = TEXT("Basic Attack");
	}

	int32 GetTotalExpAt(int32 Level) const
	{
		if (!XPGain)
		{
			return 0;
		}

		int32 TotalXP = 0;

		for (int32 i = 1; i < Level; ++i)
		{
			TotalXP += FMath::RoundToInt(XPGain->GetFloatValue(i));
		}

		return TotalXP;
	}

	int32 GetExpAt(int32 Level) const
	{
		if (!XPGain)
		{
			return 0;
		}

		return FMath::RoundToInt(XPGain->GetFloatValue(Level));
	}

	int32 GetStat(int32 Level, EBaseStat StatType) const
	{
		const int32 Index = static_cast<int32>(StatType);

		if (!BaseStats.IsValidIndex(Index) || !BaseStats[Index])
		{
			return 0;
		}

		return FMath::RoundToInt(BaseStats[Index]->GetFloatValue(Level));
	}
};

UCLASS(BlueprintType)
class RPGSYSTEM_API URPGClasses : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FRPGClassData> Classes;

	UFUNCTION(BlueprintCallable)
	int32 AddClass()
	{
		return Classes.Add(FRPGClassData());
	}

	UFUNCTION(BlueprintPure)
	TArray<FString> GetClassNames() const
	{
		TArray<FString> Names;
		Names.Reserve(Classes.Num());

		for (const FRPGClassData& ClassData : Classes)
		{
			Names.Add(ClassData.Name);
		}

		return Names;
	}

	UFUNCTION(BlueprintPure)
	bool GetClass(int32 Index, FRPGClassData& OutClass) const
	{
		if (!Classes.IsValidIndex(Index))
		{
			return false;
		}

		OutClass = Classes[Index];
		return true;
	}

	UFUNCTION(BlueprintCallable)
	int32 DuplicateClass(int32 ClassIndex)
	{
		if (!Classes.IsValidIndex(ClassIndex))
		{
			return INDEX_NONE;
		}

		FRPGClassData NewClass = Classes[ClassIndex];
		NewClass.Name = TEXT("Copy of ") + NewClass.Name;

		return Classes.Add(NewClass);
	}
};