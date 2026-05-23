#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IEventCommand.generated.h"

class AMapEventActor;
class APlayerController;

/*
This class does not need to be modified.
Empty class for reflection system visibility.
Uses the UINTERFACE macro.
Inherits from UInterface.
*/
UINTERFACE(MinimalAPI, Blueprintable)
class UEventCommandInterface : public UInterface
{
	GENERATED_BODY()
};

/* Actual Interface declaration. */
class IEventCommandInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Add interface function declarations here
	virtual FText GetCategory() const = 0;
	virtual FText GetLabel() const = 0;
	virtual int32 GetGraphPosX() const = 0;
	virtual int32 GetGraphPosY() const = 0;
	virtual IEventCommandInterface* GetNextCommand() const = 0;
	virtual void Execute(APlayerController* Player, AMapEventActor* MapEventActor) = 0;
};