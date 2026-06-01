// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Actors/Triggers/RPGTrigger.h"
#include "GameFramework/Character.h"
#include "Interface/TriggerInterface.h"

#include "RPGNPC.generated.h"

class UAudioComponent;
class UCapsuleComponent;
class UCharacterMovementComponent;
class URPGInteractableComponent;
class USkeletalMesh;
class USkeletalMeshComponent;
class USphereComponent;
class UWidgetComponent;

UCLASS(Blueprintable)
class RPGSYSTEM_API ARPGNPC : public ACharacter, public ITriggerInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARPGNPC();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//ITriggerInterface functions
	const URPGTriggerData* GetTriggerData_Implementation() const override;
    void SetTriggerData_Implementation(URPGTriggerData* pTriggerData) override;
	void SetFinished_Implementation(ERPGTriggerFinishAction FinishAction) override;
	void EnableTrigger_Implementation(bool Enabled) override;
	void ExecuteTrigger_Implementation() override;
	const FRPGEventChain GetActiveState_Implementation() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger")
	TObjectPtr<URPGTriggerData> TriggerData;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, export, Category = "Trigger")
	FText PrompText = FText::FromString("Interact");

#if WITH_EDITOR
	// This function runs automatically after the actor is moved in the editor
	virtual void PostEditMove(bool bFinished) override;
#endif

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
protected:
	UFUNCTION()
	void OnPromptSetVisible(ARPGFieldCharacter* TriggeringCharacter, bool bIsVisible);

	bool CheckRequirements() const;

	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UAudioComponent> AudioSource;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	//USphereComponent* TriggerSphere;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	//TObjectPtr<UWidgetComponent> WidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<URPGInteractableComponent> InteractComponent;

	UPROPERTY()
	TObjectPtr<ARPGFieldCharacter> TriggerSource;

private:

	bool bIsInProximity = false;
	bool bThreadRunning = false;
	bool bThreadFinished = false;
	int32 ActiveStateIndex = 0;

};
