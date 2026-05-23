#pragma once

#include "BaseCommand.h"
#include "BranchCommand.generated.h"

class AMapEventActor;
class UBranchParameter;
struct FBranch;

/*
*/
UCLASS(Abstract, Blueprintable)
class RPGSYSTEM_API UBranchCommand : public UBaseCommand
{
	GENERATED_UCLASS_BODY()

public:
	const TArray<FBranch>& GetBranches() const { return Branches; }
	virtual int32 GetBranchSize() const { return 0; }

	UFUNCTION(BlueprintCallable)
	virtual void SelectBranch(int32 SelectedBranch) {}

	UPROPERTY()
	TArray<FBranch> Branches;

#if WITH_EDITOR
	void SetBranchPin(uint32 InIndex, UEdGraphPin* Pin) {}
	bool GetBranchByPin(const UEdGraphPin* InPin, FBranch& OutBranch) const { return false; }
	int32 GetBranchByParam(const UBranchParameter* BranchParameter, FBranch& Branch) const { return -1; }
	virtual void SetBranchCommand(UBaseCommand* Command, int32 Index) {};
	virtual UBaseCommand* GetBranchCommand(int32 Index) const { return nullptr; }
#endif
};