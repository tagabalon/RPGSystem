#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/RPGTriggerData.h"

#include "RPGTouchTrigger.generated.h"

class USphereComponent;
class UWidgetComponent;
class URPGInteractWidget;
class ARPGFieldCharacter;

UCLASS()
class RPGSYSTEM_API ARPGTouchTrigger : public AActor
{
	GENERATED_BODY()

public:
	ARPGTouchTrigger();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> TriggerSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UWidgetComponent> WidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger")
	TObjectPtr<URPGTriggerData> TriggerData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt")
	bool bPromptForAction = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt")
	FText PromptText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt")
	float PromptDistance = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Requirements")
	int32 FlagRequirement = -1;

	UPROPERTY(BlueprintReadOnly, Category = "Trigger")
	bool bPrompted = false;

	UPROPERTY(BlueprintReadOnly, Category = "Trigger")
	bool bThreadRunning = false;

	UPROPERTY(BlueprintReadOnly, Category = "Trigger")
	bool bThreadFinished = false;

	UPROPERTY()
	TObjectPtr<ARPGFieldCharacter> TriggeringActor;

	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnTriggerEndOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex
	);

	bool CheckRequirements() const;
	bool IsPromptable(const AActor* Actor) const;

public:

	UFUNCTION(BlueprintCallable, Category = "Trigger")
	const URPGTriggerData* GetTriggerData() const
	{
        return TriggerData;
	}

	UFUNCTION(BlueprintCallable, Category = "Trigger")
	void Interact(ARPGFieldCharacter* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "Trigger")
	void Prompt(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "Trigger")
	void UnPrompt();

	UFUNCTION(BlueprintCallable, Category = "Trigger")
	void SetFinished(ERPGTriggerFinishAction FinishAction);

	UFUNCTION(BlueprintCallable, Category = "Trigger")
	void DisableTrigger();

	UFUNCTION(BlueprintCallable, Category = "Trigger")
	void SetTriggerActive(bool bActive);

#if WITH_EDITOR
	virtual void PostActorCreated() override;
#endif

private:
	TObjectPtr<USphereComponent> CreateTriggerSphere();
	TObjectPtr<UWidgetComponent> CreateInteractWidget();

	TObjectPtr<URPGInteractWidget> InteractWidget;

	bool bIsInProximity = false;
	bool bRunning = false;
};
