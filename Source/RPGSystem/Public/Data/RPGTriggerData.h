#pragma once

#include "CoreMinimal.h"

#include "Commands/RPGCommand.h"
#include "RPGTriggerData.generated.h"

class URPGCommand;

UENUM(BlueprintType)
enum class ERPGTriggerFinishAction : uint8
{
	None,
	Repeat,
	Hide
};

UENUM(BlueprintType)
enum class ERPGTriggerActivation : uint8
{
	InteractButton,
	PlayerTouch,
	TriggerTouch,
	AutuActivate,
	ParallelRun
};

USTRUCT(BlueprintType)
struct FRPGTriggerState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Conditions", meta = (DisplayName="Switch"))
	bool HasSwitchACondition = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Conditions", meta = (DisplayName="Id", EditCondition = "HasSwitchACondition"))
	FName SwitchAId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Conditions", meta = (DisplayName = "Switch"))
	bool HasSwitchBCondition = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Conditions", meta = (DisplayName = "Id", EditCondition = "HasSwitchBCondition"))
	FName SwitchBId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Conditions", meta = (DisplayName = "Variable"))
	bool HasVariableCondition = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Conditions", meta = (DisplayName = "Id", EditCondition = "HasVariableCondition"))
	FName VariableId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Conditions", meta = (DisplayName = "is greater than or equal to...", EditCondition = "HasVariableCondition"))
	int32 VariableCheck;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Conditions", meta = (DisplayName = "Item"))
	bool HasItemCondition = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Conditions", meta = (DisplayName = "Id", EditCondition = "HasItemCondition"))
	FName ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Conditions", meta = (DisplayName = "Character"))
	bool HasCharacterCondition = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Conditions", meta = (DisplayName = "Id", EditCondition = "HasCharacterCondition"))
	FName CharacterId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<USkeletalMesh> CharacterMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UAnimBlueprint> CharacterAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Activation")
	ERPGTriggerActivation TriggerActivation = ERPGTriggerActivation::InteractButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Activation")
	float TriggerRange = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger")
	ERPGTriggerFinishAction FinishAction = ERPGTriggerFinishAction::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	bool NoCollide = false;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite, Category = "Events")
	TArray<TObjectPtr<URPGCommand>> Commands;
};

UCLASS(BlueprintType)
class RPGSYSTEM_API URPGTriggerData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	URPGTriggerData()
	{
		States.AddDefaulted();
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger")
	FName TriggerId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger")
    FVector WorldPosition = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger")
    FRotator WorldRotation = FRotator::ZeroRotator;
		
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Editor")
	FVector2D GraphPosition = FVector2D::ZeroVector;*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Events")
	TArray<FRPGTriggerState> States;

	UFUNCTION(BlueprintPure, Category = "Events")
	int32 GetStateCount() const
	{
		return States.Num();
	}

	UFUNCTION(BlueprintCallable, Category = "Events")
	int32 AddState()
	{
		return States.AddDefaulted();
	}

	UFUNCTION(BlueprintCallable, Category = "Events")
	void ClearEmptyStates()
	{
		for (int32 i = States.Num() - 1; i >= 0; --i)
		{
			if (States[i].Commands.Num() == 0)
			{
				States.RemoveAt(i);
			}
		}

		if (States.Num() == 0)
		{
			States.AddDefaulted();
		}
	}

	const FRPGTriggerState* GetChain(int32 State) const
	{
		return States.IsValidIndex(State) ? &States[State] : nullptr;
	}
};