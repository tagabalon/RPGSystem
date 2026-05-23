#pragma once

#include "CoreMinimal.h"
#include "IEventCommand.h"
#include "BaseCommand.generated.h"

class AMapEventActor;

/*
*/
UCLASS(Abstract, Blueprintable)
class RPGSYSTEM_API UBaseCommand : public UObject, public IEventCommandInterface
{
	GENERATED_UCLASS_BODY()

public:

	UEdGraphNode* GetGraphNode() const { return GraphNode; }

	UFUNCTION(BlueprintCallable, Category = "Command")
	void SetGuid(const FGuid& NewGuid) { NodeGuid = NewGuid; }

	UPROPERTY()
	UEdGraphNode* GraphNode;

#if WITH_EDITOR
	void SetGraphNode(UEdGraphNode* NewGraphNode);
	void SetNextCommand(UBaseCommand* InNextCommand) { NextCommand = InNextCommand; }
#endif

	static FText CategoryMessage;
	static FText CategoryCutscene;

	UPROPERTY()
	FGuid NodeGuid;


	//UEventCommandInterface
	virtual FText GetCategory() const
	{
		return FText::FromString("Commands");
	}

	virtual FText GetLabel() const {
		return FText::FromString(TEXT("Unknown Command"));
	}

	int32 GetGraphPosX() const {
		return GraphPosition.X;
	}

	int32 GetGraphPosY() const {
		return GraphPosition.Y;
	}

	IEventCommandInterface* GetNextCommand() const {
		return NextCommand;
	}

	virtual uint32 GetInputPins() const
	{
		return 1;
	}
	
	virtual uint32 GetOutputPins() const
	{
			return 1;
	}

	virtual void Execute(APlayerController* Player, AMapEventActor* MapEventActor);

	virtual bool IsUINeeded() { return false; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBaseCommand* NextCommand;

	FVector2D GraphPosition;
};