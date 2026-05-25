#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "RPGMessageWidget.generated.h"

UCLASS(Abstract, Blueprintable)
class RPGSYSTEM_API URPGMessageWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Message")
	void ShowText(UShowText* ShowTextData);

	UFUNCTION(BlueprintImplementableEvent, Category = "Message")
	void ShowChoices(UShowChoices* ShowChoicesData);

	UFUNCTION(BlueprintImplementableEvent, Category = "Message")
	void ShowCaption(const FText& Speaker, const FText& Text);

	UFUNCTION(BlueprintImplementableEvent, Category = "Message")
	void CloseMessages();
};