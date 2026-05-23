#pragma once

#include "BaseCommand.h"

#include "Sound/SoundBase.h"

#include "PlaySpeech.generated.h"

class AMapEventActor;
class APlayerController;
class UAudioComponent;

UENUM(BlueprintType)
enum class ESpeechSource : uint8
{
	Self,
	MainCharacter,
	PersonaCharacter,
	RegisteredNPC,
	Actor
};

/*
*/
UCLASS(Blueprintable, Category="RPG System")
class RPGSYSTEM_API UPlaySpeech : public UBaseCommand
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Speech")
	USoundBase* Speech;

	UPROPERTY(EditAnywhere, Category="Speech")
	FText Text;

	UPROPERTY(EditAnywhere, Category="Speech")
	ESpeechSource SpeechSource;

	UPROPERTY(EditAnywhere, Category = "Speech", meta=(EditCondition="SpeechSource == ESpeechSource::RegisteredNPC"))
	FName NPCName;

	virtual void Execute(APlayerController* InPlayer, AMapEventActor* InMapEventActor);

	bool IsUINeeded() { return true; }

	//BaseCommand
	virtual FText GetCategory() const
	{
		return CategoryCutscene;
	}

	virtual FText GetLabel() const {
		return FText::FromString(TEXT("Play Speech"));
	}

	UFUNCTION()
	void OnSpeechPlayed();

	FScriptDelegate SpeechPlayed;
	UAudioComponent* AudioComponent = nullptr;
	APlayerController* Player;
	AMapEventActor* MapEventActor;
};