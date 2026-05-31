#include "Commands/ShowText.h"

#include "Actors/RPGFieldCharacter.h"
#include "Actors/MapEventActor.h"
#include "Data/RPGTypes.h"
#include "Gameplay/RPGPlayerController.h"
#include "Interface/MessageInterface.h"
#include "MapEvent.h"
#include "RPGSettings.h"
#include "UI/RPGHUD.h"
#include "UI/RPGMessageWidget.h"

#include "GameFramework/HUD.h"
#include "Kismet/GameplayStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ShowText)

UUserWidget* UShowText::GetMessageWidget(ARPGFieldCharacter* InstigatorActor) const
{
	if (APlayerController* Player = UGameplayStatics::GetPlayerController(InstigatorActor->GetWorld(), 0))
	{
		if (ARPGHUD* HUD = Cast<ARPGHUD>(Player->GetHUD()))
		{
			return HUD->CreateOrGetMessageWidget();
		}
	}
    return nullptr;
}

ERPGCommandResult UShowText::Execute_Implementation(ARPGTouchTrigger* Trigger, ARPGFieldCharacter* InstigatorActor)
{
	UUserWidget* MessageWidget = GetMessageWidget(InstigatorActor);
	if (!MessageWidget)
	{
		return ERPGCommandResult::Abort;
    }

	if (ARPGPlayerController* Controller = ARPGPlayerController::GetPlayerController(InstigatorActor))
	{
		InstigatorActor->DisableInput(Controller);

		FInputModeUIOnly InputModeData;
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputModeData.SetWidgetToFocus(MessageWidget->GetCachedWidget());

		Controller->SetControlMode(EControlMode::UI);
	}
		//renable input
		/*APawn* ControlledPawn = GetPawn();
    if (ControlledPawn)
    {
        ControlledPawn->EnableInput(this);
    }

    FInputModeGameOnly InputModeData;
    SetInputMode(InputModeData);

    bShowMouseCursor = false;*/

	IMessageInterface::Execute_ShowText(MessageWidget, this);
	return ERPGCommandResult::Continue;
}

