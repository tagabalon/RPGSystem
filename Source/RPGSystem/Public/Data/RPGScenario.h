#pragma once

#include "CoreMinimal.h"
#include "RPGScenario.generated.h"

class URPGTriggerData;
class UWorld;

UENUM(BlueprintType)
enum class ERPGLootType : uint8
{
	Item,
	Armor,
	Weapon,
	Gold
};

USTRUCT(BlueprintType)
struct FRPGLootReward
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	ERPGLootType LootType = ERPGLootType::Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	int32 IntValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	float FloatValue = 0.0f;
};

USTRUCT(BlueprintType)
struct FRPGLoot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	TArray<FRPGLootReward> Items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	bool bUnique = true;
};

USTRUCT(BlueprintType)
struct FRPGScenarioNPC
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FString CharacterName = TEXT("NPC");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	TArray<int32> ChatterLines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	bool bCanReply = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	TArray<int32> ResponseLines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	TArray<int32> LoopingLines;
};

USTRUCT(BlueprintType)
struct FRPGScenarioLocation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
	FName LocationId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
	FVector Position = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
	FRotator Rotation = FRotator::ZeroRotator;
};

UCLASS(BlueprintType)
class RPGSYSTEM_API URPGScenario : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FName ScenarioId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FText ScenarioName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity", meta = (MultiLine = true))
	FText Synopsis;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	int32 ChapterNumber = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	TSoftObjectPtr<UWorld> MapLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	FName StartingLocationId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	TArray<FRPGScenarioLocation> Locations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Triggers")
	TArray<TObjectPtr<URPGTriggerData>> TriggerList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
	TArray<FName> FlagKeys;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
	TArray<FName> VariableKeys;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
	TArray<int32> VariableValues;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot")
	TArray<FRPGLoot> Loots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC")
	TArray<FRPGScenarioNPC> Townies;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Editor")
	FString ScenarioFolder = TEXT("/Game/Data/Scenarios/Default");

	UFUNCTION(BlueprintPure, Category = "Triggers")
	int32 GetTriggerIndex(const URPGTriggerData* InTriggerData) const
	{
		return TriggerList.IndexOfByKey(InTriggerData);
	}

	UFUNCTION(BlueprintPure, Category = "Triggers")
	URPGTriggerData* GetTriggerData(int32 Index) const
	{
		return TriggerList.IsValidIndex(Index) ? TriggerList[Index] : nullptr;
	}

	UFUNCTION(BlueprintCallable, Category = "Triggers")
	int32 AddTriggerData(URPGTriggerData* InTriggerData)
	{
		return TriggerList.AddUnique(InTriggerData);
	}

	UFUNCTION(BlueprintPure, Category = "Variables")
	int32 GetFlagCount() const
	{
		return FlagKeys.Num();
	}

	UFUNCTION(BlueprintPure, Category = "Variables")
	int32 GetVariableCount() const
	{
		return VariableKeys.Num();
	}
};