#include "Actors/RPGFieldCharacter.h"

#include "Actors/Triggers/RPGTouchTrigger.h"
#include "RPGConstants.h"
#include "RPGSettings.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedActionKeyMapping.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

ARPGFieldCharacter::ARPGFieldCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
}

void ARPGFieldCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ARPGFieldCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UInputAction* InputMove = Cast<UInputAction>(InputMoveAssetPath.TryLoad());
    UInputAction* InputCamera = Cast<UInputAction>(InputCameraAssetPath.TryLoad());
    
	if (const URPGSettings* RPGSettings = GetDefault<URPGSettings>())
	{
		if (RPGSettings->InputMove)
		{
			InputMove = Cast<UInputAction>(RPGSettings->InputMove.LoadSynchronous());
        }
		if (RPGSettings->InputCamera)
		{
			InputCamera = Cast<UInputAction>(RPGSettings->InputCamera.LoadSynchronous());
		}
		/*if (RPGSettings->InputInteract)
		{
            InputInteract = Cast<UInputAction>(RPGSettings->InputInteract.LoadSynchronous());
		}*/
	}

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (InputCamera != nullptr)
		{
			Input->BindAction(InputCamera, ETriggerEvent::Triggered, this, &ARPGFieldCharacter::InputActionCamera);
		}

		if (InputMove != nullptr)
		{
			Input->BindAction(InputMove, ETriggerEvent::Triggered, this, &ARPGFieldCharacter::InputActionMove);
		}
	}
}

void ARPGFieldCharacter::SetInteractableTrigger(AActor* TriggerActor)
{
	InteractableTriggerActor = TriggerActor;
}

void ARPGFieldCharacter::Move(FVector2D MovementVector)
{
	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ARPGFieldCharacter::Look(FVector2D LookAxisVector)
{
	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ARPGFieldCharacter::InputActionMove(const FInputActionValue& Value)
{
	Move(Value.Get<FVector2D>());
}

void ARPGFieldCharacter::InputActionCamera(const FInputActionValue& Value)
{
    Look(Value.Get<FVector2D>());
}

void ARPGFieldCharacter::InputActionInteract()
{
	if (InteractableTriggerActor && InteractableTriggerActor->GetClass()->ImplementsInterface(UTriggerInterface::StaticClass()))
	{
		ITriggerInterface::Execute_ExecuteTrigger(InteractableTriggerActor);
    }
}