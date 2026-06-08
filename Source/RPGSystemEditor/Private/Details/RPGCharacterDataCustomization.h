#pragma once

#include "CoreMinimal.h"
#include "Data/RPGCharacters.h"
#include "IPropertyTypeCustomization.h"

class IPropertyHandle;
class IDetailChildrenBuilder;
class IPropertyTypeCustomizationUtils;
class FDetailWidgetRow;

class FRPGCharacterDataCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

private:
	TArray<TSharedPtr<FName>> ClassOptions;

	void RefreshClassOptions();

	TSharedRef<SWidget> MakeClassCombo(TSharedRef<IPropertyHandle> ClassIdHandle);
	FText GetClassComboText(TSharedRef<IPropertyHandle> ClassIdHandle) const;
};