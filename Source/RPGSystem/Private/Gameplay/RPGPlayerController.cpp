#include "Gameplay/RPGPlayerController.h"

#include "Actors/RPGFieldCharacter.h"
#include "Data/RPGTypes.h"
#include "RPGConstants.h"
#include "RPGSettings.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
//#include "RPGFieldPartyManager.h"

ARPGPlayerController::ARPGPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	FieldInputMappingContext = Cast<UInputMappingContext>(FieldInputsMappingPath.TryLoad());
	InputInteract = Cast<UInputAction>(InputInteractAssetPath.TryLoad());

    UIInputMappingContext = Cast<UInputMappingContext>(UIInputsMappingPath.TryLoad());

}

void ARPGPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay: %s ControllerClass=%s IsLocal=%d"), *GetNameSafe(this), *GetNameSafe(GetClass()), IsLocalController());

	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
}

void ARPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	const URPGSettings* RPGSettings = GetDefault<URPGSettings>();
	if (!RPGSettings)
	{
		return;
	}		

	if (RPGSettings->InputMappingFieldControls)
	{
		FieldInputMappingContext = Cast<UInputMappingContext>(RPGSettings->InputMappingFieldControls.LoadSynchronous());
	}

	//if (RPGSettings->InputMappingUIControls)
	{
		UIInputMappingContext = Cast<UInputMappingContext>(RPGSettings->InputMappingUIControls.LoadSynchronous());
	}
	
	if (RPGSettings->InputInteract)
	{
		InputInteract = Cast<UInputAction>(RPGSettings->InputInteract.LoadSynchronous());
	}

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInput)
	{
		return;
	}
	
	if (InputInteract)
	{
		EnhancedInput->BindAction(InputInteract, ETriggerEvent::Started, this, &ARPGPlayerController::HandleInteract);
	}

    SetControlMode(EControlMode::Field);

	

	/*if (SwapLeaderAction)
	{
		EnhancedInput->BindAction(
			SwapLeaderAction,
			ETriggerEvent::Started,
			this,
			&ARPGPlayerController::HandleSwapLeader
		);
	}*/
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
			TArray<FKey> Keys = Subsystem->QueryKeysMappedToAction(InputInteract);

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
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Interact pressed"));
	}

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
	 ARPGFieldCharacter* FieldCharacter = Cast<ARPGFieldCharacter>(ControlledPawn);
	 if (FieldCharacter)
	 {
	     FieldCharacter->InputActionInteract();
	 }
}

void ARPGPlayerController::SetControlMode(EControlMode ControlMode)
{
	if (ActiveControlMode == ControlMode)
	{
		return;
	}

	const URPGSettings* RPGSettings = GetDefault<URPGSettings>();
	if (!RPGSettings)
	{
		return;
    }

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInput)
	{
		return;
	}
	
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (CurrentInputMappingContext)
			{
				Subsystem->RemoveMappingContext(CurrentInputMappingContext);
			}

			switch (ControlMode)
			{
			case EControlMode::Field:
				// Set field control input mapping context
				Subsystem->AddMappingContext(FieldInputMappingContext, 0);
				CurrentInputMappingContext = FieldInputMappingContext;
                break;
            case EControlMode::UI:
                // Set UI control input mapping context
                Subsystem->AddMappingContext(UIInputMappingContext, 0);
                CurrentInputMappingContext = UIInputMappingContext;
                break;
			}
		}
	}
}

ARPGPlayerController* ARPGPlayerController::GetPlayerController(UObject* WorldContextObject)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		return Cast<ARPGPlayerController>(World->GetFirstPlayerController());
	}
    return nullptr;
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