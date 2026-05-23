#pragma once

#include "Branch/BranchParameter.h"
#include "Branch.generated.h"

class UBaseCommand;

USTRUCT()
struct RPGSYSTEM_API FBranch
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	UBranchParameter* BranchParam;

#if WITH_EDITOR
	UEdGraphPin* Pin;
#endif

	UPROPERTY()
	UBaseCommand* LinkedCommand;
};