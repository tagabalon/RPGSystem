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
	TArray<FName> GetClassIds() const
	{
		TArray<FName> Names;
		Names.Reserve(Classes.Num());

		for (const FRPGClassData& ClassData : Classes)
		{
			Names.Add(ClassData.ClassId);
		}

		return Names;
	}

	UFUNCTION(BlueprintPure)
	bool GetClass(FName ClassId, FRPGClassData& OutClass) const
	{
		const int32 Index = Classes.IndexOfByPredicate([&](const FRPGClassData& Class)
			{
				return Class.ClassId == ClassId;
			});


		if (Classes.IsValidIndex(Index))
		{
			OutClass = Classes[Index];
			return true;
		}

		return false;
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

/*
#include "AssetToolsModule.h"
#include "Curves/CurveFloat.h"
#include "PackageTools.h"

#if WITH_EDITOR
#include "AssetRegistry/AssetRegistryModule.h"
#endif

void URPGClasses::CreateDefaultCurvesForClass(FRPGClassData& ClassData)
{
	ClassData.BaseStats.SetNum(6);

	const FString ClassName = ClassData.ClassId.ToString();

	static const TCHAR* StatNames[] =
	{
		TEXT("Strength"),
		TEXT("Agility"),
		TEXT("Fortitude"),
		TEXT("Intelligence"),
		TEXT("Cunning"),
		TEXT("Luck")
	};

	FAssetToolsModule& AssetTools =
		FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");

	for (int32 i = 0; i < 6; ++i)
	{
		const FString AssetName = FString::Printf(
			TEXT("Curve_%s_%s"),
			*ClassName,
			StatNames[i]
		);

		UPackage* Package = CreatePackage(
			*(CurveAssetFolder / AssetName)
		);

		UCurveFloat* Curve = NewObject<UCurveFloat>(
			Package,
			UCurveFloat::StaticClass(),
			*AssetName,
			RF_Public | RF_Standalone
		);

		Curve->FloatCurve.AddKey(1.0f, 10.0f);
		Curve->FloatCurve.AddKey(50.0f, 100.0f);
		Curve->FloatCurve.AddKey(99.0f, 250.0f);

		FAssetRegistryModule::AssetCreated(Curve);
		Package->MarkPackageDirty();

		ClassData.BaseStats[i] = Curve;
	}

	// XP curve
	const FString XPAssetName = FString::Printf(
		TEXT("Curve_%s_XP"),
		*ClassName
	);

	UPackage* XPPackage = CreatePackage(
		*(CurveAssetFolder / XPAssetName)
	);

	UCurveFloat* XPCurve = NewObject<UCurveFloat>(
		XPPackage,
		UCurveFloat::StaticClass(),
		*XPAssetName,
		RF_Public | RF_Standalone
	);

	XPCurve->FloatCurve.AddKey(1.0f, 50.0f);
	XPCurve->FloatCurve.AddKey(50.0f, 1000.0f);
	XPCurve->FloatCurve.AddKey(99.0f, 5000.0f);

	FAssetRegistryModule::AssetCreated(XPCurve);
	XPPackage->MarkPackageDirty();

	ClassData.XPGain = XPCurve;
}

#endif*/