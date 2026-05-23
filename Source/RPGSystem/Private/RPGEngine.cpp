// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGEngine.h"

#include "Actors/MainCharacter.h"

#include "Interfaces/IPluginManager.h"
#include "Containers/StringConv.h"
#include "GameFramework/HUD.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RPGEngine)

//class ABP_MainCharacter;

ARPGEngine::ARPGEngine()
{
	static ConstructorHelpers::FClassFinder<ACharacter> PawnFinder(TEXT("/Script/Engine.Blueprint'/RPGSystem/Blueprints/BP_MainCharacter.BP_MainCharacter_C'"));
	if (PawnFinder.Succeeded())
		DefaultPawnClass = PawnFinder.Class;

	static ConstructorHelpers::FClassFinder<AHUD> HUDFinder(TEXT("/Script/Engine.Blueprint'/RPGSystem/Blueprints/BP_HUD.BP_HUD_C'"));
	if (HUDFinder.Succeeded())
		HUDClass = HUDFinder.Class;
}

void ARPGEngine::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

}