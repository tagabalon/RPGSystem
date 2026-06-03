#pragma once

#include "RPGCommand.h"

#include "ShowText.generated.h"

class AMapEventActor;
/*
*/
UCLASS(Blueprintable, Category="RPG System")
class RPGSYSTEM_API UShowText : public URPGCommand
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Message")
	FText SpeakerName = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Message")
	FText Text = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Message")
	UTexture2D* Face = nullptr;
	
public:
	void SetName(FText InName) { SpeakerName = InName; }
	void SetText(FText InText) { Text = InText; }
	//virtual void Execute(APlayerController* Player, AMapEventActor* MapEventActor);

	bool IsUINeeded() { return true; }

	virtual ERPGCommandResult Execute_Implementation(AActor* TriggerActor, ARPGFieldCharacter* InstigatorActor);

private:
    UUserWidget* GetMessageWidget(ARPGFieldCharacter* InstigatorActor) const;

#if WITH_EDITOR
public:
	virtual FText GetNodeTitle() const
	{
		return FText::FromString(TEXT("Show Text"));
	}


#endif // WITH_EDITOR
};