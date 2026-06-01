#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGFieldSpawner.generated.h"

class ARPGFieldCharacter;
class ARPGMapScenario;
class URPGCharacters;
struct FRPGPartyMember;

UCLASS()
class RPGSYSTEM_API ARPGFieldSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARPGFieldSpawner();

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void InitializeField(URPGScenario* RuntimeScenario);

	UFUNCTION(BlueprintCallable, Category = "Party")
	void SpawnParty();

	UFUNCTION(BlueprintCallable, Category = "Party")
	void SetLeader(FName ActorId);

	UFUNCTION(BlueprintCallable, Category = "Party")
	void CycleLeader();

	UFUNCTION(BlueprintCallable, Category = "Scenario")
	void ActivateScenarioActors(URPGScenario* RuntimeScenario);

	UFUNCTION(BlueprintCallable, Category = "Scenario")
	void ClearSpawnedActors();

	UFUNCTION(BlueprintCallable, Category = "Scenario")
	void SetStartingLocation(AActor* StartLoc);

	UFUNCTION(BlueprintCallable, Category = "Scenario")
    AActor* GetTriggerActorById(FName TriggerId) const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TObjectPtr<ARPGMapScenario> MapScenario;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TObjectPtr<URPGCharacters> CharactersDatabase;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TObjectPtr<AActor> StartingLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TArray<TObjectPtr<AActor>> PartySpawnPoints;

	UPROPERTY(BlueprintReadOnly, Category = "Party")
	TMap<FName, TObjectPtr<ARPGFieldCharacter>> SpawnedPartyMembers;

	UPROPERTY(BlueprintReadOnly, Category = "Party")
	TMap<FName, TObjectPtr<AActor>> TriggerActors;

	UPROPERTY(BlueprintReadOnly, Category = "Party")
	TObjectPtr<ARPGFieldCharacter> CurrentLeader;

	ARPGFieldCharacter* SpawnPartyMember(TSharedPtr<FRPGPartyMember>& PartyMember, int32 PartyIndex);
	void RefreshFollowerTargets();
};
