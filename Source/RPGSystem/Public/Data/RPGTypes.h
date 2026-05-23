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