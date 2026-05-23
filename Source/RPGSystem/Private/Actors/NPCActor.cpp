// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/NPCActor.h"

#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ANPCActor::ANPCActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetNestedDefaultSubobjectClass<UCapsuleComponent>(TEXT("DefaultSceneRoot")))
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (RootComponent == nullptr)
	{
		RootComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DefaultSceneRoot"));
	}
	CapsuleComponent = CastChecked<UCapsuleComponent>(RootComponent);
	CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
	CapsuleComponent->SetCapsuleRadius(32.0f);
	CapsuleComponent->bDynamicObstacle = true;

	AwarenessRoot = CreateDefaultSubobject<USceneComponent>(TEXT("AwarenessTrigger"));
	AwarenessRoot->SetupAttachment(RootComponent);
	if (AwarenessSphere != nullptr)
	{
		AwarenessSphere->SetupAttachment(AwarenessRoot);
	}

	/*AwarenessSphere->SetupAttachment(CapsuleComponent);
	Prompt->SetupAttachment(CapsuleComponent);*/


	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetFinder(TEXT("WidgetBlueprint'/RPGSystem/Widgets/Interact/W_InteractPrompt.W_InteractPrompt_C'"));
	if (WidgetFinder.Succeeded())
	{
		PromptWidget = WidgetFinder.Class;
		CreateInteractPrompt();
	}

	CharacterMovement = CreateDefaultSubobject<UCharacterMovementComponent>("Character Movement");
	if (CharacterMovement != nullptr)
	{
		CharacterMovement->UpdatedComponent = CapsuleComponent;
	}

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	if (Mesh != nullptr)
	{
		if (CharacterMesh != nullptr)
		{
			Mesh->SetSkeletalMesh(CharacterMesh);
		}
		Mesh->AlwaysLoadOnClient = true;
		Mesh->AlwaysLoadOnServer = true;
		Mesh->SetupAttachment(RootComponent);
	}

	AudioSource = CreateDefaultSubobject<UAudioComponent>("AudioSource");
	AudioSource->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ANPCActor::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ANPCActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

