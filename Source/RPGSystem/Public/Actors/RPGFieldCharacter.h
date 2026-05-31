#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "RPGFieldCharacter.generated.h"

class ARPGTouchTrigger;
class UAudioComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
struct FRPGPartyMember;

UCLASS()
class RPGSYSTEM_API ARPGFieldCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** Audio */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* Audio;


public:
	ARPGFieldCharacter();

    void SetPartyMemberData(TWeakPtr<FRPGPartyMember> InPartyMemberData) { PartyMemberData = InPartyMemberData; }
	
	UFUNCTION(BlueprintCallable, Category = "Trigger")
	void SetInteractableTrigger(ARPGTouchTrigger* InteractableTrigger);

	UFUNCTION()
	void InputActionInteract();
protected:
	//ACharacter
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(FVector2D MovementVector);

	void Look(FVector2D LookAxisVector);

	bool bMovementEnabled{ true };
	bool bCameraEnabled{ true };

	TWeakPtr<FRPGPartyMember> PartyMemberData;

	UFUNCTION()
	void InputActionMove(const FInputActionValue& Value);

	UFUNCTION()
	void InputActionCamera(const FInputActionValue& Value);

	ARPGTouchTrigger* InteractableTrigger;
};
