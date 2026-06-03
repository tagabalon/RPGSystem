#pragma once

#include "CoreMinimal.h"

#include "Data/RPGTriggerData.h"
#include "GameFramework/Actor.h"
#include "Interface/TriggerInterface.h"

#include "RPGTrigger.generated.h"

UCLASS()
class RPGSYSTEM_API ARPGTrigger : public AActor, public ITriggerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARPGTrigger();

	//ITriggerInterface functions
	const URPGTriggerData* GetTriggerData_Implementation() const override;
	void SetTriggerData_Implementation(URPGTriggerData* pTriggerData) override;
	void SetFinished_Implementation(ERPGTriggerFinishAction FinishAction) override;
	void EnableTrigger_Implementation(bool Enabled) override;
	void ExecuteTrigger_Implementation() override;
	int32 GetActiveState_Implementation(FRPGTriggerState& ActiveState) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger")
	TObjectPtr<URPGTriggerData> TriggerData;

	UPROPERTY(BlueprintReadOnly, Category = "Trigger")
	bool bThreadRunning = false;

	UPROPERTY(BlueprintReadOnly, Category = "Trigger")
	bool bThreadFinished = false;

    int32 ActiveStateIndex = 0;
	bool bRunning = false;
	bool bEnabled = true;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ARPGFieldCharacter> TriggerSource;

#if WITH_EDITOR
	// This function runs automatically after the actor is moved in the editor
	virtual void PostEditMove(bool bFinished) override;
#endif

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	const URPGTriggerData* GetTriggerData() const
	{
		return TriggerData;
	}

};
