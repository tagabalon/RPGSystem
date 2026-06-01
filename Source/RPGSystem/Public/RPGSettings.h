#pragma once

#include "Engine/DeveloperSettings.h"

#include "InputAction.h"
#include "Templates/SubclassOf.h"
#include "UI/RPGHUD.h"
#include "UI/RPGInteractWidget.h"
#include "UI/RPGMessageWidget.h"
#include "UObject/SoftObjectPath.h"

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

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Scenario Folder", meta = (ContentDir))
	FString ScenarioFolder;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSoftClassPtr<URPGInteractWidget> InteractWidgetAsset;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "UI|Classes", meta = (MetaClass = "/Script/Engine.HUD"))
	FSoftClassPath RPGHUDClass;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Field Input")
	TSoftObjectPtr<UInputMappingContext> InputMappingFieldControls;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "UI Input")
	TSoftObjectPtr<UInputMappingContext> InputMappingUIControls;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Field Input")
	TSoftObjectPtr<UInputAction> InputMove;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Field Input")
	TSoftObjectPtr<UInputAction> InputCamera;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Field Input")
	TSoftObjectPtr<UInputAction> InputInteract;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Field Input")
	TSoftObjectPtr<UInputAction> InputOpenMenu; 

#if WITH_EDITOR
	// Called when any property is changed in the settings UI
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	// Used by Unreal's setting system to display banner warnings/errors at the top of the panel
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

};