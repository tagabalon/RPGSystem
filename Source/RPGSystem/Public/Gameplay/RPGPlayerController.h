#pragma once

#include "CoreMinimal.h"
#include "Data/RPGTypes.h"
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
    ARPGPlayerController();
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	/*UFUNCTION(BlueprintCallable, Category = "RPG|Input")
	void SetFieldPartyManager(ARPGFieldPartyManager* InManager);*/

	UFUNCTION(BlueprintPure, Category = "RPG|Input")
	bool GetKeyForInputAction(const UInputAction* InputAction, FKey& OutKey) const;

	UFUNCTION(Exec)
	void DebugInputState();

	UFUNCTION(BlueprintCallable, Category = "Controls")
    void SetControlMode(EControlMode ControlMode);

    static ARPGPlayerController* GetPlayerController(UObject* WorldContextObject);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	int32 FieldMappingPriority = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Actions")
	TObjectPtr<UInputAction> InputInteract;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Actions")
	TObjectPtr<UInputAction> SwapLeaderAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Actions")
	TObjectPtr<UInputAction> PauseAction;

	/*UPROPERTY(BlueprintReadOnly, Category = "RPG|Party")
	TObjectPtr<ARPGFieldPartyManager> FieldPartyManager;*/

	void HandleInteract();
	void HandleSwapLeader();
	void HandlePause();

private:
    EControlMode ActiveControlMode = EControlMode::None;

    UInputMappingContext* CurrentInputMappingContext = nullptr;
    UInputMappingContext* FieldInputMappingContext = nullptr;
    UInputMappingContext* UIInputMappingContext = nullptr;
};