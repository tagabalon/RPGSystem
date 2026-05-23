#pragma once

#include "BaseCommand.h"
#include "ShowText.generated.h"

class AMapEventActor;
/*
*/
UCLASS(Blueprintable, Category="RPG System")
class RPGSYSTEM_API UShowText : public UBaseCommand
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Message")
	FText Name = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Message")
	FText Text = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Message")
	UTexture2D* Face = nullptr;
	
public:
	void SetName(FText InName) { Name = InName; }
	void SetText(FText InText) { Text = InText; }
	virtual void Execute(APlayerController* Player, AMapEventActor* MapEventActor);

	bool IsUINeeded() { return true; }

	//BaseCommand
	virtual FText GetCategory() const
	{
		return CategoryMessage;
	}

	virtual FText GetLabel() const {
		return FText::FromString(TEXT("Show Text"));
	}

};