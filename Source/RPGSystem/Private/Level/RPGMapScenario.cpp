#include "Level/RPGMapScenario.h"

#include "Actors/Spawners/RPGFieldSpawner.h"
#include "Actors/Triggers/RPGNPC.h"
#include "Actors/Triggers/RPGTrigger.h"
#include "Actors/Triggers/RPGTouchTrigger.h"
#include "Data/RPGTriggerData.h"
#include "Gameplay/RPGProgressionSubsystem.h"
#include "RPGSettings.h"

#include "GameFramework/PlayerStart.h"

#if WITH_EDITOR
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetToolsModule.h"
#include "Editor.h"
#include "EngineUtils.h"
#include "Factories/DataAssetFactory.h"
#include "Logging/MessageLog.h"
#include "Misc/UObjectToken.h"
#include "LevelEditor.h"
#include "Selection.h"
#include "AssetSelection.h"
#include "SLevelViewport.h"
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
        TArray<TObjectPtr<URPGScenario>> AllScenarios = GetAllScenarios();
		for (URPGScenario* Scenario : AllScenarios)
		{
			if (Scenario && Scenario->ScenarioId == Progression->CurrentScenarioId)
			{
				RuntimeActiveScenario = Scenario;
				break;
			}
		}

		// Fallback.
		if (!RuntimeActiveScenario && AllScenarios.Num() > 0)
		{
			RuntimeActiveScenario = AllScenarios[0];
		}
	}

    SpawnManager = GetSpawner();
	if (SpawnManager)
	{
		SpawnManager->SetFolderPath(TEXT("RPGSystem"));

		if (UWorld* World = GetWorld())
		{
			if (World->IsPlayInEditor())
			{
                SpawnManager->SetStartingLocation(DebugStartPoint);
			}
		}

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

	return GetWorld()->SpawnActor<ARPGFieldSpawner>(ARPGFieldSpawner::StaticClass(), GetActorLocation(), GetActorRotation(), Params);
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

AActor* ARPGMapScenario::GetTriggerActorById(FName TriggerId) const
{
	if (SpawnManager)
	{
        return SpawnManager->GetTriggerActorById(TriggerId);
	}
    return nullptr;
}

void ARPGMapScenario::SetActiveArea(FName AreaId)
{

}
TArray<FName> ARPGMapScenario::GetScenarioIds() const
{
    TArray<TObjectPtr<URPGScenario>> AllScenarios = GetAllScenarios();

	TArray<FName> Ids;
    Ids.Add(FName("None"));

	for (URPGScenario* Scenario : AllScenarios)
	{
		if (Scenario)
		{
			Ids.Add(Scenario->ScenarioId);
		}
	}
    return Ids;
}


TArray<TObjectPtr<URPGScenario>> ARPGMapScenario::GetAllScenarios() const
{
	TArray<TObjectPtr<URPGScenario>> AllScenarios;

    FString ScenarioAssetFolder = GetScenariosFolder();
	FAssetRegistryModule& AssetRegistryModule =	FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AssetDataList;

	AssetRegistryModule.Get().GetAssetsByPath(FName(*ScenarioAssetFolder), AssetDataList, true);

	for (const FAssetData& AssetData : AssetDataList)
	{
		if (!AssetData.GetClass()->IsChildOf(URPGScenario::StaticClass()))
		{
			continue;
		}

		URPGScenario* Scenario = Cast<URPGScenario>(AssetData.GetAsset());
		if (Scenario)
		{
			AllScenarios.AddUnique(Scenario);
		}
	}

    return AllScenarios;
}

TObjectPtr<URPGScenario> ARPGMapScenario::GetScenarioWithId(FName Id) const
{
	TArray<TObjectPtr<URPGScenario>> AllScenarios = GetAllScenarios();
	for (URPGScenario* Scenario : AllScenarios)
	{
		if (Scenario && Scenario->ScenarioId == Id)
		{
			return Scenario;
		}
	}
    return nullptr;
}

void ARPGMapScenario::SyncTriggers()
{
	if (!ActiveScenario)
	{
		return;
    }

#if WITH_EDITORONLY_DATA
	ScenarioTriggers.Empty();
#endif // WITH_EDITORONLY_DATA

	for (URPGTriggerData* TriggerData : ActiveScenario->TriggerList)
	{
		if (!TriggerData)
		{
			continue;
		}

		if (TriggerData->States.Num() == 0)
			continue;

		const FRPGTriggerState* ActiveState = &TriggerData->States[0];
		if (ActiveState == nullptr)
			continue;

		if (ActiveState->CharacterMesh)
		{
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			if (ARPGNPC* NPC = GetWorld()->SpawnActor<ARPGNPC>(ARPGNPC::StaticClass(), TriggerData->WorldPosition, TriggerData->WorldRotation, Params))			
			{
				NPC->GetMesh()->SetSkeletalMesh(ActiveState->CharacterMesh);
				if (ActiveState->CharacterAnimation)
				{
					NPC->GetMesh()->SetAnimInstanceClass(ActiveState->CharacterAnimation->GeneratedClass);
				}
				NPC->SetFolderPath(ScenarioId);

				ITriggerInterface::Execute_SetTriggerData(NPC, TriggerData);

#if WITH_EDITORONLY_DATA
                ScenarioTriggers.Add(NPC);
#endif // WITH_EDITORONLY_DATA
			}
		}
		else if (ActiveState->TriggerActivation == ERPGTriggerActivation::PlayerTouch || ActiveState->TriggerActivation == ERPGTriggerActivation::TriggerTouch)
		{
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			if (ARPGTouchTrigger* TouchTrigger = GetWorld()->SpawnActor<ARPGTouchTrigger>(ARPGTouchTrigger::StaticClass(), TriggerData->WorldPosition, TriggerData->WorldRotation, Params))
			{
				TouchTrigger->SetFolderPath(ScenarioId);
#if WITH_EDITORONLY_DATA
				ScenarioTriggers.Add(TouchTrigger);
#endif // WITH_EDITORONLY_DATA
			}
		}
		else
		{
			SpawnTrigger(TriggerData);
		}
	}
}

#if WITH_EDITOR

FString ARPGMapScenario::GetScenariosFolder() const
{
	if (const URPGSettings* Settings = GetDefault<URPGSettings>())
	{
		if (!Settings->ScenarioFolder.IsEmpty())
		{
			return Settings->ScenarioFolder;
		}
		else
		{
			FMessageLog("RPGSystem").Error()
				->AddToken(FTextToken::Create(
					FText::FromString(
						"Scenario folder is not set in RPGSettings. Please set it before creating a new scenario asset."
					)
				))
				->AddToken(FUObjectToken::Create(const_cast<URPGSettings*>(Settings)))
				->AddToken(FTextToken::Create(FText::FromString(" (Edit RPGSettings)")));
		}
	}
	return TEXT("/Plugins/RPGSystem/Data/Scenarios");
}

void ARPGMapScenario::CreateNewScenarioAsset()
{
	FString ScenarioAssetFolder = GetScenariosFolder();

	const FString AssetName = FString::Printf(TEXT("DA_%s_SCN"), *NewScenarioId.ToString());

	UDataAssetFactory* Factory = NewObject<UDataAssetFactory>();
	Factory->DataAssetClass = URPGScenario::StaticClass();

	FAssetToolsModule& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");

	UObject* NewAsset = AssetTools.Get().CreateAsset(AssetName, ScenarioAssetFolder, URPGScenario::StaticClass(), Factory);

	URPGScenario* NewScenario = Cast<URPGScenario>(NewAsset);
	if (!NewScenario)
	{
		return;
	}

	NewScenario->ScenarioId = NewScenarioId;
	NewScenario->ScenarioName = FText::FromName(NewScenarioId);
	NewScenario->ScenarioFolder = ScenarioAssetFolder / AssetName;

	ActiveScenario = NewScenario;

	NewScenario->MarkPackageDirty();

	Modify();
	MarkPackageDirty();
}

void ARPGMapScenario::AddTriggerActor()
{
	if (!ActiveScenario)
	{
		return;
	}

	FString ScenarioFolder = GetScenariosFolder().Append("/").Append(*ActiveScenario->ScenarioId.ToString());

    int32 TriggerCount = ActiveScenario->TriggerList.Num() + 1;
    const FName TriggerId = FName(*FString::Printf(TEXT("TRG%03d"), TriggerCount));
	const FString AssetName = FString::Printf(TEXT("DA%s_%s"), *ActiveScenario->ScenarioId.ToString(), *TriggerId.ToString());

	UDataAssetFactory* Factory = NewObject<UDataAssetFactory>();
	Factory->DataAssetClass = URPGTriggerData::StaticClass();

	FAssetToolsModule& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");

	UObject* NewAsset = AssetTools.Get().CreateAsset(AssetName, ScenarioFolder, URPGTriggerData::StaticClass(), Factory);

	URPGTriggerData* NewTriggerData = Cast<URPGTriggerData>(NewAsset);
	NewTriggerData->TriggerId = TriggerId;

	//Spawn in the middle of the level editor
	FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>("LevelEditor");
	TSharedPtr<ILevelEditor> FirstLevelEditor = LevelEditorModule.GetFirstLevelEditor();

	if (FirstLevelEditor.IsValid())
	{
		TSharedPtr<SLevelViewport> ActiveViewport = FirstLevelEditor->GetActiveViewportInterface();
		if (ActiveViewport.IsValid())
		{
			FEditorViewportClient* ActiveViewportClient = ActiveViewport->GetViewportClient().Get();
			if (ActiveViewportClient)
			{
				// 2. Get Camera Location and Rotation
				FVector CameraLocation = ActiveViewportClient->GetViewLocation();
				FRotator CameraRotation = ActiveViewportClient->GetViewRotation();

				// 3. Calculate Spawn Position (e.g., 500 units in front of the camera)
				NewTriggerData->WorldPosition = CameraLocation + (CameraRotation.Vector() * 500.0f);
				NewTriggerData->WorldRotation = FRotator::ZeroRotator;
			}
		}
	}

	ActiveScenario->TriggerList.Add(NewTriggerData);
	ActiveScenario->MarkPackageDirty();

    SpawnTrigger(NewTriggerData);

	Modify();
    MarkPackageDirty();
}

ARPGTrigger* ARPGMapScenario::SpawnTrigger(URPGTriggerData* TriggerData)
{
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//Spawn the actor
	if (ARPGTrigger* TriggerActor = GetWorld()->SpawnActor<ARPGTrigger>(ARPGTrigger::StaticClass(), TriggerData->WorldPosition, TriggerData->WorldRotation, Params))
	{
        TriggerActor->SetActorLabel(TriggerData->TriggerId.ToString());
		TriggerActor->SetFolderPath(ActiveScenario->ScenarioId);

		ITriggerInterface::Execute_SetTriggerData(TriggerActor, TriggerData);

#if WITH_EDITORONLY_DATA
		ScenarioTriggers.Add(TriggerActor);
#endif // WITH_EDITORONLY_DATA

        return TriggerActor;
	}

	return nullptr;
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
			UE_LOG(LogTemp, Error, TEXT("Only one ARPGMapScenario is allowed per level."));

			Destroy();
			return;
		}
	}

	SetFolderPath(TEXT("RPGSystem"));
	SetActorLabel(TEXT("RPG_MapScenario"));

	Modify();
	MarkPackageDirty();


}

void ARPGMapScenario::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Ensure the property pointer is valid
	if (PropertyChangedEvent.Property != nullptr)
	{
		// Get the name of the property that the user just modified
		FName ChangedPropertyName = PropertyChangedEvent.GetPropertyName();

		// Check if it matches our GetOptions string property
		if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(ARPGMapScenario, ScenarioId))
		{
			if (ScenarioId == FName("None"))
			{
                ActiveScenario = nullptr;
            }
			else
			{
                ActiveScenario = GetScenarioWithId(ScenarioId);
			}
		}
	}
}

#endif //WITH_EDITOR