#pragma once

#include "CoreMinimal.h"
#include "RPGTypes.generated.h"

UENUM(BlueprintType)
enum class EBaseStat : uint8
{
	Strength UMETA(DisplayName = "Strength"),
	Agility UMETA(DisplayName = "Agility"),
	Fortitude UMETA(DisplayName = "Fortitude"),
	Intelligence UMETA(DisplayName = "Intelligence"),
	Cunning UMETA(DisplayName = "Cunning"),
	Luck UMETA(DisplayName = "Luck")
};

UENUM(BlueprintType)
enum class EControlMode : uint8
{
	Field UMETA(DisplayName = "Field Controls"),
	Combat UMETA(DisplayName = "Combat Controls"),
	UI UMETA(DisplayName = "UI Controls"),
	None UMETA(DisplayName = "None")
};