// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MainCharacter.h"

#include "Actors/MapEventActor.h"
#include "RPGSettings.h"

#include "EnhancedActionKeyMapping.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> FindInputMapping(TEXT("InputMappingContext'RPGSystem/Input/IMC_CRPG.IMC_CRPG'"));
	if (FindInputMapping.Succeeded())
	{
		InputMapping = Cast<UInputMappingContext>(FindInputMapping.Object);
	}
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(InputMapping != nullptr)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(InputMapping, 0);
			}
		}
	}
}

void AMainCharacter::SetInteractable(AMapEventActor* InInteractable)
{
	InteractableObject = InInteractable;
}

void AMainCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (InputCamera != nullptr)
		{
			Input->BindAction(InputCamera, ETriggerEvent::Triggered, this, FName("InputActionCamera"));
			CameraActionValue = &Input->BindActionValue(InputCamera);
		}

		if (InputMove != nullptr)
		{
			Input->BindAction(InputMove, ETriggerEvent::Triggered, this, FName("InputActionMove"));
			MoveActionValue = &Input->BindActionValue(InputMove);
		}

		if (InputInteract != nullptr)
		{
			Input->BindAction(InputInteract, ETriggerEvent::Triggered, this, FName("InputActionInteract"));
		}
	}
}

void AMainCharacter::InputActionCamera()
{
	if (!bCameraEnabled)
		return;

	const FVector2D& InputValue = CameraActionValue->GetValue().Get<FVector2D>();

	On_Input_Camera(InputValue.X, InputValue.Y);


	//GEngine->AddOnScreenDebugMessage(- 1, 2.f, FColor::Yellow, InputValue.ToString());

}

void AMainCharacter::InputActionMove()
{
	if (!bMovementEnabled)
		return;

	const FVector2D& InputValue = MoveActionValue->GetValue().Get<FVector2D>();

	On_Input_Move(InputValue.X, InputValue.Y);

	/*const FRotator& rotation = GetControlRotation();
	const FVector& rightVector = UKismetMathLibrary::GetRightVector(FRotator(rotation.Pitch, 0, rotation.Yaw));
	AddMovementInput(rightVector, InputValue.X);

	const FVector& forwardVector = UKismetMathLibrary::GetRightVector(FRotator(0, 0, rotation.Yaw));
	AddMovementInput(forwardVector, InputValue.Y);*/
}

void AMainCharacter::InputActionInteract()
{
	if (InteractableObject != nullptr)
	{
		InteractableObject->OnInteract();
	}
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

