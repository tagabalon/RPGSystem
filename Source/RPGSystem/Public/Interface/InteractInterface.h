#pragma once

#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

class ShowText;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class RPGSYSTEM_API IInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interact")
	void ShowGlint();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interact")
	void ShowInteract(const FString& InteractText);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interact")
	void HideInteract();
};
