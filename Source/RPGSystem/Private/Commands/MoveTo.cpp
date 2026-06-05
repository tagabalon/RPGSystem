#include "Commands/MoveTo.h"

#include "Actors/RPGFieldCharacter.h"
#include "Gameplay/RPGProgressionSubsystem.h"
#include "Gameplay/RPGTriggerRunnerSubsystem.h"
#include "Level/RPGMapScenario.h"

#include "AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

#if WITH_EDITOR
#include "EngineUtils.h"
#endif // WITH_EDITOR

ERPGCommandResult UMoveTo::Execute_Implementation(AActor* TriggerActor, ARPGFieldCharacter* InstigatorActor)
{
    Instigator = InstigatorActor;

    AActor* ActorToMove = nullptr;

    switch (Target)
    {
    case ETargetToMove::Self:
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
            MoveRequest.SetUsePathfinding(true);

            if (WaitToReachDestination)
            {
                UPathFollowingComponent* PathFollowComp = AICon->GetPathFollowingComponent();
                if (PathFollowComp)
                {
                    // Ensure we don't double-bind if this function is called multiple times
                    PathFollowComp->OnRequestFinished.RemoveAll(this);
                    // OnRequestFinished is a TMulticastDelegate, not a dynamic multicast delegate, so use AddLambda or AddRaw/AddUObject
                    PathFollowComp->OnRequestFinished.AddUObject(this, &UMoveTo::OnReachDestination);
                }
                MoveToResult = AICon->MoveTo(MoveRequest);

                return ERPGCommandResult::Wait;
            }
            else
            {
                MoveToResult = AICon->MoveTo(MoveRequest);

                return ERPGCommandResult::Continue;
            }
        }
    }

    return ERPGCommandResult::Continue;
}


void UMoveTo::OnReachDestination(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    if (UGameInstance* GameInstance = Instigator->GetGameInstance())
    {
        if (URPGTriggerRunnerSubsystem* TriggerRunner = GameInstance->GetSubsystem<URPGTriggerRunnerSubsystem>())
        {
            TriggerRunner->FinishWaiting(this);
        }
    }
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