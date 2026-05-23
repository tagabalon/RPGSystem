#pragma once

#include "CoreMinimal.h"

#include "GameSettings.generated.h"

UCLASS(Blueprintable, BlueprintType)
class RPGSYSTEM_API UGameSettings : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	TArray<int32> StartingParty;
};
