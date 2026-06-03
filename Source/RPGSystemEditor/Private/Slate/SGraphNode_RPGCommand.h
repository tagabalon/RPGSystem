#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"

class URPGCommandGraphNode;

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

private:
	FText GetBodyText() const;
	TSharedRef<SWidget> CreateCommandBodyWidget();
	FText GetTitleText() const;

	const FSlateBrush* GetTextureBrush(UTexture2D* Texture) const;
    const TArray<TSharedPtr<FName>> GetConvoIds(URPGConvoAsset* Convo) const;

	TSharedRef<SWidget> CreateShowTextBody(UShowText* Command);
	TSharedRef<SWidget> CreateStartConvoBody(UStartConvo* Command);
};