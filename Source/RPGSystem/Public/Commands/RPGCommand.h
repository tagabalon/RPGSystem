#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RPGCommand.generated.h"

class ARPGFieldCharacter;
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

protected:
	virtual ERPGCommandResult Execute_Implementation(AActor* TriggerActor, ARPGFieldCharacter* InstigatorActor)
	{
        return ERPGCommandResult::Continue;
	}

	virtual ERPGCommandResult Continue_Implementation()
	{
        return ERPGCommandResult::Continue;
	}

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "RPG|Command")
	ERPGCommandResult Execute(AActor* TriggerActor, ARPGFieldCharacter* InstigatorActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "RPG|Command")
	ERPGCommandResult Continue();


	// Graph properties
public:
	UPROPERTY()
	FGuid CommandGuid;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	FVector2D EditorPosition;
#endif

	virtual FText GetNodeTitle() const
	{
		return FText::FromString(TEXT("RPG Command"));
	}
};