#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RPGHUD.generated.h"

class UUserWidget;

UCLASS()
class RPGSYSTEM_API ARPGHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;

    TObjectPtr<UUserWidget> CreateOrGetMessageWidget();
private:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> MessageWidgetClass;

    UPROPERTY()
    UUserWidget* MessageWidget;
};
