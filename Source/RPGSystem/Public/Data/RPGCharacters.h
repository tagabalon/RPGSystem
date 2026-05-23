#pragma once

#include "CoreMinimal.h"
#include "RPGClasses.h"
#include "RPGTypes.h"

#include "RPGCharacters.generated.h"

//class UPaperSprite;
//class UTexture2D;
//class USkeletalMesh;
//class UAnimBlueprint;
//class UActorSkills;


USTRUCT(BlueprintType)
struct FEquipArmor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ArmorType = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ArmorId = 0;
};

USTRUCT(BlueprintType)
struct FRPGCharacterId
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RPGCharacterId = -1;

	FRPGCharacterId() = default;
	explicit FRPGCharacterId(int32 InId) : RPGCharacterId(InId) {}
};

USTRUCT(BlueprintType)
struct FRPGCharacterData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	FString Nickname;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterBuildType BuildType = ECharacterBuildType::None;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (GetOptions = "GetClassNameOptions"), Category = "General Settings")
	FName ClassId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	int32 InitialLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	int32 MaxLevel = 249;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = true), Category = "General Settings")
	FString Profile;

	// Unity Sprite equivalents are usually UTexture2D or UPaperSprite.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> CharacterSprite = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> BattlePortrait = nullptr;

	// Replace these with your actual UE actor/mesh classes later.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> FieldCharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> BattleAnimatorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> StatBonuses = { 0, 0, 0, 0, 0, 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> Equipments = { -1, -1, -1, -1, -1, -1, -1 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEquipArmor> Armors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> AttackSkills;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Build = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> SmallIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> CombatMeshClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> MenuIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> EquipmentMeshClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UObject> SkillTree = nullptr;

	void AddAttackSkill(int32 SkillIndex)
	{
		AttackSkills.Add(SkillIndex);
	}

	void AdjustEquip(int32 NewLength)
	{
		const int32 OldLength = Equipments.Num();
		Equipments.SetNum(NewLength);

		for (int32 i = OldLength; i < NewLength; ++i)
		{
			Equipments[i] = -1;
		}
	}
};

UCLASS(BlueprintType)
class RPGSYSTEM_API URPGCharacters : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<URPGClasses> ClassDatabase;
	
	// Try hard-loading it later
	/*static URPGClasses* GetClassesDatabase()
	{
		static TWeakObjectPtr<URPGClasses> CachedDatabase;

		if (CachedDatabase.IsValid())
		{
			return CachedDatabase.Get();
		}

		const FSoftObjectPath AssetPath(
			TEXT("/Game/Data/DA_RPGClasses.DA_RPGClasses")
		);

		URPGClasses* LoadedDatabase = Cast<URPGClasses>(AssetPath.TryLoad());
		CachedDatabase = LoadedDatabase;

		return LoadedDatabase;
	}*/

	UFUNCTION()
	TArray<FString> GetClassNameOptions() const
	{
		TArray<FString> Options;

		if (!ClassDatabase)
		{
			return Options;
		}

		for (const FRPGClassData& ClassData : ClassDatabase->Classes)
		{
			Options.Add(ClassData.ClassId.ToString());
		}

		return Options;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FRPGCharacterData> Characters;

	UFUNCTION(BlueprintPure)
	int32 GetCharacterCount() const
	{
		return Characters.Num();
	}

	UFUNCTION(BlueprintPure)
	bool GetCharacter(int32 Index, FRPGCharacterData& OutCharacter) const
	{
		if (!Characters.IsValidIndex(Index))
		{
			return false;
		}

		OutCharacter = Characters[Index];
		return true;
	}

	UFUNCTION(BlueprintCallable)
	void AddCharacter()
	{
		Characters.Add(FRPGCharacterData());
	}

	UFUNCTION(BlueprintPure)
	TArray<FString> GetCharacterNames() const
	{
		TArray<FString> Names;
		Names.Reserve(Characters.Num());

		for (const FRPGCharacterData& Character : Characters)
		{
			Names.Add(Character.Name);
		}

		return Names;
	}
};