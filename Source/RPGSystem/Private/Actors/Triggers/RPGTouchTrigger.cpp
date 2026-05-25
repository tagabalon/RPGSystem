#include "Actors/Triggers/RPGTouchTrigger.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Gameplay/RPGTriggerRunnerSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "RPGGameInstance.h"
#include "RPGSettings.h"

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
    CreateInteractPrompt();
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

	if (TObjectPtr<URPGPromptWidget> Prompt = GetInteractPromptWidget())
	{
        Prompt->HidePrompt();
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

TObjectPtr<UWidgetComponent> ARPGTouchTrigger::CreateInteractPrompt()
{
	if (const URPGSettings* RPGSettings = GetDefault<URPGSettings>())
	{
		if (RPGSettings->PromptWidgetAsset.IsNull())
		{
			UE_LOG(
				LogTemp,
				Error,
				TEXT("%s: URPGSettings is missing TriggerPromptWidget reference."),
				*GetName()
			);
			return nullptr;
		}

		PromptWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TriggerPrompt"));

		PromptWidget->SetupAttachment(RootComponent);
		PromptWidget->SetWidgetSpace(EWidgetSpace::Screen);
		PromptWidget->SetDrawSize(FVector2D(140.f, 100.0f));
		PromptWidget->SetWorldLocation(GetActorLocation());

		if (UClass* WidgetClass = RPGSettings->PromptWidgetAsset.LoadSynchronous())
		{
			PromptWidget->SetWidgetClass(WidgetClass);
		}
	}

	return PromptWidget;
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
		float squareDistance = (GetActorLocation() - PendingTriggeringActor->GetActorLocation()).SquaredLength();
		if (IsPromptable(PendingTriggeringActor))
		{
            Prompt(PendingTriggeringActor);
		}
		else
		{
			UnPrompt();
		}
	}
}

void ARPGTouchTrigger::OnTriggerBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	/*if (!OtherActor || !TriggerData || bThreadRunning)
	{
		return;
	}*/

	/*if (!OtherActor->ActorHasTag(TEXT("Player")))
	{
		return;
	}*/

	UE_LOG(LogTemp, Verbose, TEXT("CheckRequirements"));
	if (CheckRequirements())
	{
		if (bPromptForAction)
		{
			bIsInProximity = true;
            PendingTriggeringActor = OtherActor;
			SetActorTickEnabled(true);

			UE_LOG(LogTemp, Verbose, TEXT("Player entered proximity of %s."), *GetName());
		}
		else
		{
			Interact(OtherActor);
		}
	}
	else if (!bPromptForAction)
	{
		PendingTriggeringActor = OtherActor;
	}
}

void ARPGTouchTrigger::OnTriggerEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	if (!OtherActor/* || !OtherActor->ActorHasTag(TEXT("Player"))*/)
	{
		return;
	}

	if (bPrompted)
	{
		UnPrompt();
	}

	if (PendingTriggeringActor == OtherActor)
	{
		PendingTriggeringActor = nullptr;
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
		float squareDistance = (GetActorLocation() - PendingTriggeringActor->GetActorLocation()).SquaredLength();
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

TObjectPtr<URPGPromptWidget> ARPGTouchTrigger::GetInteractPromptWidget() const
{
	if (PromptWidget)
	{
		return Cast<URPGPromptWidget>(PromptWidget->GetUserWidgetObject());
	}
	return nullptr;
}

void ARPGTouchTrigger::Prompt(AActor* InstigatorActor)
{
	/*if (!InstigatorActor)
	{
		return;
	}*/

	if (TObjectPtr<URPGPromptWidget> Prompt = GetInteractPromptWidget())
	{
        Prompt->SetPromptText(PromptText);
        Prompt->ShowPrompt();
	}

	// TODO:
	// Replace with your UI subsystem / HUD call.
	// Example:
	// URPGUISubsystem::Get(this)->ShowPrompt(PromptText, this);

	bPrompted = true;
}

void ARPGTouchTrigger::UnPrompt()
{
	// TODO:
	// Replace with your UI subsystem / HUD call.
	// URPGUISubsystem::Get(this)->HidePrompt();

	if (TObjectPtr<URPGPromptWidget> Prompt = GetInteractPromptWidget())
	{
		Prompt->HidePrompt();
	}
	bPrompted = false;
}

void ARPGTouchTrigger::Interact(AActor* InstigatorActor)
{
	if (!TriggerData || bThreadRunning)
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

	TriggerSphere->SetCollisionEnabled(
		bActive ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision
	);

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