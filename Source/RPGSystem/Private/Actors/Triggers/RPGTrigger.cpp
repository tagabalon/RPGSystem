#include "Actors/Triggers/RPGTrigger.h"

#include "Gameplay/RPGTriggerRunnerSubsystem.h"

// Sets default values
ARPGTrigger::ARPGTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TriggerRoot"));
}

// Called when the game starts or when spawned
void ARPGTrigger::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ARPGTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARPGTrigger::ExecuteTrigger_Implementation()
{
	if (!TriggerData || bThreadRunning || !TriggerSource)
	{
		return;
	}

	bThreadRunning = true;
	bThreadFinished = false;

	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance)
	{
		return;
	}

	URPGTriggerRunnerSubsystem* TriggerRunner = GameInstance->GetSubsystem<URPGTriggerRunnerSubsystem>();

	if (!TriggerRunner)
	{
		return;
	}

	const bool bStarted = TriggerRunner->RunTrigger(this, TriggerSource);

	bThreadRunning = bStarted;
}

void ARPGTrigger::SetTriggerData_Implementation(URPGTriggerData* pTriggerData)
{
	TriggerData = pTriggerData;
}

const URPGTriggerData* ARPGTrigger::GetTriggerData_Implementation() const
{
    return TriggerData;
}

void ARPGTrigger::SetFinished_Implementation(ERPGTriggerFinishAction FinishAction)
{
	EnableTrigger(false);

	bThreadRunning = false;
	bThreadFinished = true;
}

void ARPGTrigger::EnableTrigger_Implementation(bool bValue)
{	
    bEnabled = bValue;
	PrimaryActorTick.bCanEverTick = bEnabled;
}

const FRPGEventChain ARPGTrigger::GetActiveState_Implementation() const
{
	if(ActiveStateIndex < TriggerData->States.Num())
	{
		return TriggerData->States[ActiveStateIndex];
    }

    return FRPGEventChain();
}

#if WITH_EDITOR
void ARPGTrigger::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	// Make sure the data asset is valid and assigned
	if (TriggerData)
	{
		// Tell Unreal we are about to modify this asset (handles Undo/Redo and dirtying)
		TriggerData->Modify();

		// Update the data asset's variables (assuming you have SavedLocation/SavedRotation inside it)
		TriggerData->WorldPosition = GetActorLocation();
		TriggerData->WorldRotation = GetActorRotation();
	}
}
#endif