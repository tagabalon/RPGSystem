#pragma once

#include "CoreMinimal.h"

#include "RPGWeapons.h"

#include "RPGSkills.generated.h"


UENUM(BlueprintType)
enum class ERPGSkillType : uint8
{
	BasicAttack,
	Martial,
	Magical,
	Passive
};

UENUM(BlueprintType)
enum class ERPGSkillUseScope : uint8
{
	None,
	SingleTarget,
	NarrowArc,
	WideArc,
	FullArc,
	RandomEnemy,
	Area,
	OneAlly,
	AllAllies,
	OneDeadAlly,
	AllDeadAllies,
	User
};

UENUM(BlueprintType)
enum class ERPGHitCheck : uint8
{
	None,
	HitBox,
	Projectile,
	Weapon
};

UENUM(BlueprintType)
enum class ERPGSkillUseMode : uint8
{
	Always,
	CombatMode,
	MapMode,
	Never
};

UENUM(BlueprintType)
enum class ERPGSkillRange : uint8
{
	WeaponBased,
	Melee,
	Ranged,
	Self
};

UENUM(BlueprintType)
enum class ERPGSkillElement : uint8
{
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

UENUM(BlueprintType)
enum class ERPGUseEffect : uint8
{
	PureDamage,
	PhysicalDamage,
	MagicalDamage,
	HealthRecover,
	ManaRecover,
	StaminaRecover,
	Taunted,
	Extra
};

UENUM(BlueprintType)
enum class ERPGPassiveType : uint8
{
	WeaponBonusDamage,
	BlockWeapon,
	StartStaminaMod,
	DerivedStatBonus,
	AllowWeaponType
};

UENUM(BlueprintType)
enum class ERPGGameEffectType : uint8
{
	AddStatus
	// TODO: Add remaining GameEffectType values.
};

USTRUCT(BlueprintType)
struct FRPGSkillId
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SkillId = -1;

	FRPGSkillId() = default;
	explicit FRPGSkillId(int32 InSkillId) : SkillId(InSkillId) {}
};

USTRUCT(BlueprintType)
struct FRPGHitInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ColliderName;
};

USTRUCT(BlueprintType)
struct FRPGGameEffect
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERPGGameEffectType GameEffectType = ERPGGameEffectType::AddStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 IntValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FloatValue = 0.0f;
};

USTRUCT(BlueprintType)
struct FRPGExtraEffect : public FRPGGameEffect
{
	GENERATED_BODY()

	FRPGExtraEffect()
	{
		GameEffectType = ERPGGameEffectType::AddStatus;
		IntValue = 1;
	}

	FString GetDisplayString() const
	{
		switch (GameEffectType)
		{
		case ERPGGameEffectType::AddStatus:
			if (IntValue == 1)
			{
				return TEXT("Staggered");
			}
			if (IntValue == 2)
			{
				return TEXT("Stunned");
			}
			break;
		}

		return TEXT("");
	}
};

USTRUCT(BlueprintType)
struct FRPGPassiveEffect
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERPGPassiveType Effect = ERPGPassiveType::WeaponBonusDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FloatValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 IntValue = 0;

	FString GetName() const
	{
		switch (Effect)
		{
		case ERPGPassiveType::WeaponBonusDamage:
			return TEXT("Weapon Damage Bonus:");

		case ERPGPassiveType::StartStaminaMod:
			return FloatValue >= 1.0f
				? TEXT("Start combat with full Stamina")
				: TEXT("Starting Stamina:");

		default:
			return TEXT("???");
		}
	}

	FString GetValue(bool bIsInEffect) const
	{
		switch (Effect)
		{
		case ERPGPassiveType::WeaponBonusDamage:
			return bIsInEffect
				? FString::Printf(TEXT("%d%%"), FMath::FloorToInt(FloatValue * 100.0f))
				: TEXT("0% (Wrong weapon type)");

		case ERPGPassiveType::StartStaminaMod:
			return FloatValue >= 1.0f
				? TEXT("")
				: FString::Printf(TEXT("%d%%"), FMath::FloorToInt(FloatValue * 100.0f));

		default:
			return TEXT("???");
		}
	}
};

USTRUCT(BlueprintType)
struct FRPGSkillData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity", meta = (MultiLine = true))
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	ERPGSkillType SkillType = ERPGSkillType::BasicAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cost")
	int32 ManaCost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cost")
	int32 StaminaCost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	ERPGSkillUseScope SkillScope = ERPGSkillUseScope::SingleTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	ERPGSkillUseMode SkillMode = ERPGSkillUseMode::CombatMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	ERPGSkillRange RangeType = ERPGSkillRange::WeaponBased;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	int32 SkillRange = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Requirements")
	ERPGWeaponType MainWeaponRequired = ERPGWeaponType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Speed = 1.0f;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FRPGDerivedStatModifier AttackMod;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int32 StaminaGain = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	ERPGUseEffect UseEffect = ERPGUseEffect::PhysicalDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float Damage = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	int32 Recover = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float CastingTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TArray<FRPGExtraEffect> ExtraEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FRPGExtraEffect MainEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Check")
	ERPGHitCheck HitCheck = ERPGHitCheck::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Check")
	TArray<int32> HitColliders;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Check")
	FString HitColliderName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Passive")
	FRPGPassiveEffect Passive;

	int32 GetCollider(int32 AnimIndex) const
	{
		if (HitCheck == ERPGHitCheck::HitBox && HitColliders.Num() == 0)
		{
			return 0;
		}

		if (HitColliders.IsValidIndex(AnimIndex))
		{
			return HitColliders[AnimIndex];
		}

		return -1;
	}
};

UCLASS(BlueprintType)
class RPGSYSTEM_API URPGSkills : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills")
	TArray<FRPGSkillData> Skills;

	UFUNCTION(BlueprintPure)
	int32 GetSkillCount() const
	{
		return Skills.Num();
	}

	UFUNCTION(BlueprintPure)
	bool GetSkill(int32 SkillId, FRPGSkillData& OutSkill) const
	{
		if (!Skills.IsValidIndex(SkillId))
		{
			return false;
		}

		OutSkill = Skills[SkillId];
		return true;
	}

	const FRPGSkillData* GetSkillPtr(int32 SkillId) const
	{
		return Skills.IsValidIndex(SkillId) ? &Skills[SkillId] : nullptr;
	}

	UFUNCTION(BlueprintCallable)
	int32 AddSkill()
	{
		return Skills.Add(FRPGSkillData());
	}

	UFUNCTION(BlueprintPure)
	TArray<FString> GetSkillNames() const
	{
		TArray<FString> Names;
		Names.Reserve(Skills.Num());

		for (const FRPGSkillData& Skill : Skills)
		{
			Names.Add(Skill.Name);
		}

		return Names;
	}

	UFUNCTION(BlueprintCallable)
	int32 DuplicateSkill(int32 SkillId)
	{
		if (!Skills.IsValidIndex(SkillId))
		{
			return INDEX_NONE;
		}

		FRPGSkillData NewSkill = Skills[SkillId];
		NewSkill.Name = TEXT("Copy of ") + NewSkill.Name;

		return Skills.Add(NewSkill);
	}
};