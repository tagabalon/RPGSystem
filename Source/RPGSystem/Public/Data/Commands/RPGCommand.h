#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RPGCommand.generated.h"

class ARPGTouchTrigger;

UENUM(BlueprintType)
enum class ERPGCommandResult : uint8
{
	Continue,
	Wait,
	Finish,
	Abort
};

UCLASS(BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced, Abstract)
class RPGSYSTEM_API URPGCommand : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category = "RPG|Command")
	ERPGCommandResult Execute(ARPGTouchTrigger* Trigger, AActor* InstigatorActor);

	virtual ERPGCommandResult Execute_Implementation(
		ARPGTouchTrigger* Trigger,
		AActor* InstigatorActor
	)
	{
		return ERPGCommandResult::Continue;
	}
};