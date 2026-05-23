// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/WidgetTree.h"
#include "Data/RPGDatabase.h"
#include "GameFramework/GameModeBase.h"

#include "RPGEngine.generated.h"

/**
 * 
 */
UCLASS(config = Game, notplaceable, BlueprintType, Blueprintable, Transient)
class RPGSYSTEM_API ARPGEngine : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARPGEngine();

	//GameModeBase
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);

	///** Please add a variable description */
	//UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	//TObjectPtr<USceneComponent> DefaultSceneRoot;

	///** Please add a variable description */
	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Actors")
	//TObjectPtr<ACharacter> Player;

	///** Please add a variable description */
	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Actors")
	//TArray<AWC_NPC_C*> NPCs;

	///** Please add a variable description */
	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	//TEnumAsByte<WC_Mode> ActiveMode;

	///** Please add a variable description */
	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(EditInline="true"))
	//TObjectPtr<UWidget> ActiveWidget;

private:
	UUserWidget* MessagePanel = nullptr;

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Database")
	URPGDatabase* Database;
};

