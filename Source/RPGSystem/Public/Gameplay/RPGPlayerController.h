#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputCoreTypes.h"
#include "RPGPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class ARPGFieldPartyManager;

UCLASS()
class RPGSYSTEM_API ARPGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	/*UFUNCTION(BlueprintCallable, Category = "RPG|Input")
	void SetFieldPartyManager(ARPGFieldPartyManager* InManager);*/

	UFUNCTION(BlueprintPure, Category = "RPG|Input")
	bool GetKeyForInputAction(const UInputAction* InputAction, FKey& OutKey) const;

	UFUNCTION(Exec)
	void DebugInputState();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	int32 FieldMappingPriority = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Actions")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Actions")
	TObjectPtr<UInputAction> SwapLeaderAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Actions")
	TObjectPtr<UInputAction> PauseAction;

	/*UPROPERTY(BlueprintReadOnly, Category = "RPG|Party")
	TObjectPtr<ARPGFieldPartyManager> FieldPartyManager;*/

	void HandleInteract();
	void HandleSwapLeader();
	void HandlePause();
};