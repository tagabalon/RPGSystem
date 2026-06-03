#pragma once

#include "CoreMinimal.h"

#include "RPGCommand.h"

#include "ControlTrigger.generated.h"

UENUM(BlueprintType)
enum class FControlTriggerTarget : uint8
{
	Self,
	TriggerById
};

UCLASS(Blueprintable, Category = "RPG System")
class RPGSYSTEM_API UControlTrigger : public URPGCommand
{
	GENERATED_BODY()
public:
	virtual ERPGCommandResult Execute_Implementation(AActor* TriggerActor, ARPGFieldCharacter* InstigatorActor) override;

    // The target trigger to control. If TriggerById is selected, the command will try to find a trigger with the specified ID and control it.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control Trigger")
    FControlTriggerTarget Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control Trigger", meta = (EditCondition = "Target == FControlTriggerTarget::TriggerById"))
	FName TriggerId;

    // Whether to change the trigger's state. If true, the command will attempt to set the trigger to the specified state. If false, the command will not change the trigger's state but may still execute other logic (not implemented in this basic version).
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control Trigger")
    bool bChangeTriggerState = false;

    // The state to set the trigger to. This should be a valid state defined in the trigger's data asset. The command will try to set the trigger to this state.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control Trigger")
    int32 TriggerStateToSet;
};
