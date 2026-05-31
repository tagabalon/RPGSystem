// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "MessageInterface.generated.h"


class UShowChoices;
class UShowText;
class FText;
class URPGCommand;

USTRUCT(BlueprintType)
struct FRPGMessageData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message")
	FString Speaker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message", meta = (MultiLine = true))
	FString Message;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message")
	TObjectPtr<UTexture2D> BustSprite = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message")
	TObjectPtr<USoundBase> VoiceAudio = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message")
	TObjectPtr<URPGCommand> Invoker = nullptr;


};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMessageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPGSYSTEM_API IMessageInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Message")
	void ShowMessage(const FRPGMessageData& MessageData);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Message")
	void ShowText(UShowText* ShowTextData);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Message")
	void ShowChoices(UShowChoices* ShowChoicesData);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Message")
	void ShowCaption(const FText& Speaker, const FText& Text);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Message")
	void CloseMessages();
};
