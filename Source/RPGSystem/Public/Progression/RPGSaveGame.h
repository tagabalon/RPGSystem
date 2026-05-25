#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RPGSaveGame.generated.h"

UCLASS()
class RPGSYSTEM_API URPGSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
	FString SavedMapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
	FVector SavedPlayerPosition = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
	FRotator SavedPlayerRotation = FRotator::ZeroRotator;
};