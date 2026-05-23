// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"

#include "MainCharacter.generated.h"

class AMapEventActor;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class RPGSYSTEM_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	AMainCharacter();
	void SetInteractable(AMapEventActor* InInteractable);

	//AActor
	virtual void Tick(float DeltaTime) override;

	void EnableMovement(bool IsEnabled) { bMovementEnabled = IsEnabled; }
	void EnableCamera(bool IsEnabled) {	bCameraEnabled = IsEnabled;	}


protected:
	//ACharacter
	virtual void BeginPlay() override;

	//APawn
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "RPG Input")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "RPG Input")
	UInputAction* InputCamera;

	UPROPERTY(EditDefaultsOnly, Category = "RPG Input")
	UInputAction* InputMove;

	UPROPERTY(EditDefaultsOnly, Category = "RPG Input")
	UInputAction* InputInteract;

	UFUNCTION(BlueprintImplementableEvent)
	void On_Input_Camera(float Pitch, float Yaw);

	UFUNCTION(BlueprintImplementableEvent)
	void On_Input_Move(float Right, float Forward);

private:/*
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh;*/
	
	FEnhancedInputActionValueBinding* CameraActionValue = nullptr;
	FEnhancedInputActionValueBinding* MoveActionValue = nullptr;
	AMapEventActor* InteractableObject = nullptr;

	bool bMovementEnabled{ true };
	bool bCameraEnabled{ true };

	
	UFUNCTION()
	void InputActionMove();
	UFUNCTION()
	void InputActionCamera();
	UFUNCTION()
	void InputActionInteract();

};
