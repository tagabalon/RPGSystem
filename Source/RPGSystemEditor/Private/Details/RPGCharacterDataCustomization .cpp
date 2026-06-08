#include "RPGCharacterDataCustomization.h"

#include "Data/RPGClasses.h"
#include "Data/RPGDatabase.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "PropertyHandle.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Text/STextBlock.h"

TSharedRef<IPropertyTypeCustomization> FRPGCharacterDataCustomization::MakeInstance()
{
	return MakeShared<FRPGCharacterDataCustomization>();
}

void FRPGCharacterDataCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow,IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	HeaderRow
		.NameContent()
		[
			StructPropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			SNew(STextBlock)
				.Text(FText::FromString(TEXT("Character Data")))
		];
}

void FRPGCharacterDataCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	RefreshClassOptions();

	uint32 NumChildren = 0;
	StructPropertyHandle->GetNumChildren(NumChildren);

	for (uint32 Index = 0; Index < NumChildren; ++Index)
	{
		TSharedPtr<IPropertyHandle> ChildHandle = StructPropertyHandle->GetChildHandle(Index);

		if (!ChildHandle.IsValid())
		{
			continue;
		}

		if (ChildHandle->GetProperty()->GetFName() == TEXT("RPGClassId"))
		{
			ChildBuilder.AddCustomRow(FText::FromString(TEXT("Class")))
				.NameContent()
				[
					SNew(STextBlock)
						.Text(FText::FromString(TEXT("Class")))
				]
				.ValueContent()
				.MinDesiredWidth(250.f)
				[
					MakeClassCombo(ChildHandle.ToSharedRef())
				];

			continue;
		}

		ChildBuilder.AddProperty(ChildHandle.ToSharedRef());
	}
}

void FRPGCharacterDataCustomization::RefreshClassOptions()
{
	ClassOptions.Reset();

	if (const URPGDatabase* Database = URPGDatabase::GetRPGDatabase())
	{
		if (URPGClasses* Classes = Database->ClassesDatabase.LoadSynchronous())
		{
			TArray<FString> ClassNames = Classes->GetClassNames();
			for (FString ClassName : ClassNames)
			{
				ClassOptions.Add(MakeShared<FName>(ClassName));
			}
		}
	}
}

TSharedRef<SWidget> FRPGCharacterDataCustomization::MakeClassCombo(
	TSharedRef<IPropertyHandle> ClassIdHandle
)
{
	return SNew(SComboBox<TSharedPtr<FName>>)
		.OptionsSource(&ClassOptions)
		.OnGenerateWidget_Lambda([](TSharedPtr<FName> Item)
			{
				return SNew(STextBlock)
					.Text(Item.IsValid()
						? FText::FromName(*Item)
						: FText::FromString(TEXT("Invalid")));
			})
		.OnSelectionChanged_Lambda([ClassIdHandle](TSharedPtr<FName> NewSelection, ESelectInfo::Type)
			{
				if (!NewSelection.IsValid())
				{
					return;
				}

				ClassIdHandle->SetValue(*NewSelection);
			})
		[
			SNew(STextBlock)
				.Text_Lambda([this, ClassIdHandle]()
					{
						return GetClassComboText(ClassIdHandle);
					})
		];
}

FText FRPGCharacterDataCustomization::GetClassComboText(
	TSharedRef<IPropertyHandle> ClassIdHandle
) const
{
	FName CurrentValue;
	if (ClassIdHandle->GetValue(CurrentValue) == FPropertyAccess::Success && !CurrentValue.IsNone())
	{
		return FText::FromName(CurrentValue);
	}

	return FText::FromString(TEXT("Select class..."));
}