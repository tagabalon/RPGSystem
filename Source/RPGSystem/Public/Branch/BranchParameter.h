#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BranchParameter.generated.h"

UCLASS()
class RPGSYSTEM_API UBranchParameter : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName Label;

#if WITH_EDITOR
	virtual FText GetDisplayName() const;
#endif
};
