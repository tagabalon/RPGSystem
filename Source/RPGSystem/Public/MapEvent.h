// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Commands/IEventCommand.h"
#include "Commands/BaseCommand.h"
#include "Interface/MessageInterface.h"
#include "RPGEngine.h"

#include "MapEvent.generated.h"

class AMapEventActor;
class APlayerController;
class UEdGraph;
class UEdGraphNode;
class UMapEvent;


#if WITH_EDITOR

/** Interface for calling the graph editor methods */
class RPGSYSTEM_API IMapEventGraphInterface
{
public:
	IMapEventGraphInterface() {}
	virtual ~IMapEventGraphInterface() {}

	virtual void OnInputTriggered(UEdGraphNode* GraphNode, const int32 Index) const {}
	virtual void OnOutputTriggered(UEdGraphNode* GraphNode, const int32 Index) const {}
};

DECLARE_DELEGATE(FMapEventGraphEvent);

#endif

UCLASS(BlueprintType, hideCategories = Object)
class RPGSYSTEM_API UMapEvent : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	void Execute(APlayerController* Player, AMapEventActor* MapEventActor);

	UPROPERTY()
	FGuid AssetGuid;
#if WITH_EDITOR
	friend class UMapEventGraph;

	void GetCommands(TArray<IEventCommandInterface*>& Commands) const;
	bool Contains(IEventCommandInterface* Command) const;
	UEdGraph* GetGraph() const { return Graph; }
	void SetGraph(UEdGraph* InGraph) { Graph = InGraph; }
	UBaseCommand* CreateCommand(const UClass* EventCommandClass, UEdGraphNode* GraphNode, UBaseCommand* FromCommand, int32 BranchIndex);
	void DeleteCommand(UBaseCommand* InCommand);
	UBaseCommand* GetRootCommand() const { return RootCommand; }
#endif

private:
	UBaseCommand* GetParentCommand(UBaseCommand* InChild) const;


	UPROPERTY(VisibleAnywhere)
	UBaseCommand* RootCommand = nullptr;

#if WITH_EDITORONLY_DATA

	UPROPERTY()
	UEdGraph* Graph = nullptr;

#endif
};
