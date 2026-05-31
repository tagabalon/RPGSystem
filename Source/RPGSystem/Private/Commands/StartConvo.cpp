#include "Commands/StartConvo.h"

#include "Actors/Triggers/RPGTouchTrigger.h"
#include "Actors/RPGFieldCharacter.h"
#include "Data/RPGConvoAsset.h"
#include "Data/RPGTypes.h"
#include "Gameplay/RPGPlayerController.h"
#include "Gameplay/RPGTriggerRunnerSubsystem.h"
#include "UI/RPGHUD.h"
#include "UI/RPGMessageWidget.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

UUserWidget* UStartConvo::GetMessageWidget(ARPGFieldCharacter* InstigatorActor) const
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

ERPGCommandResult UStartConvo::Execute_Implementation(ARPGTouchTrigger* Trigger, ARPGFieldCharacter* InstigatorActor)
{
	MessageWidget = GetMessageWidget(InstigatorActor);
	if (!MessageWidget)
	{
		return ERPGCommandResult::Abort;
	}

	RuntimeConvo = ConvoAsset ? ConvoAsset->FindConvo(ConvoId) : nullptr;
	CurrentLineIndex = 0;

    Instigator = InstigatorActor;

	if (ARPGPlayerController* Controller = ARPGPlayerController::GetPlayerController(InstigatorActor))
	{
		InstigatorActor->DisableInput(Controller);

		FInputModeUIOnly InputModeData;
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputModeData.SetWidgetToFocus(MessageWidget->GetCachedWidget());

		Controller->SetControlMode(EControlMode::UI);
	}

	if (!RuntimeConvo)
	{
		return ERPGCommandResult::Continue;
	}

	ShowCurrentLine();
	return ERPGCommandResult::Wait;
}

ERPGCommandResult UStartConvo::Continue_Implementation()
{
	CurrentLineIndex++;

	if (!RuntimeConvo || CurrentLineIndex >= RuntimeConvo->Lines.Num())
	{
		RuntimeConvo = nullptr;

		if (UGameInstance* GameInstance = Instigator->GetGameInstance())
		{
			if (URPGTriggerRunnerSubsystem* TriggerRunner = GameInstance->GetSubsystem<URPGTriggerRunnerSubsystem>())
			{
				TriggerRunner->ContinueTrigger();
			}

		}
		IMessageInterface::Execute_CloseMessages(MessageWidget);



		return ERPGCommandResult::Continue;
	}

	ShowCurrentLine();
	return ERPGCommandResult::Wait;
}

void UStartConvo::ShowCurrentLine()
{
	if (!RuntimeConvo/* || !RuntimeGameInstance*/)
	{
		return;
	}

    const FRPGConvoLine& Line = RuntimeConvo->Lines[CurrentLineIndex];

    FRPGMessageData MessageData;
    MessageData.Speaker = Line.Speaker.ToString();
    MessageData.Message = Line.Message.ToString();
    MessageData.BustSprite = Line.BustSprite;
    MessageData.VoiceAudio = Line.VoiceAudio;
    MessageData.Invoker = this;

	IMessageInterface::Execute_ShowMessage(MessageWidget, MessageData);

	if (Line.CharacterId != NAME_None)
	{
		if (UAudioComponent* AudioComponent = Instigator->GetComponentByClass<UAudioComponent>())
		{
			AudioComponent->SetSound(Line.VoiceAudio);
            AudioComponent->Play();
		}
    }
}

TArray<FName> UStartConvo::GetConvoOptions() const
{
	TArray<FName> Options;
	if (ConvoAsset)
	{
		for (const FRPGConvo& Convo : ConvoAsset->Convos)
		{
			Options.Add(Convo.ConvoId);
		}
	}
    return Options;
}