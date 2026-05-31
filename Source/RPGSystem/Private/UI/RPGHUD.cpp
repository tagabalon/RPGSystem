#include "UI/RPGHUD.h"

#include "UI/RPGMessageWidget.h"

void ARPGHUD::BeginPlay()
{
    Super::BeginPlay();
}

TObjectPtr<UUserWidget> ARPGHUD::CreateOrGetMessageWidget()
{
    if (!MessageWidget && MessageWidgetClass)
    {
        MessageWidget = CreateWidget<UUserWidget>(GetWorld(), MessageWidgetClass);
        if (MessageWidget)
        {
            MessageWidget->AddToViewport();
        }
    }

    return MessageWidget; 
}