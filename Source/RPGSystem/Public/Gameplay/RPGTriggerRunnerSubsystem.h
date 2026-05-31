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

	UPROPERTY()
	TObjectPtr<ARPGTouchTrigger> Trigger = nullptr;

	UPROPERTY()
	TObjectPtr<ARPGFieldCharacter> InstigatorActor = nullptr;

	UPROPERTY()
	TArray<TObjectPtr<URPGCommand>> Commands;

	int32 CommandIndex = 0;
};

UCLASS()
class RPGSYSTEM_API URPGTriggerRunnerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "RPG|Triggers")
	bool RunTrigger(ARPGTouchTrigger* Trigger, ARPGFieldCharacter* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "RPG|Events")
	void ContinueTrigger();

	UFUNCTION(BlueprintCallable, Category = "RPG|Events")
	void AbortTrigger();

	UFUNCTION(BlueprintPure, Category = "RPG|Events")
	bool IsRunningTrigger() const { return bIsRunning; }

private:
	UPROPERTY()
	TObjectPtr<URPGCommand> WaitingCommand;

	UPROPERTY()
	FRPGTriggerExecutionContext CurrentContext;

	UPROPERTY()
	bool bIsRunning = false;

	void ExecuteNextCommand();
	void FinishTrigger();
};
