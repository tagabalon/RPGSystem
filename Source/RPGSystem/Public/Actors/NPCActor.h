// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Actors/MapEventActor.h"

#include "GameFramework/Actor.h"
#include "NPCActor.generated.h"


class UAudioComponent;
class UCapsuleComponent;
class UCharacterMovementComponent;
class USkeletalMesh;
class USkeletalMeshComponent;

UCLASS()
class RPGSYSTEM_API ANPCActor : public AMapEventActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANPCActor(const FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditDefaultsOnly, Category="Map Event")
	USkeletalMesh* CharacterMesh = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, export, Category = "Map Event")
	FText PrompText = FText::FromString("Interact");

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
protected:
	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(Category=Character, EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh;

	/** Movement component used for movement logic in various movement modes (walking, falling, etc), containing relevant settings and functions to control movement. */
	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UCharacterMovementComponent> CharacterMovement;

	/** The CapsuleComponent being used for movement collision (by CharacterMovement). Always treated as being vertically aligned in simple collision check functions. */
	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	/** The CapsuleComponent being used for movement collision (by CharacterMovement). Always treated as being vertically aligned in simple collision check functions. */
	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UAudioComponent> AudioSource;

private:
	UPROPERTY()
	USceneComponent* AwarenessRoot;

};
