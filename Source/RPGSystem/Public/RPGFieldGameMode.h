#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RPGFieldGameMode.generated.h"

UCLASS()
class RPGSYSTEM_API ARPGFieldGameMode : public AGameModeBase
{
	GENERATED_BODY()

	ARPGFieldGameMode();

    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
};
