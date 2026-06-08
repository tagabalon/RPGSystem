#include "Gameplay/RPGTriggerRunnerSubsystem.h"

#include "Actors/RPGFieldCharacter.h"
#include "Actors/Triggers/RPGTouchTrigger.h"
#include "Commands/RPGCommand.h"
#include "Data/RPGTriggerData.h"
#include "Interface/TriggerInterface.h"

int32 URPGTriggerRunnerSubsystem::RunTrigger(AActor* TriggerActor, ARPGFieldCharacter* InstigatorActor)
{
	if (!TriggerActor || !TriggerActor->GetClass()->ImplementsInterface(UTriggerInterface::StaticClass()))
	{
		return false;
	}

	if (IsTriggerRunning(TriggerActor))
	{
        return false;
	}

	const URPGTriggerData* TriggerData = ITriggerInterface::Execute_GetTriggerData(TriggerActor);
	if (!TriggerData)
	{
		return false;
    }

	FRPGTriggerState ActiveState;	
	int32 ActiveStateIndex = ITriggerInterface::Execute_GetActiveState(TriggerActor, ActiveState);


	FRPGTriggerExecutionContext NewContext = FRPGTriggerExecutionContext();
	NewContext.StateIndex = ActiveStateIndex;
	NewContext.InstigatorActor = InstigatorActor;
	NewContext.Commands = ActiveState.Commands;
	NewContext.CommandIndex = 0;

    RunningContexts.Add(TriggerActor, NewContext);

	ExecuteNextCommand(TriggerActor);
	return ActiveStateIndex;
}

void URPGTriggerRunnerSubsystem::ExecuteNextCommand(AActor* TriggerActor)
{
	if (!RunningContexts.Contains(TriggerActor))
	{
		return;
	}

    FRPGTriggerExecutionContext* CurrentContext = &RunningContexts.FindChecked(TriggerActor);
	if (!CurrentContext)
	{
		return;
    }

	while (CurrentContext->Commands.IsValidIndex(CurrentContext->CommandIndex))
	{
		URPGCommand* Command = CurrentContext->Commands[CurrentContext->CommandIndex];
		CurrentContext->CommandIndex++;

		if (!Command)
		{
			continue;
		}

		const ERPGCommandResult Result = Command->Execute(TriggerActor, CurrentContext->InstigatorActor);

		switch (Result)
		{
		case ERPGCommandResult::Continue:
			continue;

		case ERPGCommandResult::Wait:
			CurrentContext->WaitingCommand = Command;
			return;

		case ERPGCommandResult::Abort:
			AbortTrigger(TriggerActor);
			return;
		}
	}

	FinishTrigger(TriggerActor);
}

void URPGTriggerRunnerSubsystem::ContinueTrigger(AActor* TriggerActor, URPGCommand* WaitingCommand)
{
	if (FRPGTriggerExecutionContext* CurrentContext = &RunningContexts.FindChecked(TriggerActor))	
	{
		if (CurrentContext->WaitingCommand == WaitingCommand)
		{
			CurrentContext->WaitingCommand = nullptr;
        }
		ExecuteNextCommand(TriggerActor);
	}
}

//void URPGTriggerRunnerSubsystem::FinishWaiting(URPGCommand* PendingCommand)
//{
//	if (WaitingCommand == PendingCommand)
//	{
//		WaitingCommand = nullptr;
//
//		ExecuteNextCommand();
//	}
//}

void URPGTriggerRunnerSubsystem::AbortTrigger(AActor* TriggerActor)
{
	if (RunningContexts.Contains(TriggerActor))
	{
		RunningContexts.Remove(TriggerActor);
    }
}

bool URPGTriggerRunnerSubsystem::IsTriggerRunning(AActor* TriggerActor) const
{
	if (RunningContexts.Contains(TriggerActor))
	{
		return true;
    }
	return false;
}

void URPGTriggerRunnerSubsystem::FinishTrigger(AActor* TriggerActor)
{
	if (RunningContexts.Contains(TriggerActor))
	{
		RunningContexts.Remove(TriggerActor);

		ITriggerInterface::Execute_InitializeState(TriggerActor);
    }
	/*if (Trigger->GetClass()->ImplementsInterface(UTriggerInterface::StaticClass()))
	{
		if (const URPGTriggerData* TriggerData = ITriggerInterface::Execute_GetTriggerData(Trigger))
		{
			FRPGTriggerState ActiveState;
			if (ITriggerInterface::Execute_GetActiveState(Trigger, ActiveState) >= 0)
			{
				ITriggerInterface::Execute_SetFinished(Trigger, ActiveState.FinishAction);
			}
		}
	}*/

}