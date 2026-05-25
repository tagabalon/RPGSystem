#include "RPGSettings.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h" // Required for FDataValidationContext
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(RPGSettings)

URPGSettings::URPGSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

#if WITH_EDITOR
void URPGSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    // If they just modified the database asset property, trigger a validation check
    if (PropertyChangedEvent.Property && PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(URPGSettings, GameDatabaseAsset))
    {
        if (!GameDatabaseAsset.IsValid())
        {
            UE_LOG(LogTemp, Warning, TEXT("MyPluginSettings: GameDatabaseAsset has not been assigned yet!"));
        }
    }
}

EDataValidationResult URPGSettings::IsDataValid(FDataValidationContext& Context) const
{
    // Start by evaluating the base class state
    EDataValidationResult Result = Super::IsDataValid(Context);

    // Check if the soft path points to a valid asset
    if (!GameDatabaseAsset.IsValid())
    {
        // This line adds a visible error text banner at the top of your Project Settings page
        Context.AddError(FText::FromString("The Game Database Asset is required for the plugin to function. Please assign it."));
        Result = EDataValidationResult::Invalid;
    }

    return Result;
}
#endif