#pragma once

#include "CoreMinimal.h"
#include "Data/RPGDerivedStats.h"
#include "Gameplay/RPGInventory.h"
#include "Gameplay/RPGPartyMember.h"
#include "Templates/SharedPointer.h"

#include "RPGPartySubsystem.generated.h"

class URPGDatabase;


UCLASS()
class RPGSYSTEM_API URPGPartySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "RPG|Party")
	void InitializeNewGame(const URPGDatabase* Database);

	UFUNCTION(BlueprintCallable, Category = "RPG|Party")
	bool AddPartyMember(FName ActorId, int32 Level = 1);

	UFUNCTION(BlueprintCallable, Category = "RPG|Party")
	bool RemovePartyMember(FName ActorId);

	UFUNCTION(BlueprintPure, Category = "RPG|Party")
	bool HasPartyMember(FName ActorId) const;

	//UFUNCTION(BlueprintPure, Category = "RPG|Party")
	TArray<TSharedPtr<FRPGPartyMember>> GetPartyMembers() const { return PartyMembers; }

	UFUNCTION(BlueprintCallable, Category = "RPG|Inventory")
	void AddItem(FName ItemId, int32 Quantity = 1);

	UFUNCTION(BlueprintCallable, Category = "RPG|Inventory")
	bool RemoveItem(FName ItemId, int32 Quantity = 1);

	UFUNCTION(BlueprintPure, Category = "RPG|Inventory")
	int32 GetItemQuantity(FName ItemId) const;

	UFUNCTION(BlueprintPure, Category = "RPG|Gold")
	int32 GetGold() const { return Inventory.Gold; }

	UFUNCTION(BlueprintCallable, Category = "RPG|Gold")
	void AddGold(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "RPG|Gold")
	bool SpendGold(int32 Amount);

protected:
	UFUNCTION()
	void InitializePartyMember(const URPGDatabase* Database, const FName& CharacterId);

	UFUNCTION()
	void InitializeBaseStats(TArray<int32>& BaseStats, const FRPGClassData& ClassData, int32 Level);

	UFUNCTION()
	void InitializeEqupment(const URPGDatabase* Database, FName CharacterId, FRPGCharacterEquipment& Equipment);

private:
	TArray<TSharedPtr<FRPGPartyMember>> PartyMembers;

	UPROPERTY()
	FRPGInventory Inventory;

	UPROPERTY()
	float CombatStartingStamina = 0;
};