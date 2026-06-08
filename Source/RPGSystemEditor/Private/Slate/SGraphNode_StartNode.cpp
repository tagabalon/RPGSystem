#include "Slate/SGraphNode_StartNode.h"

#include "Graph/RPGStartGraphNode.h"

#include "SGraphPin.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"

void SGraphNode_StartNode::Construct(const FArguments& InArgs, URPGStartGraphNode* InNode)
{
	GraphNode = InNode;
	UpdateGraphNode();
}

void SGraphNode_StartNode::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();

	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);

	GetOrAddSlot(ENodeZone::Center)
	/*[
		SNew(SBorder)
		.Padding(8.f)
		.BorderImage(FAppStyle::GetBrush("Graph.Node.Body"))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("Start")))
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(16.f, 0.f, 0.f, 0.f)
			[
				SAssignNew(RightNodeBox, SVerticalBox)
			]
		]
	];*/
	[
		SNew(SBorder)
		.Padding(8.f)
		.BorderImage(FAppStyle::GetBrush("Graph.Node.Body"))
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SAssignNew(LeftNodeBox, SVerticalBox)
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(8.f, 0.f)
			[
				SNew(STextBlock)
					.Text(FText::FromString(TEXT("Start")))
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SAssignNew(RightNodeBox, SVerticalBox)
			]
		]
	];

	CreatePinWidgets();
}

void SGraphNode_StartNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));

	if (PinToAdd->GetDirection() == EGPD_Output)
	{
		RightNodeBox->AddSlot()
			.AutoHeight()
			.HAlign(HAlign_Right)
			[
				PinToAdd
			];

		OutputPins.Add(PinToAdd);
	}
}