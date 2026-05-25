#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "RPGPromptWidget.generated.h"

UCLASS(Abstract, Blueprintable)
class RPGSYSTEM_API URPGPromptWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "RPG|Prompt")
	void SetPromptText(const FText& Text);

	UFUNCTION(BlueprintImplementableEvent, Category = "RPG|Prompt")
	void ShowPrompt();

	UFUNCTION(BlueprintImplementableEvent, Category = "RPG|Prompt")
	void HidePrompt();
};