#include "RPGConstants.h"

const FName RPGSlot_MainHand(TEXT("MainHand"));
const FName RPGSlot_OffHand(TEXT("OffHand"));
const FName EquipmentType_Weapon(TEXT("Weapon"));
const FName EquipmentType_Accessory(TEXT("Accessory"));

const FSoftObjectPath FieldInputsMappingPath(TEXT("/RPGSystem/Input/IMC_Defaults.IMC_Defaults"));
const FSoftObjectPath InputMoveAssetPath(TEXT("/RPGSystem/Input/IA_Move.IA_Move"));
const FSoftObjectPath InputCameraAssetPath(TEXT("/RPGSystem/Input/IA_Look.IA_Look"));
const FSoftObjectPath InputInteractAssetPath(TEXT("/RPGSystem/Input/IA_Interact.IA_Interact"));
const FSoftObjectPath InputOpenMenuAssetPath(TEXT("/RPGSystem/Input/IA_Menu.IA_Menu"));
const FSoftObjectPath UIInputsMappingPath(TEXT("/RPGSystem/Input/IMC_Defaults.IMC_UIDefaults"));