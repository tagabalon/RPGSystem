#pragma once

#include "UObject/Interface.h"
#include "Data/RPGTriggerData.h"

#include "TriggerInterface.generated.h"

struct FRPGTriggerState;
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
    int32 GetActiveState(FRPGTriggerState& ActiveState) const;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Trigger")
    void SetTriggerData(URPGTriggerData* TriggerData);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Trigger")
    void SetActiveState(int32 NewState);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Trigger")
    void ExecuteTrigger();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Trigger")
    void EnableTrigger(bool Enabled);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Trigger")
    void InitializeState();
};