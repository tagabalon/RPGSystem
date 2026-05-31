#include "Gameplay/RPGTriggerRunnerSubsystem.h"

#include "Commands/RPGCommand.h"
#include "Data/RPGTriggerData.h"
#include "Actors/RPGFieldCharacter.h"
#include "Actors/Triggers/RPGTouchTrigger.h"

bool URPGTriggerRunnerSubsystem::RunTrigger(ARPGTouchTrigger* Trigger, ARPGFieldCharacter* InstigatorActor)
{
	if (bIsRunning || !Trigger)
	{
		return false;
	}

    const URPGTriggerData* TriggerData = Trigger->GetTriggerData();
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
	CurrentContext.Trigger = Trigger;
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

		const ERPGCommandResult Result = Command->Execute(CurrentContext.Trigger, CurrentContext.InstigatorActor);

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

void URPGTriggerRunnerSubsystem::AbortTrigger()
{
	bIsRunning = false;
	CurrentContext = FRPGTriggerExecutionContext();
}

void URPGTriggerRunnerSubsystem::FinishTrigger()
{
	ARPGTouchTrigger* Trigger = CurrentContext.Trigger.Get();
	const URPGTriggerData* TriggerData = Trigger->GetTriggerData();

	bIsRunning = false;

	if (Trigger && TriggerData)
	{
		Trigger->SetFinished(TriggerData->FinishAction);
	}

	CurrentContext = FRPGTriggerExecutionContext();
}