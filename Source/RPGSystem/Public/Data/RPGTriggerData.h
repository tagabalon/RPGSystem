#pragma once

#include "CoreMinimal.h"

#include "Data/Commands/RPGCommand.h"
#include "RPGTriggerData.generated.h"

class URPGCommand;

UENUM(BlueprintType)
enum class ERPGTriggerFinishAction : uint8
{
	None,
	Repeat,
	Hide
};

USTRUCT(BlueprintType)
struct FRPGEventChain
{
	GENERATED_BODY()

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
	int32 TriggerId = -1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger")
	ERPGTriggerFinishAction FinishAction = ERPGTriggerFinishAction::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Editor")
	FVector2D GraphPosition = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Events")
	TArray<FRPGEventChain> States;

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

	const FRPGEventChain* GetChain(int32 State) const
	{
		return States.IsValidIndex(State) ? &States[State] : nullptr;
	}
};