#pragma once

#include "CoreMinimal.h"
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

	UPROPERTY(BlueprintReadOnly, Category = "Scenario")
	TArray<TObjectPtr<URPGScenario>> AvailableScenarios;

	UPROPERTY(BlueprintReadOnly, Category = "Scenario")
	TArray<TObjectPtr<ARPGTouchTrigger>> Triggers;

	UPROPERTY(BlueprintReadOnly, Category = "Scenario")
	FName CurrentAreaId;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scenarios")
	FString ScenarioAssetFolder = TEXT("/Game/Data/Scenarios");
#endif

public:
	UFUNCTION(BlueprintCallable, Category = "Scenario")
	void StartScenario();

	UFUNCTION(BlueprintCallable, Category = "Scenario")
	void ContinueScenario();

	UFUNCTION(BlueprintCallable, Category = "Scenario")
	ARPGTouchTrigger* GetTriggerByData(URPGTriggerData* TriggerData) const;

	UFUNCTION(BlueprintCallable, Category = "Scenario")
	void SetActiveArea(FName AreaId);

	UFUNCTION(BlueprintPure, Category = "Scenario")
	URPGScenario* GetScenario() const
	{
		return ActiveScenario;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FName ScenarioId;

#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category = "Scenarios")
	void CreateNewScenarioAsset();

	virtual void PostActorCreated() override;

	UFUNCTION(CallInEditor, Category = "Scenarios")
	void RefreshScenarios();
#endif
};