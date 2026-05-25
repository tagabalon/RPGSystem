#include "Actors/Spawners/RPGFieldSpawner.h"

#include "Actors/RPGFieldCharacter.h"
#include "Actors/Triggers/RPGTouchTrigger.h"
#include "Data/RPGCharacters.h"
#include "Data/RPGScenario.h"
#include "Gameplay/RPGPartySubsystem.h"
#include "Level/RPGMapScenario.h"

//#include "AIController.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"

ARPGFieldSpawner::ARPGFieldSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARPGFieldSpawner::BeginPlay()
{
	Super::BeginPlay();
}

void ARPGFieldSpawner::InitializeField(URPGScenario* RuntimeScenario)
{
	SpawnParty();
	ActivateScenarioActors(RuntimeScenario);
}

void ARPGFieldSpawner::SpawnParty()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance || !CharactersDatabase)
	{
		return;
	}

	URPGPartySubsystem* PartySubsystem = GameInstance->GetSubsystem<URPGPartySubsystem>();
	if (!PartySubsystem)
	{
		return;
	}

	TArray<TSharedPtr<FRPGPartyMember>> PartyMembers = PartySubsystem->GetPartyMembers();
	if (PartyMembers.Num() > 0)
	{
		for (int32 i = 0; i < PartyMembers.Num(); ++i)
		{
			SpawnPartyMember(PartyMembers[i], i);
		}

		SetLeader(PartyMembers[0]->CharacterId);
	}
}

ARPGFieldCharacter* ARPGFieldSpawner::SpawnPartyMember(TSharedPtr<FRPGPartyMember>& PartyMember, int32 PartyIndex)
{
	if (PartyMember->CharacterId.IsNone() || SpawnedPartyMembers.Contains(PartyMember->CharacterId))
	{
		return nullptr;
	}

	FRPGCharacterData CharacterData;
	if (!CharactersDatabase->GetCharacterData(PartyMember->CharacterId, CharacterData))
	{
		return nullptr;
    }

	if (CharacterData.FieldCharacterClass == nullptr)
	{
        return nullptr;
	}

	UClass* FieldActorClass = CharacterData.FieldCharacterClass.LoadSynchronous();

	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();

	if (PartySpawnPoints.IsValidIndex(PartyIndex) && PartySpawnPoints[PartyIndex])
	{
		SpawnLocation = PartySpawnPoints[PartyIndex]->GetActorLocation();
		SpawnRotation = PartySpawnPoints[PartyIndex]->GetActorRotation();
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ARPGFieldCharacter* SpawnedCharacter = GetWorld()->SpawnActor<ARPGFieldCharacter>(
			FieldActorClass,
			SpawnLocation,
			SpawnRotation,
			Params
		);

	if (!SpawnedCharacter)
	{
		return nullptr;
	}

	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (PC && SpawnedCharacter)
	{
		PC->Possess(SpawnedCharacter);
		PC->SetViewTarget(SpawnedCharacter);
		PC->ResetIgnoreInputFlags();
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;

		UE_LOG(LogTemp, Warning, TEXT("Possessed: %s"), *GetNameSafe(PC->GetPawn()));
	}

	SpawnedCharacter->SetPartyMemberData(PartyMember);
	//SpawnedCharacter->SpawnDefaultController();

	SpawnedPartyMembers.Add(PartyMember->CharacterId, SpawnedCharacter);

	return SpawnedCharacter;
}

void ARPGFieldSpawner::SetLeader(FName ActorId)
{
	/*if (ActorId.IsNone() && SpawnedPartyMembers.Num() > 0)
	{
		for (const TPair<FName, TObjectPtr<ARPGFieldCharacter>>& Pair : SpawnedPartyMembers)
		{
			ActorId = Pair.Key;
			break;
		}
	}*/

	ARPGFieldCharacter* NewLeader = SpawnedPartyMembers.FindRef(ActorId);
	if (!NewLeader)
	{
		return;
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		return;
	}

	if (CurrentLeader/* && CurrentLeader != NewLeader*/)
	{
		//CurrentLeader->SetAsLeader(false);
		CurrentLeader->SpawnDefaultController();
	}

	/*if (AAIController* AIController = Cast<AAIController>(NewLeader->GetController()))
	{
		AIController->UnPossess();
	}*/

	CurrentLeader = NewLeader;
	//CurrentLeader->SetAsLeader(true);

	PlayerController->Possess(CurrentLeader);

	/*if (URPGPartySubsystem* PartySubsystem =
		GetGameInstance()->GetSubsystem<URPGPartySubsystem>())
	{
		PartySubsystem->SetCurrentLeader(ActorId);
	}

	RefreshFollowerTargets();*/
}

void ARPGFieldSpawner::CycleLeader()
{
	/*if (SpawnedPartyMembers.Num() <= 1)
	{
		return;
	}

	TArray<FName> ActorIds;
	SpawnedPartyMembers.GetKeys(ActorIds);

	const FName CurrentId = CurrentLeader ? CurrentLeader->ActorId : NAME_None;
	const int32 CurrentIndex = ActorIds.IndexOfByKey(CurrentId);

	const int32 NextIndex =
		(CurrentIndex == INDEX_NONE)
		? 0
		: (CurrentIndex + 1) % ActorIds.Num();

	SetLeader(ActorIds[NextIndex]);*/
}

void ARPGFieldSpawner::RefreshFollowerTargets()
{
	//for (const TPair<FName, TObjectPtr<ARPGFieldCharacter>>& Pair : SpawnedPartyMembers)
	//{
	//	ARPGFieldCharacter* Character = Pair.Value;

	//	if (!Character || Character == CurrentLeader)
	//	{
	//		continue;
	//	}

	//	Character->SetAsLeader(false);

	//	if (!Character->GetController())
	//	{
	//		Character->SpawnDefaultController();
	//	}

	//	// TODO:
	//	// Cast to your follower AI controller and call FollowActor(CurrentLeader).
	//	// ARPGFollowerAIController* FollowerAI =
	//	//     Cast<ARPGFollowerAIController>(Character->GetController());
	//	// if (FollowerAI)
	//	// {
	//	//     FollowerAI->FollowActor(CurrentLeader);
	//	// }
	//}
}

void ARPGFieldSpawner::ActivateScenarioActors(URPGScenario* RuntimeScenario)
{
	if (RuntimeScenario == nullptr)
	{
		return;
    }

	for (TActorIterator<ARPGTouchTrigger> It(GetWorld()); It; ++It)
	{
		ARPGTouchTrigger* Trigger = *It;

		if (!Trigger)
		{
			continue;
		}

		const bool bActive = RuntimeScenario->TriggerList.Contains(Trigger->GetTriggerData());

		Trigger->SetTriggerActive(bActive);
	}
}

void ARPGFieldSpawner::ClearSpawnedActors()
{
	for (const TPair<FName, TObjectPtr<ARPGFieldCharacter>>& Pair : SpawnedPartyMembers)
	{
		if (Pair.Value)
		{
			Pair.Value->Destroy();
		}
	}

	SpawnedPartyMembers.Empty();
	CurrentLeader = nullptr;
}