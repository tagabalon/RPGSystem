#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "RPGFieldCharacter.generated.h"

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

public:
	ARPGFieldCharacter();

    void SetPartyMemberData(TWeakPtr<FRPGPartyMember> InPartyMemberData) { PartyMemberData = InPartyMemberData; }

protected:
	//ACharacter
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(FVector2D MovementVector);

	void Look(FVector2D LookAxisVector);

	bool bMovementEnabled{ true };
	bool bCameraEnabled{ true };

	TWeakPtr<FRPGPartyMember> PartyMemberData;

	/*UPROPERTY(EditDefaultsOnly, Category = "RPG Input")
	UInputMappingContext* InputMapping;*/

	UPROPERTY(EditDefaultsOnly, Category = "RPG Input")
	UInputAction* InputCamera;

	UPROPERTY(EditDefaultsOnly, Category = "RPG Input")
	UInputAction* InputMove;

	UPROPERTY(EditDefaultsOnly, Category = "RPG Input")
	UInputAction* InputInteract;

	UFUNCTION()
	void InputActionMove(const FInputActionValue& Value);

	UFUNCTION()
	void InputActionCamera(const FInputActionValue& Value);
};
