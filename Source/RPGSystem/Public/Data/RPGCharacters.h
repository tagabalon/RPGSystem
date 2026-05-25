#pragma once

#include "CoreMinimal.h"
#include "RPGConstants.h"
#include "RPGTypes.h"
#include "Data/RPGDatabase.h"

#include "RPGCharacters.generated.h"

class URPGClasses;
class ARPGFieldCharacter;

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
struct FRPGCharacterData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
	FName RPGCharacterId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	FString RPGCharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	FString Nickname;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (GetOptions = "GetClassIdsOptions"), Category = "General Settings")
	FName RPGClassId;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<ARPGFieldCharacter> FieldCharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> BattleAnimatorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> StatBonuses = { 0, 0, 0, 0, 0, 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FName> StartingEquipment;

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

	FRPGCharacterData();

	void AddAttackSkill(int32 SkillIndex)
	{
		AttackSkills.Add(SkillIndex);
	}

	void AdjustEquip(int32 NewLength)
	{
		/*const int32 OldLength = Equipments.Num();
		Equipments.SetNum(NewLength);

		for (int32 i = OldLength; i < NewLength; ++i)
		{
			Equipments[i] = -1;
		}*/
	}
};

UCLASS(BlueprintType)
class RPGSYSTEM_API URPGCharacters : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION()
	TArray<FName> GetClassIdsOptions() const
	{
        return URPGDatabase::GetRPGClassIds();
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FRPGCharacterData> Characters;

	UFUNCTION(BlueprintPure)
	int32 GetCharacterCount() const
	{
		return Characters.Num();
	}

	UFUNCTION(BlueprintPure)
	bool GetCharacterData(FName CharacterId, FRPGCharacterData& OutCharacter) const
	{
		const int32 Index = Characters.IndexOfByPredicate([&](const FRPGCharacterData& Character)
		{
			return Character.RPGCharacterId == CharacterId;
        });


		if (Characters.IsValidIndex(Index))
		{
			OutCharacter = Characters[Index];
			return true;
		}

		return false;
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
			Names.Add(Character.RPGCharacterName);
		}

		return Names;
	}

	UFUNCTION(BlueprintPure)
	TArray<FName> GetCharacterIds() const
	{
		TArray<FName> Names;
		Names.Reserve(Characters.Num());

		for (const FRPGCharacterData& Character : Characters)
		{
			Names.Add(Character.RPGCharacterId);
		}

		return Names;
	}
};