#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"
#include "SNodePanel.h"

class URPGCommandGraphNode;

class UMoveTo;
class UShowText;
class UStartConvo;
class URPGConvoAsset;

class SGraphNode_RPGCommand : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SGraphNode_RPGCommand) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, URPGCommandGraphNode* InNode);

	virtual void UpdateGraphNode() override;
	virtual void MoveTo(const FVector2D& NewPosition, FNodeSet& NodeFilter, bool bMarkDirty = true) override;

private:
	FText GetBodyText() const;
	TSharedRef<SWidget> CreateCommandBodyWidget();
	FText GetTitleText() const;

	const FSlateBrush* GetTextureBrush(UTexture2D* Texture) const;
    const TArray<TSharedPtr<FName>> GetConvoIds(URPGConvoAsset* Convo) const;
	void RefreshNamedOptions(TArray<FName> NameList);
	void RefreshEnumOptions(const UEnum* Enum);

	TSharedRef<SWidget> CreateShowTextBody(UShowText* Command);
	TSharedRef<SWidget> CreateStartConvoBody(UStartConvo* Command);
	TSharedRef<SWidget> CreateMoveToBody(UMoveTo* Command);

	TArray<TSharedPtr<FName>> NamedOptions;
};