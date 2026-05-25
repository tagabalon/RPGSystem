// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RPGPlayerCharacter.h"

#include "Actors/MapEventActor.h"
#include "RPGSettings.h"

#include "EnhancedActionKeyMapping.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

ARPGPlayerCharacter::ARPGPlayerCharacter() : Super()
{
	/*static ConstructorHelpers::FObjectFinder<UInputMappingContext> FindInputMapping(TEXT("InputMappingContext'RPGSystem/Input/IMC_CRPG.IMC_CRPG'"));
	if (FindInputMapping.Succeeded())
	{
		InputMapping = Cast<UInputMappingContext>(FindInputMapping.Object);
	}*/
}

void ARPGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ARPGPlayerCharacter::SetInteractable(AMapEventActor* InInteractable)
{
	InteractableObject = InInteractable;
}

//void ARPGPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//	// Add Input Mapping Context
//	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
//	{
//		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
//		{
//			Subsystem->AddMappingContext(InputMapping, 0);
//		}
//	}
//
//	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
//	{
//		if (InputCamera != nullptr)
//		{
//			Input->BindAction(InputCamera, ETriggerEvent::Triggered, this, &ARPGPlayerCharacter::InputActionCamera);
//			//CameraActionValue = &Input->BindActionValue(InputCamera);
//		}
//
//		if (InputMove != nullptr)
//		{
//			Input->BindAction(InputMove, ETriggerEvent::Triggered, this, &ARPGPlayerCharacter::InputActionMove);
//			//MoveActionValue = &Input->BindActionValue(InputMove);
//		}
//
//		/*if (InputInteract != nullptr)
//		{
//			Input->BindAction(InputInteract, ETriggerEvent::Started, this, &ARPGPlayerCharacter::Interact);
//		}*/
//	}
//}

//void ARPGPlayerCharacter::InputActionMove(const FInputActionValue& Value)
//{
//	FVector2D MovementVector = Value.Get<FVector2D>();
//	Move(MovementVector);
//}
//
//void ARPGPlayerCharacter::InputActionCamera(const FInputActionValue& Value)
//{
//	FVector2D LookAxisVector = Value.Get<FVector2D>();
//    Look(LookAxisVector);
//}

//void ARPGPlayerCharacter::InputActionInteract()
//{
//	if (InteractableObject != nullptr)
//	{
//		InteractableObject->OnInteract();
//	}
//}

void ARPGPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

