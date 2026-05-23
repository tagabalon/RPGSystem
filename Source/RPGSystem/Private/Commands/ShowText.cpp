#include "Commands/ShowText.h"

#include "Actors/MapEventActor.h"
#include "Interface/MessageInterface.h"
#include "RPGSettings.h"
#include "MapEvent.h"

#include "GameFramework/HUD.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ShowText)

void UShowText::Execute(APlayerController* Player, AMapEventActor* MapEventActor)
{
	if (AHUD* HUD = Player->GetHUD())
	{
		if (HUD->GetClass()->ImplementsInterface(UMessageInterface::StaticClass()))
		{
			IMessageInterface::Execute_ShowText(HUD, this);
		}
	}
}