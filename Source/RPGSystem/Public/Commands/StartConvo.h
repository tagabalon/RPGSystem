#pragma once

#include "RPGCommand.h"

#include "StartConvo.generated.h"

class ARPGFieldCharacter;
class URPGConvoAsset;
struct FRPGConvo;

UCLASS(Blueprintable, Category = "RPG System")
class RPGSYSTEM_API UStartConvo : public URPGCommand
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TObjectPtr<URPGConvoAsset> ConvoAsset = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (GetOptions = "GetConvoOptions"), Category = "Dialogue")
	FName ConvoId;

	virtual ERPGCommandResult Execute_Implementation(AActor* TriggerActor, ARPGFieldCharacter* InstigatorActor) override;

	virtual ERPGCommandResult Continue_Implementation() override;

	UFUNCTION()
	TArray<FName> GetConvoOptions() const;

private:
	UUserWidget* GetMessageWidget(ARPGFieldCharacter* InstigatorActor) const;
	void ShowCurrentLine();

	UPROPERTY()
	int32 CurrentLineIndex = 0;

	const FRPGConvo* RuntimeConvo = nullptr;

	UPROPERTY()
	TObjectPtr<ARPGFieldCharacter> Instigator;

	UPROPERTY()
	TObjectPtr<UUserWidget> MessageWidget;

};