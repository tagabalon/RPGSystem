#include "Gameplay/RPGProgressionSubsystem.h"

#include "Level/RPGMapScenario.h"

#include "Kismet/GameplayStatics.h"

void URPGProgressionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	SessionStartTime = FDateTime::Now();
	//CurrentScenario = nullptr;
	CurrentScenarioId = NAME_None;
	CurrentMapId = NAME_None;
	CurrentLevelPackageName.Empty();
}

void URPGProgressionSubsystem::Deinitialize()
{
	//CurrentScenario = nullptr;

	Super::Deinitialize();
}

void URPGProgressionSubsystem::InitializeFromMapScenario(ARPGMapScenario* MapScenario)
{
	if (!MapScenario)
	{
		return;
	}

	UWorld* World = MapScenario->GetWorld();
	if (World)
	{
		CurrentLevelPackageName = World->GetMapName();

		// Removes PIE prefixes like UEDPIE_0_
		CurrentMapId = FName(*UGameplayStatics::GetCurrentLevelName(World, true));
	}

	SetCurrentScenario(MapScenario->GetScenario());
}

void URPGProgressionSubsystem::SetCurrentScenario(URPGScenario* Scenario)
{
	//CurrentScenario = Scenario;

	if (Scenario)
	{
		CurrentScenarioId = Scenario->ScenarioId;
	}
	else
	{
		CurrentScenarioId = NAME_None;
	}
}