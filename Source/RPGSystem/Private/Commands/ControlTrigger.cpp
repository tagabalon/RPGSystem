#include "Commands/ControlTrigger.h"

#include "Gameplay/RPGProgressionSubsystem.h"
#include "Level/RPGMapScenario.h"

ERPGCommandResult UControlTrigger::Execute_Implementation(AActor* TriggerActor, ARPGFieldCharacter* InstigatorActor)
{
    AActor* TargetTrigger = nullptr;

    if (Target == FControlTriggerTarget::Self)
    {
        TargetTrigger = TriggerActor;
    }
    else if (Target == FControlTriggerTarget::TriggerById)
    {
        if (URPGProgressionSubsystem* Progression = URPGProgressionSubsystem::Get(TriggerActor))
        {
            if (ARPGMapScenario* MapScenario = Progression->CurrentScenario)
            {
                TargetTrigger = MapScenario->GetTriggerActorById(TriggerId);
            }
        }
    }

    if (TargetTrigger)
    {
        if (bChangeTriggerState)
        {
            ITriggerInterface::Execute_SetActiveState(TargetTrigger, TriggerStateToSet);
        }
    }

    return ERPGCommandResult::Continue;
}
