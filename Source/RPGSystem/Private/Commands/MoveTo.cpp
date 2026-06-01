#include "Commands/MoveTo.h"

#include "Gameplay/RPGProgressionSubsystem.h"
#include "Level/RPGMapScenario.h"

#include "AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

#if WITH_EDITOR
#include "EngineUtils.h"
#endif // WITH_EDITOR

ERPGCommandResult UMoveTo::Execute_Implementation(AActor* TriggerActor, ARPGFieldCharacter* InstigatorActor)
{
    AActor* ActorToMove = nullptr;

    switch (Target)
    {
    case FTargetToMove::Self:
        ActorToMove = TriggerActor;
        break;
    }

    URPGProgressionSubsystem* Progression = URPGProgressionSubsystem::Get(TriggerActor);
    if (!Progression)
    {
        return ERPGCommandResult::Abort;
    }

    FVector DestinationLocation = TriggerActor->GetActorLocation();

    if (ARPGMapScenario* MapScenario = Progression->CurrentScenario)
    {
        if (AActor* Trigger = MapScenario->GetTriggerActorById(TriggerDestination))
        {
            DestinationLocation = Trigger->GetActorLocation();
        }
    }

    if (ActorToMove)
    {
        if (AAIController* AICon = ActorToMove->GetWorld()->SpawnActor<AAIController>())
        {
            AICon->Possess(Cast<APawn>(ActorToMove));

            FAIMoveRequest MoveRequest;
            MoveRequest.SetGoalLocation(DestinationLocation);
            MoveRequest.SetAcceptanceRadius(15.f);

            MoveToResult = AICon->MoveTo(MoveRequest);

            return ERPGCommandResult::Wait;
        }
    }

    return ERPGCommandResult::Continue;
}

ERPGCommandResult UMoveTo::Continue_Implementation()
{
    if (WaitToReachDestination)
    {
        if (MoveToResult.Code == EPathFollowingRequestResult::RequestSuccessful)
        {
            return ERPGCommandResult::Wait;
        }
    }
    return ERPGCommandResult::Continue;
}

TArray<FName> UMoveTo::GetScenarioTriggers() const
{
    TArray<FName> Names;

#if WITH_EDITOR

    UWorld* EditorWorld = nullptr;
    if (GEditor)
    {
        EditorWorld = GEditor->GetEditorWorldContext().World();
    }

    for (const ARPGMapScenario* Scenario : TActorRange<ARPGMapScenario>(EditorWorld))
    {
        if (Scenario->GetScenario() && Scenario->GetScenario()->TriggerList.Num() > 0)
        {
            for (const URPGTriggerData* TriggerData : Scenario->GetScenario()->TriggerList)
            {
                Names.Add(TriggerData->TriggerId);
            }
        }
    }

#endif // WITH_EDITOR

    return Names;
}