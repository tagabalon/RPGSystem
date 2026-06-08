#include "Editor/RPGDataEditorWrappers.h"

#include "Data/RPGDatabase.h"
#include "Data/RPGGameSettings.h"

TArray<FName> URPGDataEditorObject::GetClassIdsOptions() const
{
	return URPGDatabase::GetRPGClassIds();
}


TArray<FName> URPGDataEditorObject::GetEquipmentTypeOptions() const
{
    if (const URPGDatabase* Database = URPGDatabase::GetRPGDatabase())
    {
        if (const URPGGameSettings* GameSettings = Database->GameSettings.LoadSynchronous())
        {
            return GameSettings->EquipmentTypes;
        }
    }
    return TArray<FName>();
}


TArray<FName> URPGDataEditorObject::GetItemOptions() const
{
    return TArray<FName>();
}