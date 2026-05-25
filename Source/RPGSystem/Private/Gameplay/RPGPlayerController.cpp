#include "Gameplay/RPGPlayerController.h"

#include "RPGSettings.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
//#include "RPGFieldPartyManager.h"

void ARPGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (const URPGSettings* RPGSettings = GetDefault<URPGSettings>())
			{
				if (RPGSettings->InputMapping)
				{
					Subsystem->AddMappingContext(RPGSettings->InputMapping.LoadSynchronous(), FieldMappingPriority);
				}
			}
		}
	}

	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
}

void ARPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);

	if (!EnhancedInput)
	{
		return;
	}

	if (InteractAction)
	{
		EnhancedInput->BindAction(
			InteractAction,
			ETriggerEvent::Started,
			this,
			&ARPGPlayerController::HandleInteract
		);
	}

	if (SwapLeaderAction)
	{
		EnhancedInput->BindAction(
			SwapLeaderAction,
			ETriggerEvent::Started,
			this,
			&ARPGPlayerController::HandleSwapLeader
		);
	}

	if (PauseAction)
	{
		EnhancedInput->BindAction(
			PauseAction,
			ETriggerEvent::Started,
			this,
			&ARPGPlayerController::HandlePause
		);
	}
}

void ARPGPlayerController::DebugInputState()
{
	UE_LOG(LogTemp, Warning, TEXT("=== Input Debug ==="));
	UE_LOG(LogTemp, Warning, TEXT("Pawn: %s"), *GetNameSafe(GetPawn()));
	UE_LOG(LogTemp, Warning, TEXT("InputComponent: %s"), *GetNameSafe(InputComponent));
	UE_LOG(LogTemp, Warning, TEXT("IsLocalController: %d"), IsLocalController());
	UE_LOG(LogTemp, Warning, TEXT("IgnoreMoveInput: %d"), IsMoveInputIgnored());
	UE_LOG(LogTemp, Warning, TEXT("IgnoreLookInput: %d"), IsLookInputIgnored());
	UE_LOG(LogTemp, Warning, TEXT("IsInputKeyDown W: %d"), IsInputKeyDown(EKeys::W));

	if (ULocalPlayer* LP = GetLocalPlayer())
	{
		if (auto* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			TArray<FKey> Keys = Subsystem->QueryKeysMappedToAction(InteractAction);

			for (const FKey& Key : Keys)
			{
				UE_LOG(LogTemp, Warning, TEXT("Interact mapped to: %s"), *Key.ToString());
			}
		}
	}
}


//void ARPGPlayerController::SetFieldPartyManager(ARPGFieldPartyManager* InManager)
//{
//	FieldPartyManager = InManager;
//}

bool ARPGPlayerController::GetKeyForInputAction(const UInputAction* InputAction, FKey& OutKey) const
{
	if (!InputAction)
	{
		return false;
	}

	const ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!LocalPlayer)
	{
		return false;
	}

	const UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!Subsystem)
	{
		return false;
	}

	const TArray<FKey> Keys = Subsystem->QueryKeysMappedToAction(InputAction);

	if (Keys.Num() <= 0)
	{
		return false;
	}

	OutKey = Keys[0];
	return true;
}

void ARPGPlayerController::HandleInteract()
{
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		return;
	}

	// Let the currently possessed field character handle nearby trigger interaction.
	if (ControlledPawn->GetClass()->ImplementsInterface(nullptr))
	{
		// Placeholder if you later add an interaction interface.
	}

	// Simpler route for now:
	// Cast to your ARPGFieldCharacter and call Interact().
	// ARPGFieldCharacter* FieldCharacter = Cast<ARPGFieldCharacter>(ControlledPawn);
	// if (FieldCharacter)
	// {
	//     FieldCharacter->Interact();
	// }
}

void ARPGPlayerController::HandleSwapLeader()
{
	/*if (FieldPartyManager)
	{
		FieldPartyManager->CycleLeader();
	}*/
}

void ARPGPlayerController::HandlePause()
{
	SetPause(!IsPaused());
}