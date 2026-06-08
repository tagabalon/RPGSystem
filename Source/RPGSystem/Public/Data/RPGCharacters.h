#pragma once

#include "CoreMinimal.h"
#include "RPGConstants.h"
#include "RPGTypes.h"

#include "RPGCharacters.generated.h"

class ACharacter;
class URPGClasses;
class URPGDatabase;
class ARPGFieldCharacter;

USTRUCT(BlueprintType)
struct RPGSYSTEM_API FEquipItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (GetOptions = "GetEquipmentTypeOptions"))
	FName EquipmentType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (GetOptions = "GetItemOptions"))
	FName ItemId;
};


USTRUCT(BlueprintType)
struct RPGSYSTEM_API FRPGCharacterData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings", meta = (DisplayName = "Unique Id"))
	FName RPGCharacterId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings", meta = (DisplayName = "Name"))
	FString RPGCharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	FString Nickname;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (GetOptions = "GetClassIdsOptions", DisplayName = "Class"), Category = "General Settings")
	FName RPGClassId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	int32 InitialLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	int32 MaxLevel = 249;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = true), Category = "General Settings")
	FString Profile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	TObjectPtr<UTexture2D> CharacterSprite = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	TObjectPtr<UTexture2D> BattlePortrait = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	TSoftClassPtr<ARPGFieldCharacter> FieldCharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	TSoftClassPtr<ACharacter> CombatCharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	TSubclassOf<UAnimInstance> BattleAnimatorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	TObjectPtr<UTexture2D> SmallIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	TObjectPtr<UTexture2D> MenuIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Stat Bonuses")
	TMap<EBaseStat, FName> StatBonuses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Starting Equipment")
	TArray<FEquipItem> StartingEquipment;
	//TMap<FName, FName> StartingEquipment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	TArray<FName> AttackSkills;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> EquipmentMeshClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UObject> SkillTree = nullptr;*/

	FRPGCharacterData();

	void AddAttackSkill(FName SkillId);
	const FEquipItem* GetStartingEquipment(FName EquipmentType) const;
	void SetStartingEquipment(FName EquipmentType, FName ItemId);

};

UCLASS(BlueprintType)
class RPGSYSTEM_API URPGCharacters : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FRPGCharacterData> Characters;

	UFUNCTION()
	TArray<FName> GetClassIdsOptions() const;

	UFUNCTION(BlueprintPure)
	int32 GetCharacterCount() const
	{
		return Characters.Num();
	}

	UFUNCTION(BlueprintPure)
	bool GetCharacterData(FName CharacterId, FRPGCharacterData& OutCharacter) const;

	UFUNCTION(BlueprintCallable)
	int32 AddCharacter()
	{
		return Characters.Add(FRPGCharacterData());
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