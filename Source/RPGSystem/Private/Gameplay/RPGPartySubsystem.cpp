#include "Gameplay/RPGPartySubsystem.h"

#include "Data/RPGDatabase.h"
#include "Data/RPGCharacters.h"
#include "Data/RPGClasses.h"
#include "Data/RPGGameSettings.h"
#include "RPGConstants.h"

void URPGPartySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void URPGPartySubsystem::Deinitialize()
{
	PartyMembers.Empty();
	Inventory.Gold = 0;

	Super::Deinitialize();
}

void URPGPartySubsystem::InitializeNewGame(const URPGDatabase* Database)
{
	PartyMembers.Empty();
	Inventory.Gold = 0;

	if (!Database)
	{
		return;
    }

	URPGGameSettings* GameSettings = Database->GameSettings.LoadSynchronous();
	if (GameSettings == nullptr)
	{
        return;
	}

	CombatStartingStamina = GameSettings->CombatStartingStaminaRate;

	if (GameSettings->StartingParty.Num() > 0)
	{
		for (const FName& ActorId : GameSettings->StartingParty)
		{
			InitializePartyMember(Database, ActorId);
		}
    }
}

void URPGPartySubsystem::InitializePartyMember(const URPGDatabase* Database, const FName& CharacterId)
{
	URPGCharacters* CharacterDatabase = Database->CharactersDatabase.LoadSynchronous();
	if (CharacterDatabase == nullptr)
	{
		return;
	}

	URPGClasses* ClassDatabase = Database->ClassesDatabase.LoadSynchronous();
	if (ClassDatabase == nullptr)
	{
		return;
	}

	FRPGCharacterData CharacterData;
	if (!CharacterDatabase->GetCharacterData(CharacterId, CharacterData))
	{
		return;
	}

	FRPGClassData ClassData;
	if (!ClassDatabase->GetClass(CharacterData.RPGClassId, ClassData))
	{
		return;
	}

    TSharedPtr<FRPGPartyMember> NewMember = MakeShared<FRPGPartyMember>();

	//FRPGPartyMember NewMember;
	NewMember->CharacterId = CharacterData.RPGCharacterId;
    NewMember->ClassId = CharacterData.RPGClassId;
	NewMember->Level = CharacterData.InitialLevel;
    //TODO: Calculate starting HP/MP/Stamina based on class and level
	//NewMember.CurrentXP =

    InitializeBaseStats(NewMember->BaseStats, ClassData, NewMember->Level);
	NewMember->DerivedStats.InitializeWithBaseStats(NewMember->BaseStats, NewMember->Level);

    NewMember->CurrentHP = NewMember->DerivedStats.GetInt(ERPGDerivedStat::MaxHealth);
    NewMember->CurrentMP = NewMember->DerivedStats.GetInt(ERPGDerivedStat::MaxMana);
    NewMember->CurrentStamina = FMath::RoundToInt(NewMember->DerivedStats.GetFloat(ERPGDerivedStat::MaxStamina) * CombatStartingStamina);

	InitializeEqupment(Database, NewMember->CharacterId, NewMember->Equipment);
    //NewMember.Equipment.Initialize(Database);

	PartyMembers.Add(NewMember);
}

void URPGPartySubsystem::InitializeBaseStats(TArray<int32>& BaseStats, const FRPGClassData& ClassData, int32 Level)
{
	// Ensure BaseStats always has exactly 6 entries.
	constexpr int32 DesiredNumStats = 6;

	BaseStats.Empty();
	BaseStats.Reserve(DesiredNumStats);

	for (int32 StatIndex = 0; StatIndex < DesiredNumStats; ++StatIndex)
	{
		int32 StatValue = 10; // default fallback

		if (ClassData.BaseStats.IsValidIndex(StatIndex))
		{
			const TObjectPtr<UCurveFloat>& Curve = ClassData.BaseStats[StatIndex];
			if (Curve)
			{
				StatValue = FMath::RoundToInt(Curve->GetFloatValue(Level));
			}
		}

		BaseStats.Add(StatValue);
	}
}

void URPGPartySubsystem::InitializeEqupment(const URPGDatabase* Database, FName CharacterId, FRPGCharacterEquipment& Equipment)
{
	if (URPGGameSettings* GameSettings = Database->GameSettings.LoadSynchronous())
	{
		FRPGCharacterData CharacterData;
		if (!Database->CharactersDatabase.LoadSynchronous()->GetCharacterData(CharacterId, CharacterData))
		{
			return;
        }

		for (const auto& [Key, Value] : CharacterData.StartingEquipment)
		{
			FRPGEquipmentSlot NewSlot;

			NewSlot.SlotId = Key;
			NewSlot.InventoryItem.ItemId = Value;
			NewSlot.InventoryItem.EquippedActorId = CharacterId;
			NewSlot.InventoryItem.Quantity = 1; // Assuming starting equipment is always 1 quantity
	
            Equipment.EquipmentSlots.Add(NewSlot);
            Inventory.Items.Add(NewSlot.InventoryItem);
		}
	}
}

bool URPGPartySubsystem::AddPartyMember(FName ActorId, int32 Level)
{
	if (ActorId.IsNone() || HasPartyMember(ActorId))
	{
		return false;
	}

	/*FRPGPartyMember NewMember;
	NewMember.CharacterId = ActorId;
	NewMember.Level = FMath::Max(1, Level);

	PartyMembers.Add(NewMember);*/
	return true;
}

bool URPGPartySubsystem::RemovePartyMember(FName ActorId)
{
	/*const int32 RemovedCount = PartyMembers.RemoveAll(
		[ActorId](const FRPGPartyMember& Member)
		{
			return Member.CharacterId == ActorId;
		}
	);

	return RemovedCount > 0;*/
	return false;
}

bool URPGPartySubsystem::HasPartyMember(FName ActorId) const
{
	/*return PartyMembers.ContainsByPredicate(
		[ActorId](const FRPGPartyMember& Member)
		{
			return Member.CharacterId == ActorId;
		}
	);*/
	return false;
}

void URPGPartySubsystem::AddItem(FName ItemId, int32 Quantity)
{
	if (ItemId.IsNone() || Quantity <= 0)
	{
		return;
	}

	/*for (FRPGInventoryItem& Entry : Inventory)
	{
		if (Entry.ItemId == ItemId)
		{
			Entry.Quantity += Quantity;
			return;
		}
	}

	FRPGInventoryItem NewEntry;
	NewEntry.ItemId = ItemId;
	NewEntry.Quantity = Quantity;

	Inventory.Add(NewEntry);*/
}

bool URPGPartySubsystem::RemoveItem(FName ItemId, int32 Quantity)
{
	if (ItemId.IsNone() || Quantity <= 0)
	{
		return false;
	}

	/*for (int32 i = 0; i < Inventory.Num(); ++i)
	{
		FRPGInventoryItem& Entry = Inventory[i];

		if (Entry.ItemId != ItemId)
		{
			continue;
		}

		if (Entry.Quantity < Quantity)
		{
			return false;
		}

		Entry.Quantity -= Quantity;

		if (Entry.Quantity <= 0)
		{
			Inventory.RemoveAt(i);
		}

		return true;
	}*/

	return false;
}

int32 URPGPartySubsystem::GetItemQuantity(FName ItemId) const
{
	/*for (const FRPGInventoryItem& Entry : Inventory)
	{
		if (Entry.ItemId == ItemId)
		{
			return Entry.Quantity;
		}
	}*/

	return 0;
}

void URPGPartySubsystem::AddGold(int32 Amount)
{
	if (Amount > 0)
	{
		Inventory.Gold += Amount;
	}
}

bool URPGPartySubsystem::SpendGold(int32 Amount)
{
	if (Amount <= 0 || Inventory.Gold < Amount)
	{
		return false;
	}

	Inventory.Gold -= Amount;
	return true;
}