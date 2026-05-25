#include "RPGFieldGameMode.h"

#include "Gameplay/RPGPlayerController.h"

ARPGFieldGameMode::ARPGFieldGameMode()
{
    DefaultPawnClass = nullptr;
    PlayerControllerClass = ARPGPlayerController::StaticClass();
}