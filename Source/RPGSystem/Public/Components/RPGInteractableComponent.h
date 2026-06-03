#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "RPGInteractableComponent.generated.h"

class ARPGFieldCharacter;
class URPGInteractWidget;
class USphereComponent;
class UWidgetComponent;

DECLARE_DELEGATE_RetVal(bool, FCheckTriggerConditionSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractSignature, ARPGFieldCharacter*, TriggeringCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPromptVisibilityChangedSignature, ARPGFieldCharacter*, TriggeringCharacter, bool, bIsVisible);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGSYSTEM_API URPGInteractableComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URPGInteractableComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Interact")
	void SetAwarenessRadius(float Radius);

	UFUNCTION(BlueprintCallable, Category = "Interact")
	void EnableInteract(bool Enabled);

	// Delegates
	FCheckTriggerConditionSignature OnCheckTriggerCondition;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteractSignature OnInteractPressed;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnPromptVisibilityChangedSignature OnPromptVisibilityChanged;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
#endif //WITH_EDITOR

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	UFUNCTION()
	void OnEnterInteractable(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnExitInteractable(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
	bool IsPromptable(const ARPGFieldCharacter* OtherCharacter) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> TriggerSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UWidgetComponent> WidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt")
	FText PromptText = FText::FromString(TEXT("Interact"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt")
	float PromptDistance = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt")
	float AwarenessRadius = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prompt")
	bool bPromptForAction = true;

private:
    bool bIsInProximity = false;
    bool bIsPrompted = false;

	TObjectPtr<URPGInteractWidget> InteractWidget;
    TObjectPtr<ARPGFieldCharacter> TriggerSource;
};
