#include "Data/RPGCharacters.h"
#include "Data/RPGDatabase.h"
#include "Data/RPGGameSettings.h"

FRPGCharacterData::FRPGCharacterData()
{
    if (const URPGDatabase* Database = URPGDatabase::GetRPGDatabase())
    {
        if (const URPGGameSettings* GameSettings = Database->GameSettings.LoadSynchronous())
        {
            for (const FName& EquipTypes : GameSettings->EquipmentTypes)
            {
                //Add two accessories slots and one slot for all other equipment types except weapons, which are already added above.
                if (EquipTypes == EquipmentType_Accessory)
                {
                    StartingEquipment.Add(EquipTypes, NAME_None);
                    StartingEquipment.Add(EquipTypes, NAME_None);
                }
            }
        }
    }
}