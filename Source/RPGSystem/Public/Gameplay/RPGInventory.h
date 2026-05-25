#pragma once

#include "CoreMinimal.h"

#include "RPGInventory.generated.h"

USTRUCT(BlueprintType)
struct FRPGInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EquippedActorId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemSubType = 0;
};

USTRUCT(BlueprintType)
struct FRPGInventory
{
    GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FRPGInventoryItem> Items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Gold = 0;
};