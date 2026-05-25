#include "Level/RPGMapScenario.h"

#include "Gameplay/RPGProgressionSubsystem.h"
#include "Actors/Spawners/RPGFieldSpawner.h"

#if WITH_EDITOR
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetToolsModule.h"
#include "EngineUtils.h"
#include "Factories/DataAssetFactory.h"
#include "Logging/MessageLog.h"
#include "Misc/UObjectToken.h"
#endif

ARPGMapScenario::ARPGMapScenario()
{
    PrimaryActorTick.bCanEverTick = false;

	if (ActiveScenario)
	{
		ScenarioId = ActiveScenario->ScenarioId;
	}

}

void ARPGMapScenario::BeginPlay()
{
    Super::BeginPlay();

	URPGProgressionSubsystem* Progression = GetGameInstance()->GetSubsystem<URPGProgressionSubsystem>();
	if (Progression == nullptr)
	{
		return;
	}

	Progression->InitializeFromMapScenario(this);	

	// Resolve runtime active scenario.
	if (!RuntimeActiveScenario)
	{
		for (URPGScenario* Scenario : AvailableScenarios)
		{
			if (Scenario && Scenario->ScenarioId == Progression->CurrentScenarioId)
			{
				RuntimeActiveScenario = Scenario;
				break;
			}
		}

		// Fallback.
		if (!RuntimeActiveScenario && AvailableScenarios.Num() > 0)
		{
			RuntimeActiveScenario = AvailableScenarios[0];
		}
	}

	if (ARPGFieldSpawner* SpawnManager = GetSpawner())
	{
		SpawnManager->SetFolderPath(TEXT("Scenario"));

		//SpawnManager->SetMapScenario(this);
		SpawnManager->InitializeField(RuntimeActiveScenario);
	}

}

ARPGFieldSpawner* ARPGMapScenario::GetSpawner() const
{
	for (TActorIterator<ARPGFieldSpawner> It(GetWorld()); It; ++It)
	{
		return *It;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	return GetWorld()->SpawnActor<ARPGFieldSpawner>(
		ARPGFieldSpawner::StaticClass(),
		GetActorLocation(),
		GetActorRotation(),
		Params
	);
}

void ARPGMapScenario::StartScenario()
{

}

void ARPGMapScenario::ContinueScenario()
{

}

ARPGTouchTrigger* ARPGMapScenario::GetTriggerByData(URPGTriggerData* TriggerData) const
{
    return nullptr;
}

void ARPGMapScenario::SetActiveArea(FName AreaId)
{

}

#if WITH_EDITOR

void ARPGMapScenario::CreateNewScenarioAsset()
{
	const FString AssetName = FString::Printf(
		TEXT("DA_%s_SCN"),
		*ScenarioId.ToString()
	);

	UDataAssetFactory* Factory = NewObject<UDataAssetFactory>();
	Factory->DataAssetClass = URPGScenario::StaticClass();

	FAssetToolsModule& AssetTools =
		FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");

	UObject* NewAsset = AssetTools.Get().CreateAsset(
		AssetName,
		ScenarioAssetFolder,
		URPGScenario::StaticClass(),
		Factory
	);

	URPGScenario* NewScenario = Cast<URPGScenario>(NewAsset);
	if (!NewScenario)
	{
		return;
	}

	NewScenario->ScenarioId = FName(*AssetName);
	NewScenario->ScenarioName = FText::FromString(AssetName);
	NewScenario->ScenarioFolder = ScenarioAssetFolder / AssetName;

	AvailableScenarios.AddUnique(NewScenario);
	ActiveScenario = NewScenario;

	NewScenario->MarkPackageDirty();

	Modify();
	MarkPackageDirty();
}

void ARPGMapScenario::PostActorCreated()
{
	Super::PostActorCreated();

	if (!GetWorld())
	{
		return;
	}

	// Ensure only one exists per level.
	for (TActorIterator<ARPGMapScenario> It(GetWorld()); It; ++It)
	{
		ARPGMapScenario* Existing = *It;

		if (Existing && Existing != this)
		{
			UE_LOG(
				LogTemp,
				Error,
				TEXT("Only one ARPGMapScenario is allowed per level.")
			);

			Destroy();
			return;
		}
	}

	SetFolderPath(TEXT("Scenario"));
	SetActorLabel(TEXT("RPG_MapScenario"));

	Modify();
	MarkPackageDirty();
}

void ARPGMapScenario::RefreshScenarios()
{
	AvailableScenarios.Empty();

	FAssetRegistryModule& AssetRegistryModule =
		FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	TArray<FAssetData> AssetDataList;

	AssetRegistryModule.Get().GetAssetsByPath(
		FName(*ScenarioAssetFolder),
		AssetDataList,
		true // recursive
	);

	for (const FAssetData& AssetData : AssetDataList)
	{
		if (!AssetData.GetClass()->IsChildOf(URPGScenario::StaticClass()))
		{
			continue;
		}

		URPGScenario* Scenario = Cast<URPGScenario>(AssetData.GetAsset());
		if (Scenario)
		{
			AvailableScenarios.AddUnique(Scenario);
		}
	}

	Modify();
	MarkPackageDirty();
}

#endif //WITH_EDITOR