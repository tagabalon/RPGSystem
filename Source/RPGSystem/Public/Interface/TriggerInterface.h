#pragma once

#include "UObject/Interface.h"

struct FRPGEventChain;

#include "TriggerInterface.generated.h"

UINTERFACE(MinimalAPI)
class UTriggerInterface : public UInterface
{
    GENERATED_BODY()
};

class RPGSYSTEM_API ITriggerInterface
{
    GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Trigger")
    const URPGTriggerData* GetTriggerData() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Trigger")
    const FRPGEventChain GetActiveState() const;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Trigger")
    void SetTriggerData(URPGTriggerData* TriggerData);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Trigger")
    void ExecuteTrigger();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Trigger")
    void EnableTrigger(bool Enabled);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Trigger")
    void SetFinished(ERPGTriggerFinishAction FinishAction);
};