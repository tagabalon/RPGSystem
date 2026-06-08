#include "Data/RPGCharacters.h"

#include "Data/RPGDatabase.h"
#include "Data/RPGGameSettings.h"

FRPGCharacterData::FRPGCharacterData()
{
    StatBonuses.Add(EBaseStat::Strength);
    StatBonuses.Add(EBaseStat::Agility);
    StatBonuses.Add(EBaseStat::Fortitude);
    StatBonuses.Add(EBaseStat::Intelligence);
    StatBonuses.Add(EBaseStat::Cunning);
    StatBonuses.Add(EBaseStat::Luck);

    if (const URPGDatabase* Database = URPGDatabase::GetRPGDatabase())
    {
        if (const URPGGameSettings* GameSettings = Database->GameSettings.LoadSynchronous())
        {
            for (const FName& EquipTypes : GameSettings->EquipmentTypes)
            {
                int32 Index = StartingEquipment.Emplace();

                StartingEquipment[Index].EquipmentType = EquipTypes;
                StartingEquipment[Index].ItemId = NAME_None;

                //Add two accessories slots and one slot for all other equipment types except weapons, which are already added above.
                if (EquipTypes == EquipmentType_Accessory)
                {
                    Index = StartingEquipment.Emplace();

                    StartingEquipment[Index].EquipmentType = EquipTypes;
                    StartingEquipment[Index].ItemId = NAME_None;
                }
            }
        }
    }
}

void FRPGCharacterData::AddAttackSkill(FName SkillId)
{
    AttackSkills.Add(SkillId);
}

const FEquipItem* FRPGCharacterData::GetStartingEquipment(FName EquipmentType) const
{
    for(int32 Index = 0; Index < StartingEquipment.Num(); Index++)
    {
        if (StartingEquipment[Index].EquipmentType == EquipmentType)
        {
            return &StartingEquipment[Index];
        }
    }

    return nullptr;
}

void FRPGCharacterData::SetStartingEquipment(FName EquipmentType, FName ItemId)
{
    for (FEquipItem& Equip : StartingEquipment)
    {
        if (Equip.EquipmentType == EquipmentType)
        {
            Equip.ItemId = ItemId;
        }
    }
}

TArray<FName> URPGCharacters::GetClassIdsOptions() const
{
    return URPGDatabase::GetRPGClassIds();
}

bool URPGCharacters::GetCharacterData(FName CharacterId, FRPGCharacterData& OutCharacter) const
{
    const int32 Index = Characters.IndexOfByPredicate([&](const FRPGCharacterData& Character)
        {
            return Character.RPGCharacterId == CharacterId;
        });


    if (Characters.IsValidIndex(Index))
    {
        OutCharacter = Characters[Index];
        return true;
    }

    return false;
}