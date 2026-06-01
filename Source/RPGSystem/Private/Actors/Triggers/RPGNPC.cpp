// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/Triggers/RPGNPC.h"

#include "Actors/RPGFieldCharacter.h"
#include "Components/RPGInteractableComponent.h"
#include "Gameplay/RPGTriggerRunnerSubsystem.h"

#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ARPGNPC::ARPGNPC()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->bDynamicObstacle = true;

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	InteractComponent = CreateDefaultSubobject<URPGInteractableComponent>(TEXT("Interactable"));
	InteractComponent->SetupAttachment(RootComponent);
	InteractComponent->SetAwarenessRadius(400.0f);
	InteractComponent->OnInteractPressed.BindUObject(this, &ARPGNPC::ExecuteTrigger);
    InteractComponent->OnCheckTriggerCondition.BindUObject(this, &ARPGNPC::CheckRequirements);
    InteractComponent->OnPromptVisibilityChanged.AddDynamic(this, &ARPGNPC::OnPromptSetVisible);

	AudioSource = CreateDefaultSubobject<UAudioComponent>("AudioSource");
	AudioSource->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ARPGNPC::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ARPGNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARPGNPC::OnPromptSetVisible(ARPGFieldCharacter* TriggeringCharacter, bool bIsVisible)
{
	TriggerSource = TriggeringCharacter;

	if (TriggerSource)
	{
		TriggerSource->SetInteractableTrigger(this);
	}
}

bool ARPGNPC::CheckRequirements() const
{	
	return true;
}

void ARPGNPC::ExecuteTrigger_Implementation()
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

const URPGTriggerData* ARPGNPC::GetTriggerData_Implementation() const
{
	return TriggerData;
}

const FRPGEventChain ARPGNPC::GetActiveState_Implementation() const
{
	if (ActiveStateIndex < TriggerData->States.Num())
	{
		return TriggerData->States[ActiveStateIndex];
	}

	return FRPGEventChain();
}

void ARPGNPC::SetFinished_Implementation(ERPGTriggerFinishAction FinishAction)
{

}
void ARPGNPC::EnableTrigger_Implementation(bool Enabled)
{
	InteractComponent->EnableInteract(Enabled);
}

void ARPGNPC::SetTriggerData_Implementation(URPGTriggerData* pTriggerData)
{
	TriggerData = pTriggerData;
}

#if WITH_EDITOR
void ARPGNPC::PostEditMove(bool bFinished)
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
