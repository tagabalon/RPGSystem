#include "RPGGameInstance.h"

#include "Data/RPGDatabase.h"
#include "Gameplay/RPGPartySubsystem.h"
#include "Progression/RPGSaveGame.h"
#include "RPGSettings.h"

#include "Kismet/GameplayStatics.h"

void URPGGameInstance::Init()
{
	Super::Init();

	LoadDatabase();

	Party = GetSubsystem<URPGPartySubsystem>();
    Party->InitializeNewGame(Database);
}

void URPGGameInstance::Shutdown()
{
	CurrentSaveGame = nullptr;
	Database = nullptr;

	Super::Shutdown();
}

URPGGameInstance* URPGGameInstance::GetRPGGameInstance(const UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	return Cast<URPGGameInstance>(GameInstance);
}

URPGDatabase* URPGGameInstance::LoadDatabase()
{
	if (Database == nullptr)
	{
		if (const URPGSettings* Settings = GetDefault<URPGSettings>())
		{
			Database = Settings->GameDatabaseAsset.LoadSynchronous();
		}
	}	
	return Database;
}

URPGDatabase* URPGGameInstance::GetDatabase() const
{
	return Database;
}

void URPGGameInstance::SetPausedState(bool bPaused)
{
	bIsPaused = bPaused;

	if (UWorld* World = GetWorld())
	{
		UGameplayStatics::SetGamePaused(World, bPaused);
	}
}

void URPGGameInstance::SetLoadingState(bool bLoading)
{
	bIsLoading = bLoading;
}

void URPGGameInstance::StorePlayerTransform(const FVector& Position, const FRotator& Rotation)
{
	LastPlayerPosition = Position;
	LastPlayerRotation = Rotation;
}

bool URPGGameInstance::SaveGameToSlot(const FString& SlotName, int32 UserIndex)
{
	if (!CurrentSaveGame)
	{
		CurrentSaveGame = Cast<URPGSaveGame>(
			UGameplayStatics::CreateSaveGameObject(URPGSaveGame::StaticClass())
		);
	}

	if (!CurrentSaveGame)
	{
		return false;
	}

	CurrentSaveGame->SavedMapName = CurrentMapName;
	CurrentSaveGame->SavedPlayerPosition = LastPlayerPosition;
	CurrentSaveGame->SavedPlayerRotation = LastPlayerRotation;

	return UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, UserIndex);
}

bool URPGGameInstance::LoadGameFromSlot(const FString& SlotName, int32 UserIndex)
{
	USaveGame* LoadedSave = UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex);
	CurrentSaveGame = Cast<URPGSaveGame>(LoadedSave);

	if (!CurrentSaveGame)
	{
		return false;
	}

	CurrentMapName = CurrentSaveGame->SavedMapName;
	LastPlayerPosition = CurrentSaveGame->SavedPlayerPosition;
	LastPlayerRotation = CurrentSaveGame->SavedPlayerRotation;

	return true;
}