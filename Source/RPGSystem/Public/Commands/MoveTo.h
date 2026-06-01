#pragma once

#include "RPGCommand.h"
#include "Navigation/PathFollowingComponent.h"

#include "MoveTo.generated.h"

struct FPathFollowingRequestResult;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move To")
	FTargetToMove Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move To", meta = (GetOptions = "GetScenarioTriggers"))
	FName TriggerDestination;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move To")
	bool WaitToReachDestination;

	virtual ERPGCommandResult Execute_Implementation(AActor* TriggerActor, ARPGFieldCharacter* InstigatorActor) override;

	virtual ERPGCommandResult Continue_Implementation() override;

protected:
	UFUNCTION(BlueprintPure)
	TArray<FName> GetScenarioTriggers() const;

private:
	FPathFollowingRequestResult MoveToResult;
};
