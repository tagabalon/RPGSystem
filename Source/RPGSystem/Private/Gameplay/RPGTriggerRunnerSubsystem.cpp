#include "Gameplay/RPGTriggerRunnerSubsystem.h"

#include "Actors/RPGFieldCharacter.h"
#include "Actors/Triggers/RPGTouchTrigger.h"
#include "Commands/RPGCommand.h"
#include "Data/RPGTriggerData.h"
#include "Interface/TriggerInterface.h"

bool URPGTriggerRunnerSubsystem::RunTrigger(AActor* TriggerActor, ARPGFieldCharacter* InstigatorActor)
{
	if (bIsRunning || !TriggerActor || !TriggerActor->GetClass()->ImplementsInterface(UTriggerInterface::StaticClass()))
	{
		return false;
	}

	const URPGTriggerData* TriggerData = ITriggerInterface::Execute_GetTriggerData(TriggerActor);
	if (!TriggerData)
	{
		return false;
    }

	const FRPGEventChain* Chain = TriggerData->GetChain(0);
	if (!Chain)
	{
		return false;
	}

	bIsRunning = true;

	CurrentContext = FRPGTriggerExecutionContext();
	CurrentContext.TriggerActor = TriggerActor;
	CurrentContext.InstigatorActor = InstigatorActor;
	CurrentContext.Commands = Chain->Commands;
	CurrentContext.CommandIndex = 0;

	ExecuteNextCommand();
	return true;
}

void URPGTriggerRunnerSubsystem::ExecuteNextCommand()
{
	if (!bIsRunning)
	{
		return;
	}

	while (CurrentContext.Commands.IsValidIndex(CurrentContext.CommandIndex))
	{
		URPGCommand* Command = CurrentContext.Commands[CurrentContext.CommandIndex];
		CurrentContext.CommandIndex++;

		if (!Command)
		{
			continue;
		}

		const ERPGCommandResult Result = Command->Execute(CurrentContext.TriggerActor, CurrentContext.InstigatorActor);

		switch (Result)
		{
		case ERPGCommandResult::Continue:
			continue;

		case ERPGCommandResult::Wait:
			WaitingCommand = Command;
			return;

		case ERPGCommandResult::Finish:
			FinishTrigger();
			return;

		case ERPGCommandResult::Abort:
			AbortTrigger();
			return;
		}
	}

	FinishTrigger();
}

void URPGTriggerRunnerSubsystem::ContinueTrigger()
{
	if (WaitingCommand)
	{
		const ERPGCommandResult Result = WaitingCommand->Continue();

		if (Result == ERPGCommandResult::Wait)
		{
			return;
		}

		WaitingCommand = nullptr;
	}

	ExecuteNextCommand();
}

void URPGTriggerRunnerSubsystem::FinishWaiting(URPGCommand* PendingCommand)
{
	if (WaitingCommand == PendingCommand)
	{
		WaitingCommand = nullptr;

		ExecuteNextCommand();
	}
}

void URPGTriggerRunnerSubsystem::AbortTrigger()
{
	bIsRunning = false;
	CurrentContext = FRPGTriggerExecutionContext();
}

void URPGTriggerRunnerSubsystem::FinishTrigger()
{
	AActor* Trigger = CurrentContext.TriggerActor.Get();

	if (Trigger->GetClass()->ImplementsInterface(UTriggerInterface::StaticClass()))
	{
		if (const URPGTriggerData* TriggerData = ITriggerInterface::Execute_GetTriggerData(Trigger))
		{
			const FRPGEventChain& ActiveState = ITriggerInterface::Execute_GetActiveState(Trigger);
			ITriggerInterface::Execute_SetFinished(Trigger, ActiveState.FinishAction);
		}
	}

	CurrentContext = FRPGTriggerExecutionContext();
}