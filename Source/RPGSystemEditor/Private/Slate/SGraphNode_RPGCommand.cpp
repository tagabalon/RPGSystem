#include "Slate/SGraphNode_RPGCommand.h"

#include "Graph/RPGCommandGraphNode.h"
#include "Commands/RPGCommand.h"
#include "Commands/MoveTo.h"
#include "Commands/ShowText.h"
#include "Commands/StartConvo.h"
#include "Data/RPGConvoAsset.h"

#include "PropertyCustomizationHelpers.h"
#include "SGraphPin.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Text/STextBlock.h"

void SGraphNode_RPGCommand::Construct(
	const FArguments& InArgs,
	URPGCommandGraphNode* InNode
)
{
	GraphNode = InNode;
	UpdateGraphNode();
}

void SGraphNode_RPGCommand::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();

	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);

	GetOrAddSlot(ENodeZone::Center)
		[
			SNew(SBorder)
				.BorderImage(FAppStyle::GetBrush("Graph.Node.Body"))
				.Padding(0)
				[
					SNew(SVerticalBox)

						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(SBorder)
								.BorderImage(FAppStyle::GetBrush("WhiteBrush"))
								.BorderBackgroundColor(FLinearColor(0.0f, 0.478f, 0.2f))
								.Padding(FMargin(8.0f, 4.0f))
								[
									SNew(STextBlock)
										.Text(this, &SGraphNode_RPGCommand::GetTitleText)
										.ColorAndOpacity(FLinearColor::Black)
								]
						]

					+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(8.0f)
						[
							CreateCommandBodyWidget()
						]

						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(FMargin(4.0f, 2.0f))
						[
							SNew(SHorizontalBox)

								+ SHorizontalBox::Slot()
								.AutoWidth()
								[
									SAssignNew(LeftNodeBox, SVerticalBox)
								]

								+ SHorizontalBox::Slot()
								.FillWidth(1.0f)
								[
									SNew(SSpacer)
								]

								+ SHorizontalBox::Slot()
								.AutoWidth()
								[
									SAssignNew(RightNodeBox, SVerticalBox)
								]
						]
				]
		];

	CreatePinWidgets();
}

FText SGraphNode_RPGCommand::GetTitleText() const
{
	const URPGCommandGraphNode* RPGNode = Cast<URPGCommandGraphNode>(GraphNode);
	if (!RPGNode || !RPGNode->Command)
	{
		return FText::FromString(TEXT("Command"));
	}

#if WITH_EDITOR
	return RPGNode->Command->GetNodeTitle();
#else
	return FText::FromString(TEXT("Command"));
#endif
}

TSharedRef<SWidget> SGraphNode_RPGCommand::CreateCommandBodyWidget()
{
	const URPGCommandGraphNode* RPGNode = Cast<URPGCommandGraphNode>(GraphNode);
	if (!RPGNode || !RPGNode->Command)
	{
		return SNew(STextBlock).Text(FText::FromString(TEXT("Invalid command")));
	}

	if (UShowText* ShowText = Cast<UShowText>(RPGNode->Command))
	{
		return CreateShowTextBody(ShowText);
	}
	else if (UStartConvo* StartConvo = Cast<UStartConvo>(RPGNode->Command))
	{
		return CreateStartConvoBody(StartConvo);
    }
	else if (UMoveTo* MoveTo = Cast<UMoveTo>(RPGNode->Command))
	{
		RefreshEnumOptions(StaticEnum<ETargetToMove>());
		return CreateMoveToBody(MoveTo);
	}

	return SNew(STextBlock)
		.Text(this, &SGraphNode_RPGCommand::GetBodyText)
		.AutoWrapText(true);
}

FText SGraphNode_RPGCommand::GetBodyText() const
{
	const URPGCommandGraphNode* RPGNode = Cast<URPGCommandGraphNode>(GraphNode);
	if (!RPGNode || !RPGNode->Command)
	{
		return FText::GetEmpty();
	}

#if WITH_EDITOR
	return FText::FromString(TEXT("Command"));// RPGNode->Command->GetEditorNodeBodyText();
#else
	return FText::GetEmpty();
#endif
}

const FSlateBrush* SGraphNode_RPGCommand::GetTextureBrush(UTexture2D* Texture) const
{
	static FSlateBrush Brush;

	if (Texture)
	{
		Brush.SetResourceObject(Texture);
	}

	return &Brush;
}

const TArray<TSharedPtr<FName>> SGraphNode_RPGCommand::GetConvoIds(URPGConvoAsset* Convo) const
{
	TArray<TSharedPtr<FName>> ConvoIds;

	if (Convo)
	{
		for (const FRPGConvo& ConvoData : Convo->Convos)
		{
			ConvoIds.Add(MakeShared<FName>(ConvoData.ConvoId));
		}
    }

	return ConvoIds;
}

void SGraphNode_RPGCommand::RefreshNamedOptions(TArray<FName> NameList)
{
	NamedOptions.Empty();

	for (FName Item : NameList)
	{
		NamedOptions.Add(MakeShared<FName>(Item));
	}
}

void SGraphNode_RPGCommand::RefreshEnumOptions(const UEnum* Enum)
{
	NamedOptions.Empty();
	if (!Enum)
	{
		return;
	}

	for (int32 Index = 0; Index < Enum->NumEnums(); ++Index)
	{
		// Skip hidden entries
		if (Enum->HasMetaData(TEXT("Hidden"), Index))
		{
			continue;
		}

		const FString Name = Enum->GetNameStringByIndex(Index);

		NamedOptions.Add(MakeShared<FName>(*Name));
	}
}

TSharedRef<SWidget> SGraphNode_RPGCommand::CreateShowTextBody(UShowText* Command)
{
	/*return SNew(SVerticalBox)

		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 0, 0, 2)
		[
			SNew(STextBlock)
				.Text(FText::FromString(TEXT("Speaker")))
		]

		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(8, 0, 0, 4)
		[
			SNew(SEditableTextBox)
				.Text_Lambda([Command]()
					{
						return Command->SpeakerName;
					})
				.OnTextCommitted_Lambda([Command](const FText& NewText, ETextCommit::Type)
					{
						Command->Modify();
						Command->SpeakerName = NewText;
					})
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 0, 0, 4)
		[
			SNew(SMultiLineEditableTextBox)
				.Text_Lambda([Command]()
					{
						return Command->Text;
					})
				.OnTextCommitted_Lambda([Command](const FText& NewText, ETextCommit::Type)
					{
						Command->Modify();
						Command->Text = NewText;
					})
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(STextBlock)
				.Text(FText::FromString(TEXT("Bust image preview goes here")))
		];*/

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 0, 0, 2)
		[
			SNew(SBox)
			.WidthOverride(64)
			.HeightOverride(64)
			[
				SNew(SImage)
				.Image(GetTextureBrush(Command->Face))
			]
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 0, 0, 2)
		[
			SNew(SGridPanel)
			+ SGridPanel::Slot(0, 0)
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("Bust")))
			]

			+ SGridPanel::Slot(1, 0)
			[
				SNew(SObjectPropertyEntryBox)
				.AllowedClass(UTexture2D::StaticClass())
				.ObjectPath_Lambda([Command]()
				{
					return Command->Face
						? Command->Face->GetPathName()
						: FString();
				})
				.OnObjectChanged_Lambda([Command](const FAssetData& AssetData)
				{
					Command->Modify();
					Command->Face = Cast<UTexture2D>(AssetData.GetAsset());
				})
			]

			+ SGridPanel::Slot(0, 1)
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("Speaker")))
			]

			+ SGridPanel::Slot(1, 1)
			[
				SNew(SEditableTextBox)
				.Text_Lambda([Command]()
				{
					return Command->SpeakerName;
				})
				.OnTextCommitted_Lambda([Command](const FText& NewText, ETextCommit::Type)
				{
					Command->Modify();
					Command->SpeakerName = NewText;
				})
			]
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 0, 0, 4)
		[
			SNew(SMultiLineEditableTextBox)
			.Text_Lambda([Command]()
				{
					return Command->Text;
				})
			.OnTextCommitted_Lambda([Command](const FText& NewText, ETextCommit::Type)
				{
					Command->Modify();
					Command->Text = NewText;
				})
		];
}


TSharedRef<SWidget> SGraphNode_RPGCommand::CreateStartConvoBody(UStartConvo* Command)
{
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 0, 0, 2)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Convo Asset")))
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 0, 0, 2)
		[
			SNew(SObjectPropertyEntryBox)
			.AllowedClass(URPGConvoAsset::StaticClass())
			.ObjectPath_Lambda([Command]()
			{
				return Command->ConvoAsset? Command->ConvoAsset->GetPathName() : FString();
			})
			.OnObjectChanged_Lambda([this, Command](const FAssetData& AssetData)
			{
				Command->Modify();
				Command->ConvoAsset = Cast<URPGConvoAsset>(AssetData.GetAsset());

				if (Command->ConvoAsset)
				{
					RefreshNamedOptions(Command->ConvoAsset->GetConvoIds());
				}
			})
		]
		 + SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 0, 0, 2)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Convo Id")))
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(8, 0, 0, 2)
		[
			SNew(SComboBox<TSharedPtr<FName>>)
			.OptionsSource(&NamedOptions)
			.OnGenerateWidget_Lambda([](TSharedPtr<FName> Item)
			{
				return SNew(STextBlock)
					.Text(Item.IsValid()
						? FText::FromName(*Item)
						: FText::FromString(TEXT("Invalid")));
			})
			.OnSelectionChanged_Lambda([Command](TSharedPtr<FName> NewSelection, ESelectInfo::Type)
			{
				if (!NewSelection.IsValid())
				{
					return;
				}

				Command->Modify();
				Command->ConvoId = *NewSelection;
			})
			[
				SNew(STextBlock)
				.Text_Lambda([Command]()
				{
					return !Command->ConvoId.IsNone()
						? FText::FromName(Command->ConvoId)
						: FText::FromString(TEXT("Select..."));
				})
			]
		];
}

TSharedRef<SWidget> SGraphNode_RPGCommand::CreateMoveToBody(UMoveTo* Command)
{
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 0, 0, 2)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Target")))
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(8, 0, 0, 2)
		[
			SNew(SComboBox<TSharedPtr<FName>>)
			.OptionsSource(&NamedOptions)
			.OnGenerateWidget_Lambda([](TSharedPtr<FName> Item)
			{
				return SNew(STextBlock)
					.Text(Item.IsValid()? FText::FromName(*Item) : FText::FromString(TEXT("Invalid")));
			})
			.OnSelectionChanged_Lambda([Command](TSharedPtr<FName> NewSelection, ESelectInfo::Type)
			{
				if (!NewSelection.IsValid())
				{
					return;
				}
				Command->Modify();

				if (const UEnum* Enum = StaticEnum<ETargetToMove>())
				{
					const int64 Value = Enum->GetValueByNameString(NewSelection->ToString());
					if (Value != INDEX_NONE)
					{
						Command->Target = static_cast<ETargetToMove>(Value);
					}
				}
			})
		];
}