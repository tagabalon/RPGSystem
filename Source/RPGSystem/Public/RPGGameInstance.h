#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RPGGameInstance.generated.h"

class URPGDatabase;
class URPGSaveGame;
class URPGPartySubsystem;

UCLASS()
class RPGSYSTEM_API URPGGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual void Init() override;
	virtual void Shutdown() override;

	UFUNCTION(BlueprintPure, Category = "RPG|Game Instance")
	static URPGGameInstance* GetRPGGameInstance(const UObject* WorldContextObject);

	UPROPERTY(BlueprintReadOnly, Category = "RPG|Data")
	TObjectPtr<URPGDatabase> Database;

	UPROPERTY(BlueprintReadOnly, Category = "RPG|Save")
	TObjectPtr<URPGSaveGame> CurrentSaveGame;

	UPROPERTY(BlueprintReadOnly, Category = "RPG|State")
	bool bIsPaused = false;

	UPROPERTY(BlueprintReadOnly, Category = "RPG|State")
	bool bIsLoading = false;

	UPROPERTY(BlueprintReadOnly, Category = "RPG|State")
	FString CurrentMapName;

	UPROPERTY(BlueprintReadOnly, Category = "RPG|State")
	FVector LastPlayerPosition = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "RPG|State")
	FRotator LastPlayerRotation = FRotator::ZeroRotator;

	UFUNCTION(BlueprintCallable, Category = "RPG|Data")
	URPGDatabase* LoadDatabase();

	UFUNCTION(BlueprintPure, Category = "RPG|Data")
	URPGDatabase* GetDatabase() const;

	UFUNCTION(BlueprintCallable, Category = "RPG|Flow")
	void SetPausedState(bool bPaused);

	UFUNCTION(BlueprintCallable, Category = "RPG|Flow")
	void SetLoadingState(bool bLoading);

	UFUNCTION(BlueprintCallable, Category = "RPG|Flow")
	void StorePlayerTransform(const FVector& Position, const FRotator& Rotation);

	UFUNCTION(BlueprintCallable, Category = "RPG|Save")
	bool SaveGameToSlot(const FString& SlotName, int32 UserIndex = 0);

	UFUNCTION(BlueprintCallable, Category = "RPG|Save")
	bool LoadGameFromSlot(const FString& SlotName, int32 UserIndex = 0);

private:

	TObjectPtr<URPGPartySubsystem> Party;
};