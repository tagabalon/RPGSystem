#include "RPGFieldGameMode.h"

#include "Gameplay/RPGPlayerController.h"
#include "RPGSettings.h"
#include "UI/RPGHUD.h"

ARPGFieldGameMode::ARPGFieldGameMode()
{
    DefaultPawnClass = nullptr;
    PlayerControllerClass = ARPGPlayerController::StaticClass();
    HUDClass = ARPGHUD::StaticClass();
}

void ARPGFieldGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    // Call the base class implementation first
    Super::InitGame(MapName, Options, ErrorMessage);

    if (const URPGSettings* Settings = GetDefault<URPGSettings>())
    {
        if (Settings->RPGHUDClass.IsValid())
        {
            // Safely load the Blueprint HUD class at runtime
            if (UClass* LoadedHUDClass = Settings->RPGHUDClass.TryLoadClass<ARPGHUD>())
            {
                // Assign it to the Game Mode's default HUD class
                HUDClass = LoadedHUDClass;
            }
        }
    }
}