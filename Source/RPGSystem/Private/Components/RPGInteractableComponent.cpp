#include "Components/RPGInteractableComponent.h"

#include "Actors/RPGFieldCharacter.h"
#include "RPGSettings.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values for this component's properties
URPGInteractableComponent::URPGInteractableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	TriggerSphere->SetupAttachment(this);

	TriggerSphere->SetSphereRadius(AwarenessRadius);
	TriggerSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerSphere->SetCollisionObjectType(ECC_WorldDynamic);
	TriggerSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerSphere->SetGenerateOverlapEvents(true);
	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this,	&URPGInteractableComponent::OnEnterInteractable);
	TriggerSphere->OnComponentEndOverlap.AddDynamic(this, &URPGInteractableComponent::OnExitInteractable);

	if (const URPGSettings* RPGSettings = GetDefault<URPGSettings>())
	{
		if (!RPGSettings->InteractWidgetAsset.IsNull())
		{
			WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("TriggerPrompt"));
			WidgetComponent->SetupAttachment(this);

			WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
			WidgetComponent->SetDrawSize(FVector2D(140.f, 100.0f));
			//WidgetComponent->SetWorldLocation(GetActorLocation());

			if (UClass* WidgetClass = RPGSettings->InteractWidgetAsset.LoadSynchronous())
			{
				if (WidgetClass->ImplementsInterface(UInteractInterface::StaticClass()))
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
	}
}


// Called when the game starts
void URPGInteractableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (WidgetComponent)
	{
		WidgetComponent->InitWidget();
		InteractWidget = Cast<URPGInteractWidget>(WidgetComponent->GetUserWidgetObject());

		if (InteractWidget)
		{
			IInteractInterface::Execute_HideInteract(InteractWidget);
		}
	}

	SetComponentTickEnabled(false);
}


// Called every frame
void URPGInteractableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (bIsInProximity && TriggerSource && InteractWidget)
	{
		if (IsPromptable(TriggerSource))
		{
			bIsPrompted = true;
			IInteractInterface::Execute_ShowInteract(InteractWidget, PromptText.ToString());
			OnPromptVisibilityChanged.Broadcast(TriggerSource, true);
		}
		else if(bIsPrompted)
		{
			bIsPrompted = false;
            IInteractInterface::Execute_HideInteract(InteractWidget);
			OnPromptVisibilityChanged.Broadcast(nullptr, false);
		}
    }
}

void URPGInteractableComponent::SetAwarenessRadius(float Radius)
{
    AwarenessRadius = Radius;
	if (TriggerSphere)
	{
		TriggerSphere->SetSphereRadius(AwarenessRadius);
    }
}

void URPGInteractableComponent::EnableInteract(bool Enabled)
{
	TriggerSphere->SetCollisionEnabled(Enabled ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	TriggerSphere->SetGenerateOverlapEvents(Enabled);

	bIsPrompted = false;

	if (InteractWidget)
	{
		IInteractInterface::Execute_HideInteract(InteractWidget);
	}
}

bool URPGInteractableComponent::IsPromptable(const ARPGFieldCharacter* OtherCharacter) const
{
	if (OtherCharacter)
	{
		float squareDistance = (GetComponentLocation() - OtherCharacter->GetActorLocation()).SquaredLength();
		if (squareDistance < PromptDistance * PromptDistance)
		{
			FVector VectorToOther = (GetComponentLocation() - OtherCharacter->GetActorLocation());
			VectorToOther.Normalize();

			float DotProduct = FVector::DotProduct(VectorToOther, OtherCharacter->GetActorForwardVector());
			float Angle = FMath::RadiansToDegrees(FMath::Acos(DotProduct));
			return FMath::Abs(Angle) < 45.0f;
		}
	}

	return false;
}

void URPGInteractableComponent::OnEnterInteractable(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
	TriggerSource = OverlappingCharacter;

	UE_LOG(LogTemp, Verbose, TEXT("CheckRequirements"));
	if (OnCheckTriggerCondition.IsBound())
	{
		if (!OnCheckTriggerCondition.Execute())
		{
			UE_LOG(LogTemp, Verbose, TEXT("CheckRequirements failed."));
			return;
		}
    }
	if (bPromptForAction)
	{
		bIsInProximity = true;
		bIsPrompted = false;

        SetComponentTickEnabled(true);

		UE_LOG(LogTemp, Verbose, TEXT("Player entered proximity of %s."), *GetName());
	}
	else
	{
		OnInteractPressed.Broadcast(TriggerSource);
	}
}

void URPGInteractableComponent::OnExitInteractable(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex
)
{
	ARPGFieldCharacter* OverlappingCharacter = Cast<ARPGFieldCharacter>(OtherActor);
	if (!OverlappingCharacter)
	{
		return;
	}

	TriggerSource = nullptr;

	bIsInProximity = false;
	SetComponentTickEnabled(false);

	UE_LOG(LogTemp, Verbose, TEXT("Player exited proximity of %s."), *GetName());
}

#if WITH_EDITOR
void URPGInteractableComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.GetPropertyName() : NAME_None;

	// Check if a specific property inside this component changed
	if (PropertyName == GET_MEMBER_NAME_CHECKED(URPGInteractableComponent, AwarenessRadius))
	{
        SetAwarenessRadius(AwarenessRadius);
	}
}
#endif
