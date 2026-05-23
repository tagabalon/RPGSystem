#pragma once

#include "Branch/BranchParameter.h"
#include "UObject/NoExportTypes.h"
#include "DialogueChoice.generated.h"

UENUM(BlueprintType)
enum EDialogueWheelSlot : uint8
{
	TopRight,
	CenterRight,
	BottomRight,
	TopLeft,
	CenterLeft,
	BottomLeft,
	Bottom
};

UCLASS(Blueprintable)
class RPGSYSTEM_API UDialogueChoice : public UBranchParameter
{
	GENERATED_BODY()

public:

#if WITH_EDITOR
	virtual FText GetDisplayName() const;
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EDialogueWheelSlot> Slot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Text;
};
