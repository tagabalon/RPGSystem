#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGTriggerRunnerSubsystem.generated.h"

class URPGTriggerData;
class URPGCommand;
class ARPGFieldCharacter;
class ARPGTouchTrigger;

USTRUCT()
struct FRPGTriggerExecutionContext
{
	GENERATED_BODY()

	int32 StateIndex = 0;

	UPROPERTY()
	TObjectPtr<ARPGFieldCharacter> InstigatorActor = nullptr;

	UPROPERTY()
	TArray<TObjectPtr<URPGCommand>> Commands;

	UPROPERTY()
	TObjectPtr<URPGCommand> WaitingCommand;

	int32 CommandIndex = 0;
};

UCLASS()
class RPGSYSTEM_API URPGTriggerRunnerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "RPG|Triggers")
	int32 RunTrigger(AActor* TriggerActor, ARPGFieldCharacter* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "RPG|Triggers")
	void ContinueTrigger(AActor* TriggerActor, URPGCommand* WaitingCommand);

	UFUNCTION(BlueprintCallable, Category = "RPG|Triggers")
	void AbortTrigger(AActor* TriggerActor);

	UFUNCTION(BlueprintPure, Category = "RPG|Triggers")
	bool IsTriggerRunning(AActor* TriggerActor) const;

	//UFUNCTION(BlueprintCallable, Category = "RPG|Triggers")
	//void FinishWaiting(URPGCommand* PendingCommand);

private:

	UPROPERTY()
	TMap<AActor*, FRPGTriggerExecutionContext> RunningContexts;

	void ExecuteNextCommand(AActor* TriggerActor);
	void FinishTrigger(AActor* TriggerActor);
};
