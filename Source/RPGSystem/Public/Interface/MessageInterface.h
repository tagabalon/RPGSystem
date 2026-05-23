// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "MessageInterface.generated.h"

class UShowChoices;
class UShowText;
class FText;

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
	void ShowText(UShowText* ShowTextData);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Message")
	void ShowChoices(UShowChoices* ShowChoicesData);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Message")
	void ShowCaption(const FText& Speaker, const FText& Text);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Message")
	void CloseMessages();
};
