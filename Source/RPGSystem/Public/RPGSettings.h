#pragma once

#include "Engine/DeveloperSettings.h"
#include "Templates/SubclassOf.h"
#include "UObject/SoftObjectPath.h"
#include "RPGSettings.generated.h"

class UInputMappingContext;

UCLASS(Config = Plugins, defaultconfig, meta = (DisplayName = "RPG System"))
class RPGSYSTEM_API URPGSettings : public UDeveloperSettings
{
	GENERATED_UCLASS_BODY()

public:
	static URPGSettings* Get() { return CastChecked<URPGSettings>(URPGSettings::StaticClass()->GetDefaultObject()); }

	UInputMappingContext* GetInputMapping() const { return InputMapping; }

private:
	UPROPERTY(EditAnywhere, Category="RPG System")
	UInputMappingContext* InputMapping;


};