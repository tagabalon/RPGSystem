#include "Actors/Triggers/RPGTouchTrigger.h"

#include "Actors/RPGFieldCharacter.h"
#include "Gameplay/RPGTriggerRunnerSubsystem.h"
#include "RPGGameInstance.h"
#include "RPGSettings.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"


#if WITH_EDITOR
#include "AssetToolsModule.h"
#include "Factories/DataAssetFactory.h"
#include "Misc/PackageName.h"
#endif

ARPGTouchTrigger::ARPGTouchTrigger()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	CreateTriggerSphere();
	CreateInteractWidget();
}

void ARPGTouchTrigger::BeginPlay()
{
	Super::BeginPlay();

	bPrompted = false;
	bThreadFinished = false;

	if (!TriggerData)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is missing TriggerData."), *GetName());
		return;
	}

	if (WidgetComponent)
	{
		InteractWidget = Cast<URPGInteractWidget>(WidgetComponent->GetUserWidgetObject());

		UnPrompt();
	}

	bIsInProximity = false;
    bPrompted = false;

	SetTriggerActive(false);
	SetActorTickEnabled(false);
}

TObjectPtr<USphereComponent> ARPGTouchTrigger::CreateTriggerSphere()
{
	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	SetRootComponent(TriggerSphere);

	TriggerSphere->SetSphereRadius(300.0f);
	TriggerSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerSphere->SetCollisionObjectType(ECC_WorldDynamic);
	TriggerSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerSphere->SetGenerateOverlapEvents(true);

	TriggerSphere->OnComponentBeginOverlap.AddDynamic(
		this,
		&ARPGTouchTrigger::OnTriggerBeginOverlap
	);

	TriggerSphere->OnComponentEndOverlap.AddDynamic(
		this,
		&ARPGTouchTrigger::OnTriggerEndOverlap
	);

	return TriggerSphere;
}

TObjectPtr<UWidgetComponent> ARPGTouchTrigger::CreateInteractWidget()
{
	if (const URPGSettings* RPGSettings = GetDefault<URPGSettings>())
	{
		if (RPGSettings->InteractWidgetAsset.IsNull())
		{
			UE_LOG(
				LogTemp,
				Error,
				TEXT("%s: URPGSettings is missing TriggerPromptWidget reference."),
				*GetName()
			);
			return nullptr;
		}

		WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("TriggerPrompt"));

		WidgetComponent->SetupAttachment(RootComponent);
		WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		WidgetComponent->SetDrawSize(FVector2D(140.f, 100.0f));
		WidgetComponent->SetWorldLocation(GetActorLocation());

		if (UClass* WidgetClass = RPGSettings->InteractWidgetAsset.LoadSynchronous())
		{
			if(WidgetClass->ImplementsInterface(UInteractInterface::StaticClass()))
			{
				WidgetComponent->SetWidgetClass(WidgetClass);
			}
			else
			{
				UE_LOG(
					LogTemp,
					Error,
					TEXT("%s: InteractWidget class from URPGSettings does not implement UInteractInterface."),
					*GetName()
				);

				static ConstructorHelpers::FClassFinder<UUserWidget> DefaultWidgetAsset(TEXT("/RPGSystem/WBP_DefaultInteractWidget.WBP_DefaultInteractWidget"));

				if (DefaultWidgetAsset.Succeeded())
				{
					WidgetComponent->SetWidgetClass(DefaultWidgetAsset.Class);
				}
            }
		}
	}

	return WidgetComponent;
}

void ARPGTouchTrigger::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	/*if (PendingTriggeringActor && CheckRequirements())
	{
		AActor* ActorToTrigger = PendingTriggeringActor;
		PendingTriggeringActor = nullptr;

		Interact(ActorToTrigger);
	}*/

	if (bIsInProximity && !bPrompted)
	{
		float squareDistance = (GetActorLocation() - TriggeringActor->GetActorLocation()).SquaredLength();
		if (IsPromptable(TriggeringActor))
		{
            Prompt(TriggeringActor);
		}
		else
		{
			UnPrompt();
		}
	}
}

void ARPGTouchTrigger::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp,	int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult
)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Being overlap!"));
	}

    ARPGFieldCharacter* OverlappingCharacter = Cast<ARPGFieldCharacter>(OtherActor);
	if (!OverlappingCharacter)
	{
		return;
	}

	UE_LOG(LogTemp, Verbose, TEXT("CheckRequirements"));
	if (CheckRequirements())
	{
		if (bPromptForAction)
		{
			bIsInProximity = true;
			TriggeringActor = OverlappingCharacter;
			SetActorTickEnabled(true);

			UE_LOG(LogTemp, Verbose, TEXT("Player entered proximity of %s."), *GetName());
		}
		else
		{
			Interact(OverlappingCharacter);
		}
	}
}

void ARPGTouchTrigger::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex
)
{
	ARPGFieldCharacter* OverlappingCharacter = Cast<ARPGFieldCharacter>(OtherActor);
	if (!OverlappingCharacter)
	{
		return;
	}

	if (bPrompted)
	{
		UnPrompt();
	}

	if (TriggeringActor == OverlappingCharacter)
	{
		TriggeringActor = nullptr;
	}

	bIsInProximity = false;
	SetActorTickEnabled(false);

	UE_LOG(LogTemp, Verbose, TEXT("Player exited proximity of %s."), *GetName());
}

bool ARPGTouchTrigger::CheckRequirements() const
{
	if (FlagRequirement < 0)
	{
		return true;
	}

	const URPGGameInstance* RPGGameInstance =
		URPGGameInstance::GetRPGGameInstance(this);

	if (!RPGGameInstance)
	{
		return false;
	}

	// TODO:
	// Replace this with your future flag/save/subsystem function.
	// return RPGGameInstance->GetFlag(FlagRequirement);

	return true;
}

bool ARPGTouchTrigger::IsPromptable(const AActor* Actor) const
{
	if (Actor)
	{
		float squareDistance = (GetActorLocation() - TriggeringActor->GetActorLocation()).SquaredLength();
		if (squareDistance < PromptDistance * PromptDistance)
		{

			FVector VectorToMe = (GetActorLocation() - Actor->GetActorLocation());
			VectorToMe.Normalize();

			float DotProduct = FVector::DotProduct(VectorToMe, Actor->GetActorForwardVector());
			float Angle = FMath::RadiansToDegrees(FMath::Acos(DotProduct));
			return FMath::Abs(Angle) < 45.0f;
		}
	}

    return false;
}

void ARPGTouchTrigger::Prompt(AActor* InstigatorActor)
{
	if (InteractWidget)
	{
        IInteractInterface::Execute_ShowInteract(InteractWidget, PromptText.ToString());

		if (TriggeringActor == InstigatorActor)
		{
            TriggeringActor->SetInteractableTrigger(this);
		}

		bPrompted = true;
	}

}

void ARPGTouchTrigger::UnPrompt()
{
	if (InteractWidget)
	{
		IInteractInterface::Execute_HideInteract(InteractWidget);
	}

	if (TriggeringActor)
	{
		TriggeringActor->SetInteractableTrigger(nullptr);
	}
	bPrompted = false;
}

void ARPGTouchTrigger::Interact(ARPGFieldCharacter* InstigatorActor)
{
	if (!TriggerData || bThreadRunning || InstigatorActor != TriggeringActor)
	{
		return;
	}

	UnPrompt();

	bThreadRunning = true;
	bThreadFinished = false;

	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance)
	{
		return;
	}

	URPGTriggerRunnerSubsystem* TriggerRunner = GameInstance->GetSubsystem<URPGTriggerRunnerSubsystem>();

	if (!TriggerRunner)
	{
		return;
	}

	const bool bStarted = TriggerRunner->RunTrigger(this, InstigatorActor);	

	bRunning = bStarted;
}

void ARPGTouchTrigger::SetFinished(ERPGTriggerFinishAction FinishAction)
{
	switch (FinishAction)
	{
	case ERPGTriggerFinishAction::Hide:
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);
		break;

	case ERPGTriggerFinishAction::Repeat:
		break;

	case ERPGTriggerFinishAction::None:
	default:
		DisableTrigger();
		break;
	}

	bThreadRunning = false;
	bThreadFinished = true;
}

void ARPGTouchTrigger::DisableTrigger()
{
	if (TriggerSphere)
	{
		TriggerSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ARPGTouchTrigger::SetTriggerActive(bool bActive)
{
	if (!TriggerSphere)
	{
		return;
	}

	TriggerSphere->SetCollisionEnabled(bActive ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);

	TriggerSphere->SetGenerateOverlapEvents(bActive);
	PrimaryActorTick.bCanEverTick = bActive;
}

#if WITH_EDITOR
void ARPGTouchTrigger::PostActorCreated()
{
	Super::PostActorCreated();

	if (TriggerData)
	{
		return;
	}

	const FString AssetName = FString::Printf(
		TEXT("DA_Trigger_%s"),
		*GetActorLabel()
	);

	const FString PackagePath = TEXT("/Game/Data/Triggers");

	UDataAssetFactory* Factory = NewObject<UDataAssetFactory>();
	Factory->DataAssetClass = URPGTriggerData::StaticClass();

	FAssetToolsModule& AssetToolsModule =
		FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	
	UObject* NewAsset = AssetToolsModule.Get().CreateAsset(
		AssetName,
		PackagePath,
		URPGTriggerData::StaticClass(),
		Factory
	);

	TriggerData = Cast<URPGTriggerData>(NewAsset);
}
#endif