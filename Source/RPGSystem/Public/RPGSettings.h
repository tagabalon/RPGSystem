#pragma once

#include "Engine/DeveloperSettings.h"
#include "Templates/SubclassOf.h"
#include "UObject/SoftObjectPath.h"
#include "UI/RPGPromptWidget.h"
#include "UI/RPGMessageWidget.h"

#include "RPGSettings.generated.h"

class UInputMappingContext;
class URPGDatabase;

UCLASS(Config = Plugins, defaultconfig, meta = (DisplayName = "RPG System"))
class RPGSYSTEM_API URPGSettings : public UDeveloperSettings
{
	GENERATED_UCLASS_BODY()

public:
	static URPGSettings* Get() { return CastChecked<URPGSettings>(URPGSettings::StaticClass()->GetDefaultObject()); }

	virtual FName GetCategoryName() const
	{
		return FName("Plugins");
	}

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Database Configuration")
	TSoftObjectPtr<URPGDatabase> GameDatabaseAsset;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSoftClassPtr<URPGPromptWidget> PromptWidgetAsset;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSoftClassPtr<URPGMessageWidget> MessageWidgetAsset;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

#if WITH_EDITOR
	// Called when any property is changed in the settings UI
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	// Used by Unreal's setting system to display banner warnings/errors at the top of the panel
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

};