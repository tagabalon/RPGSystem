// Fill out your copyright notice in the Description page of Project Settings.


#include "MapEvent.h"
#include "Commands/BranchCommand.h"

#include "GameFramework/PlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MapEvent)

UMapEvent::UMapEvent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UMapEvent::Execute(APlayerController* Player, AMapEventActor* MapEventActor)
{
	if (RootCommand != nullptr)
	{
		RootCommand->Execute(Player, MapEventActor);
	}
}

UBaseCommand* UMapEvent::GetParentCommand(UBaseCommand* InChild) const
{
	UBaseCommand* Parent = RootCommand;
	while (Parent != nullptr) {
		if (UBaseCommand* Child = CastChecked<UBaseCommand>(Parent->GetNextCommand()))
		{
			if (Child == InChild)
			{
				return Parent;
			}
			Parent = Child;
		}
		else
		{
			break;
		}
	}
	return nullptr;
}

#if WITH_EDITOR

UBaseCommand* UMapEvent::CreateCommand(const UClass* EventCommandClass, UEdGraphNode* GraphNode, UBaseCommand* FromCommand, int32 BranchIndex)
{
	UBaseCommand* NewCommand = NewObject<UBaseCommand>(this, EventCommandClass, NAME_None, RF_Transactional);
	NewCommand->SetGraphNode(GraphNode);

	if (RootCommand == nullptr)
	{
		RootCommand = NewCommand;
	}

	if (FromCommand != nullptr)
	{
		if (Contains(FromCommand))
		{
			if (FromCommand->IsA<UBranchCommand>() && BranchIndex >= 0)
			{
				if (UBranchCommand* BranchCommand = CastChecked<UBranchCommand>(FromCommand))
				{
					BranchCommand->SetBranchCommand(NewCommand, BranchIndex);
				}
			}
			else
			{
				FromCommand->SetNextCommand(NewCommand);
			}


		}
	}
	return NewCommand;
}

void UMapEvent::DeleteCommand(UBaseCommand* InCommand)
{
	if (UBaseCommand* ParentCommand = GetParentCommand(InCommand))
	{
		if (UBaseCommand* ChildCommand = CastChecked<UBaseCommand>(InCommand->GetNextCommand()))
		{
			ParentCommand->SetNextCommand(ChildCommand);
		}
		else
		{
			ParentCommand->SetNextCommand(nullptr);
		}
	}

}

bool UMapEvent::Contains(IEventCommandInterface* Command) const
{
	TArray<IEventCommandInterface*> Commands;
	GetCommands(Commands);
	return Commands.Contains(Command);
}

void UMapEvent::GetCommands(TArray<IEventCommandInterface*>& Commands) const
{
	Commands.Empty();
	IEventCommandInterface* Curr = RootCommand;
	while (Curr != nullptr) {
		Commands.AddUnique(Curr);

		Curr = Curr->GetNextCommand();
		if (Curr != nullptr && Commands.Contains(Curr))
		{
			break;
		}
	}
}

#endif
