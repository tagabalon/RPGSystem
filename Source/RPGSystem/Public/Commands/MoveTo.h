#pragma once

#include "RPGCommand.h"
#include "Navigation/PathFollowingComponent.h"

#include "MoveTo.generated.h"

struct FPathFollowingResult;

UENUM(BlueprintType)
enum class FTargetToMove : uint8
{
	Self,
	MainCharacter,
	TriggerActor,
	PartyFollower,
	NPCActor
};

UCLASS(Blueprintable, Category = "RPG System")
class RPGSYSTEM_API UMoveTo : public URPGCommand
{
	GENERATED_BODY()
public:
    // URPGCommand interface
	virtual ERPGCommandResult Execute_Implementation(AActor* TriggerActor, ARPGFieldCharacter* InstigatorActor) override;
	virtual ERPGCommandResult Continue_Implementation() override;

    // The target to move. If PartyFollower is selected, the command will try to move the first party member that is not the main character.
	// If NPCActor is selected, the command will try to move the first NPC actor that is not the main character.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move To")
	FTargetToMove Target;

    // The destination trigger to move to. This should be the name of a trigger placed in the level. The command will try to find a trigger with this name and move to it.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move To", meta = (GetOptions = "GetScenarioTriggers"))
	FName TriggerDestination;

    // If true, the command will wait until the character reaches the destination before finishing. If false, the command will finish immediately after issuing the move command.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move To")
	bool WaitToReachDestination;

	void OnReachDestination(FAIRequestID RequestID, const FPathFollowingResult& Result);
protected:
	UFUNCTION(BlueprintPure)
	TArray<FName> GetScenarioTriggers() const;

private:
	FPathFollowingRequestResult MoveToResult;

	UPROPERTY()
	TObjectPtr<ARPGFieldCharacter> Instigator;
};
