// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/RPGFieldCharacter.h"

#include "RPGPlayerCharacter.generated.h"

class AMapEventActor;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;

/**
 * 
 */
UCLASS()
class RPGSYSTEM_API ARPGPlayerCharacter : public ARPGFieldCharacter
{
	GENERATED_BODY()
	
public:
	ARPGPlayerCharacter();
	void SetInteractable(AMapEventActor* InInteractable);

	//AActor
	virtual void Tick(float DeltaTime) override;

	void EnableMovement(bool IsEnabled) { bMovementEnabled = IsEnabled; }
	void EnableCamera(bool IsEnabled) {	bCameraEnabled = IsEnabled;	}


protected:
	//ACharacter
	virtual void BeginPlay() override;

	//APawn
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*UPROPERTY(EditDefaultsOnly, Category = "RPG Input")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "RPG Input")
	UInputAction* InputCamera;

	UPROPERTY(EditDefaultsOnly, Category = "RPG Input")
	UInputAction* InputMove;

	UPROPERTY(EditDefaultsOnly, Category = "RPG Input")
	UInputAction* InputInteract;*/


	/*UFUNCTION()
	void InputActionMove(const FInputActionValue& Value);

	UFUNCTION()
	void InputActionCamera(const FInputActionValue& Value);*/

private:
	AMapEventActor* InteractableObject = nullptr;

};
