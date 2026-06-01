#pragma once

#include "CoreMinimal.h"
#include "Actors/Triggers/RPGTrigger.h"
#include "GameFramework/Actor.h"
#include "Data/RPGScenario.h"

#include "RPGMapScenario.generated.h"

class URPGScenario;
class ARPGFieldSpawner;
class ARPGTouchTrigger;
class URPGTriggerData;

UCLASS()
class RPGSYSTEM_API ARPGMapScenario : public AActor
{
	GENERATED_BODY()

public:
	ARPGMapScenario();

protected:
	virtual void BeginPlay() override;

    ARPGFieldSpawner* GetSpawner() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scenario")
	TObjectPtr<URPGScenario> ActiveScenario;

	UPROPERTY(BlueprintReadOnly, Category = "Scenario")
	TObjectPtr<URPGScenario> RuntimeActiveScenario;

	UPROPERTY(BlueprintReadOnly, Category = "Triggers")
	TArray<TObjectPtr<ARPGTouchTrigger>> Triggers;

	UPROPERTY(BlueprintReadOnly, Category = "Scenario")
	FName CurrentAreaId;

public:
	UFUNCTION(BlueprintCallable, Category = "Scenario")
	void StartScenario();

	UFUNCTION(BlueprintCallable, Category = "Scenario")
	void ContinueScenario();

	UFUNCTION(BlueprintCallable, Category = "Trigger")
	ARPGTouchTrigger* GetTriggerByData(URPGTriggerData* TriggerData) const;

	UFUNCTION(BlueprintCallable, Category = "Trigger")
	AActor* GetTriggerActorById(FName TriggerId) const;

	UFUNCTION(BlueprintCallable, Category = "Scenario")
	void SetActiveArea(FName AreaId);

	UFUNCTION(BlueprintPure, Category = "Scenario")
	URPGScenario* GetScenario() const
	{
		return ActiveScenario;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity", meta = (GetOptions = "GetScenarioIds"))
	FName ScenarioId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "New Scenario", meta = (EditCondition = "ActiveScenario == nullptr", EditConditionHides))
	FName NewScenarioId;

#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category = "New Scenario", meta = (EditCondition = "NewScenarioId != ''"))
	void CreateNewScenarioAsset();

	UFUNCTION(CallInEditor, Category = "New Trigger")
	void AddTriggerActor();

	FString GetScenariosFolder() const;

	virtual void PostActorCreated() override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif // WITH_EDITOR

	UFUNCTION(BlueprintPure)
	TArray<FName> GetScenarioIds() const;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	TObjectPtr<APlayerStart> DebugStartPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Triggers")
	TArray<TScriptInterface<ITriggerInterface>> ScenarioTriggers;

#endif // WITH_EDITORONLY_DATA

protected:
	UFUNCTION(CallInEditor, Category = "Triggers")
	void SyncTriggers();

	TArray<TObjectPtr<URPGScenario>> GetAllScenarios() const;
	TObjectPtr<URPGScenario> GetScenarioWithId(FName ScenarioId) const;

    TObjectPtr<ARPGFieldSpawner> SpawnManager;

private:
    ARPGTrigger* SpawnTrigger(URPGTriggerData* TriggerData);
};