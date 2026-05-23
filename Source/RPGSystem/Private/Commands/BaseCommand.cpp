#include "Commands/BaseCommand.h"

#include "Actors/MapEventActor.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BaseCommand)

FText UBaseCommand::CategoryMessage = FText::FromString(TEXT("Message"));
FText UBaseCommand::CategoryCutscene = FText::FromString(TEXT("Cutscene"));

UBaseCommand::UBaseCommand(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, GraphNode(nullptr)
{
}

void UBaseCommand::Execute(APlayerController* Player, AMapEventActor* MapEventActor)
{
	if (NextCommand == this)
		return;

	if (NextCommand != nullptr)
	{
		NextCommand->Execute(Player, MapEventActor);
	}
}

#if WITH_EDITOR
void UBaseCommand::SetGraphNode(UEdGraphNode* NewGraphNode)
{
	GraphNode = NewGraphNode;
}
#endif