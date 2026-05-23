#include "Branch/BranchParameter.h"

#define LOCTEXT_NAMESPACE "BranchParameter"

#if WITH_EDITOR

FText UBranchParameter::GetDisplayName() const
{
    return FText::FromString(Label.ToString());
}

#endif


#undef LOCTEXT_NAMESPACE