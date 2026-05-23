// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MapEventActor.h"

#include "Actors/MainCharacter.h"
#include "Interface/InteractInterface.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMapEventActor::AMapEventActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetNestedDefaultSubobjectClass<USceneComponent>(TEXT("DefaultSceneRoot")))
	,  MapEvent(nullptr)
	, TriggerCondition(ETriggerCondition::Interact)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CreateAwarenessSphere();

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetFinder(TEXT("WidgetBlueprint'/RPGSystem/Widgets/Interact/W_InteractPrompt.W_InteractPrompt_C'"));
	if (WidgetFinder.Succeeded())
	{
		PromptWidget = WidgetFinder.Class;
		CreateInteractPrompt();
	}
}

void AMapEventActor::CreateAwarenessSphere()
{
	if(AwarenessSphere == nullptr)
	{ 
		AwarenessSphere = CreateDefaultSubobject<USphereComponent>(FName("AwarenessSphere"));
		AwarenessSphere->SetupAttachment(RootComponent);
	}
	AwarenessSphere->InitSphereRadius(GlintDistance);
}

void AMapEventActor::CreateInteractPrompt()
{
	if(Prompt == nullptr)
	{
		Prompt = CreateDefaultSubobject<UWidgetComponent>(FName("Prompt"));
		Prompt->SetupAttachment(RootComponent);
		Prompt->SetWidgetSpace(EWidgetSpace::Screen);
		Prompt->SetDrawSize(FVector2D(140.f, 100.0f));
		Prompt->SetWidgetClass(PromptWidget);
		Prompt->SetWorldLocation(GetActorLocation());
	}
}

void AMapEventActor::InitializeInteract()
{
	bIsCharacterPrompted = false;

	if (TriggerCondition == ETriggerCondition::Interact || TriggerCondition == ETriggerCondition::Proximity)
	{
		AwarenessSphere->OnComponentBeginOverlap.AddDynamic(this, &AMapEventActor::OnSphereOverlap);
		AwarenessSphere->OnComponentEndOverlap.AddDynamic(this, &AMapEventActor::OnSphereLeave);
	}

	if (UUserWidget* PromptObject = Prompt->GetUserWidgetObject())
	{
		if (PromptObject->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
		{
			InteractWidget = PromptObject;
		}
	}
}

// Called when the game starts or when spawned
void AMapEventActor::BeginPlay()
{
	Super::BeginPlay();

	InitializeInteract();
	
}

// Called every frame
void AMapEventActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsCharacterPrompted)
	{
		if (!CharacterCanBePrompted())
		{
			IInteractInterface::Execute_ShowGlint(InteractWidget);
			GlintedCharacter->SetInteractable(nullptr);
			bIsCharacterPrompted = false;
		}
	}
	else
	{
		if (CharacterCanBePrompted())
		{
			IInteractInterface::Execute_ShowPrompt(InteractWidget);
			GlintedCharacter->SetInteractable(this);
			bIsCharacterPrompted = true;
		}
	}
}

bool AMapEventActor::CharacterCanBePrompted() const
{
	if (GlintedCharacter != nullptr)
	{
		float squareDistance = (GetActorLocation() - GlintedCharacter->GetActorLocation()).SquaredLength();
		return squareDistance < PromptDistance * PromptDistance;
	}
	return false;
}


void AMapEventActor::OnInteract()
{
	if (MapEvent != nullptr && !bIsEventRunning)
	{
		MapEvent->Execute(UGameplayStatics::GetPlayerController(this, 0), this);
		IInteractInterface::Execute_HideInteract(InteractWidget);
		bIsEventRunning = true;
	}
}

void AMapEventActor::SetPromptDistance(float InPromptDistance)
{
	PromptDistance = InPromptDistance;
	AwarenessSphere->SetSphereRadius(PromptDistance);
}

void AMapEventActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(AMainCharacter::StaticClass()))
	{
		GlintedCharacter = Cast<AMainCharacter>(OtherActor);
		bIsCharacterPrompted = false;

		IInteractInterface::Execute_ShowGlint(InteractWidget);
	}
}

void AMapEventActor::OnSphereLeave(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IInteractInterface::Execute_HideInteract(InteractWidget);
	GlintedCharacter = nullptr;
	bIsCharacterPrompted = false;

}

#if WITH_EDITOR

void AMapEventActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (PropertyName == FName("GlintDistance"))
	{
		AwarenessSphere->InitSphereRadius(GlintDistance);
	}
	else if (PropertyName == FName("PromptWidget"))
	{
		/*if (UUserWidget::StaticClass() !=  PropertyChangedEvent.GetObjectBeingEdited(0)->GetClass())
		{
		}*/
	}
	else if (PropertyName == FName("TriggerCondition"))
	{
		if (TriggerCondition == ETriggerCondition::Interact || TriggerCondition == ETriggerCondition::Proximity)
		{
			CreateAwarenessSphere();
			CreateInteractPrompt();
		}
	}
}

#endif