#pragma once

#include "CoreMinimal.h"
#include "Containers/Set.h"
#include "UObject/ObjectMacros.h"
#include "UObject/NoExportTypes.h"

#include "RPGProgressionSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FRPGScenarioState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ScenarioId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, bool> Flags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, int32> Variables;
};

UCLASS()
class RPGSYSTEM_API URPGProgressionSubsystem
	: public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "RPG|Progression")
	void InitializeFromMapScenario(ARPGMapScenario* MapScenario);

	UFUNCTION(BlueprintCallable, Category = "RPG|Progression")
	void SetCurrentScenario(URPGScenario* Scenario);

	/*UFUNCTION(BlueprintCallable)
	void SetCurrentScenario(FName ScenarioId);

	UFUNCTION(BlueprintCallable)
	void CompleteTrigger(int32 TriggerId);

	UFUNCTION(BlueprintPure)
	bool IsTriggerCompleted(int32 TriggerId) const;*/

	UPROPERTY(BlueprintReadOnly)
	FName CurrentScenarioId;

	UPROPERTY(BlueprintReadOnly)
	FName CurrentMapId;

	UPROPERTY()
	FString CurrentLevelPackageName;

	UPROPERTY(BlueprintReadOnly)
	FString CurrentAreaName;

	UPROPERTY(BlueprintReadOnly)
	FVector LastPlayerPosition;

	UPROPERTY(BlueprintReadOnly)
	FRotator LastPlayerRotation;

	UPROPERTY(BlueprintReadOnly)
	float TotalPlayTime = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	FDateTime LastPlayed;

	UPROPERTY(BlueprintReadOnly)
	TArray<FRPGScenarioState> ScenarioStates;

	/*UPROPERTY(BlueprintReadOnly)
	TArray<FRPGQuestState> ActiveQuests;*/

	UPROPERTY(BlueprintReadOnly)
	TSet<int32> ActiveTriggers;

	UPROPERTY()
	FDateTime SessionStartTime;
};