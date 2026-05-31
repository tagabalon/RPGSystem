#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Interface/InteractInterface.h"

#include "RPGInteractWidget.generated.h"

UCLASS(Blueprintable)
class RPGSYSTEM_API URPGInteractWidget : public UUserWidget, public IInteractInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "RPG|Prompt")
	void SetPromptText(const FText& Text);

	UFUNCTION(BlueprintImplementableEvent, Category = "RPG|Prompt")
	void ShowPrompt(const FString& InteractText);

	UFUNCTION(BlueprintImplementableEvent, Category = "RPG|Prompt")
	void HideInteract();
};