#pragma once

#include "BranchCommand.h"
#include "Branch/DialogueChoice.h"
#include "Commands/Branch.h"

#include "ShowChoices.generated.h"

class AMapEventActor;

UCLASS(Blueprintable, Category="RPG System")
class RPGSYSTEM_API UShowChoices : public UBranchCommand
{
	GENERATED_UCLASS_BODY()

public:
	virtual void Execute(APlayerController* InPlayer, AMapEventActor* InMapEventActor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Message")
	TArray<UDialogueChoice*> Choices;

#if WITH_EDITOR
	DECLARE_DELEGATE(FOnChoicesChanged);
	FOnChoicesChanged OnChoicesChanged;
#endif

protected:
	virtual void SelectBranch(int32 SelectedChoice) override;

	//UBranchCommand
	int32 GetBranchSize() const;
	//FName GetBranchLabel(uint32 InIndex) const;

	//UBaseCommand
	bool IsUINeeded() { return true; }
	bool IsBranching() { return true; }

	virtual FText GetCategory() const
	{
		return CategoryMessage;
	}

	virtual FText GetLabel() const {
		return FText::FromString(TEXT("Show Choices"));
	}

#if WITH_EDITOR
	void FillChoices();
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& e);
	bool GetBranchByPin(const UEdGraphPin* InPin, FBranch& OutBranch) const;
	void SetBranchCommand(UBaseCommand* Command, int32 InIndex);
	UBaseCommand* GetBranchCommand(int32 Index) const;
	void SetBranch(int32 InIndex, UEdGraphPin* Pin);
	void SynchronizeBranches();
	int32 GetBranchByParam(const UBranchParameter* BranchParameter, FBranch& Branch) const;
#endif

	APlayerController* Player = nullptr;
	AMapEventActor* MapEvent = nullptr;
};
