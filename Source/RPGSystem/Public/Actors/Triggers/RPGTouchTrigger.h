#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actors/Triggers/RPGTrigger.h"
#include "Data/RPGTriggerData.h"

#include "RPGTouchTrigger.generated.h"

class ARPGFieldCharacter;
class URPGInteractableComponent;
class URPGInteractWidget;
class USphereComponent;
class UWidgetComponent;

UCLASS()
class RPGSYSTEM_API ARPGTouchTrigger : public ARPGTrigger
{
	GENERATED_BODY()

public:
	ARPGTouchTrigger();

    // RPGTrigger
	void SetFinished_Implementation(ERPGTriggerFinishAction FinishAction) override;
	void EnableTrigger_Implementation(bool bValue) override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void OnPromptSetVisible(ARPGFieldCharacter* TriggeringCharacter, bool bIsVisible);

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> TriggerSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UWidgetComponent> WidgetComponent;*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<URPGInteractableComponent> InteractComponent;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt")
	bool bPromptForAction = false;*/

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Requirements")
	int32 FlagRequirement = -1;

	UPROPERTY(BlueprintReadOnly, Category = "Trigger")
	bool bPrompted = false;*/

	/*UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnTriggerEndOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex
	);*/

	bool CheckRequirements() const;
	bool IsPromptable(const AActor* Actor) const;

public:
	UFUNCTION(BlueprintCallable, Category = "Trigger")
	void Interact(ARPGFieldCharacter* InstigatorActor);

	//UFUNCTION(BlueprintCallable, Category = "Trigger")
	//void Prompt(AActor* InstigatorActor);

	//UFUNCTION(BlueprintCallable, Category = "Trigger")
	//void UnPrompt();


#if WITH_EDITOR
	virtual void PostActorCreated() override;
#endif

private:
	/*TObjectPtr<USphereComponent> CreateTriggerSphere();
	TObjectPtr<UWidgetComponent> CreateInteractWidget();

	TObjectPtr<URPGInteractWidget> InteractWidget;*/

	bool bIsInProximity = false;
};
