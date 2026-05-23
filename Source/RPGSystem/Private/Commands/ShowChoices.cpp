#include "Commands/ShowChoices.h"

#include "Actors/MainCharacter.h"
#include "Actors/MapEventActor.h"
#include "Branch/DialogueChoice.h"
#include "Interface/MessageInterface.h"

#include "GameFramework/HUD.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ShowChoices)

UShowChoices::UShowChoices(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Choices.Add(nullptr);
	Branches.Emplace();

	Choices.Add(nullptr);
	Branches.Emplace();

	FillChoices();
	
	/*for (uint8 Index = 0; Index < 2; Index++)
	{
		uint8 NewIndex = Branches.Emplace();
		
		FString ChoiceName = FString::Printf(TEXT("Choice_%i"), Index);
		UDialogueChoice* NewChoice = CreateDefaultSubobject<UDialogueChoice>(FName(*ChoiceName));
		if (Index % 2 == 0)
		{
			NewChoice->Text = FText::FromString(TEXT("Yes"));
			NewChoice->Slot = CenterRight;
		}
		else
		{
			NewChoice->Text = FText::FromString(TEXT("No"));
			NewChoice->Slot = CenterLeft;
		}
		Choices.Add(NewChoice);

		Branches[NewIndex].BranchParam = NewChoice;
	}*/

}

void UShowChoices::Execute(APlayerController* InPlayer, AMapEventActor* InMapEventActor)
{
	if (AHUD* HUD = InPlayer->GetHUD())
	{
		if (HUD->GetClass()->ImplementsInterface(UMessageInterface::StaticClass()))
		{
			Player = InPlayer;
			MapEvent = InMapEventActor;
			IMessageInterface::Execute_ShowChoices(HUD, this);
		}
	}
}

int32 UShowChoices::GetBranchSize() const
{
	return Branches.Num();
}

//FName UShowChoices::GetBranchLabel(uint32 InIndex) const
//{
//	if (InIndex < Branches.Num())
//	{
//		return Branches[InIndex].
//	}
//	int32 Index = 0;
//	for (const FDialogueChoice& Choice : Choices)
//	{
//		if (!Choice.Text.IsEmptyOrWhitespace())
//		{
//			if (InIndex == Index)
//			{
//				return *Choice.Text.ToString();
//			}
//			Index++;
//		}
//	}
//	return FName(NAME_None);
//}

void UShowChoices::SelectBranch(int32 SelectedChoice)
{
	if (SelectedChoice < Branches.Num())
	{
		if (UBaseCommand* Command = Branches[SelectedChoice].LinkedCommand)
		{
			Command->Execute(Player, MapEvent);
			if(Command->IsUINeeded())
			{
				return;
			}

			if (AHUD* HUD = Player->GetHUD())
			{
				if (HUD->GetClass()->ImplementsInterface(UMessageInterface::StaticClass()))
				{
					IMessageInterface::Execute_CloseMessages(HUD);
				}
			}

			if (AMainCharacter* MainCharacter = CastChecked<AMainCharacter>(Player->GetPawn()))
			{
				MainCharacter->EnableMovement(true);
				MainCharacter->EnableCamera(true);
			}

		}
	}
}

#if WITH_EDITOR

#include "MapEvent.h"

void UShowChoices::SetBranchCommand(UBaseCommand* Command, int32 InIndex)
{
	if (InIndex < Branches.Num())
	{
		Branches[InIndex].LinkedCommand = Command;
	}
}

bool UShowChoices::GetBranchByPin(const UEdGraphPin* InPin, struct FBranch& OutBranch) const
{
	for (int32 Index = 0; Index < Branches.Num(); Index++)
	{
		if (Branches[Index].Pin != nullptr && Branches[Index].Pin == InPin)
		{
			OutBranch = Branches[Index];
			return true;
		}
	}
	return false;
}

UBaseCommand* UShowChoices::GetBranchCommand(int32 Index) const
{
	if (Index < Branches.Num())
	{
		return Branches[Index].LinkedCommand;
	}
	return nullptr;
}


void UShowChoices::SetBranch(int32 InIndex, UEdGraphPin* Pin)
{
	if (InIndex < Branches.Num())
	{
		Branches[InIndex].Pin = Pin;
	}
}

void UShowChoices::PostEditChangeProperty(FPropertyChangedEvent& e)
{
	FName PropertyName = (e.Property != nullptr) ? e.Property->GetFName() : NAME_None;
	if (PropertyName == FName("Choices"))
	{
		if (e.ChangeType == EPropertyChangeType::ArrayAdd)
		{
			FillChoices();
		}

		SynchronizeBranches();

		OnChoicesChanged.ExecuteIfBound();
		return;
	}

	if (e.MemberProperty != nullptr)
	{
		if (e.MemberProperty->GetFName() == FName("Choices"))
		{
			OnChoicesChanged.ExecuteIfBound();
			return;
		}
	}
}

void UShowChoices::FillChoices()
{
	for (uint8 Index = 0; Index < Choices.Num() ; Index++)
	{
		if (Choices[Index] == nullptr)
		{
			FString ChoiceName = FString::Printf(TEXT("Choice_%i"), Index);
			UDialogueChoice* NewChoice = NewObject<UDialogueChoice>(this, FName(*ChoiceName));
			if (Index == 0)
			{
				NewChoice->Text = FText::FromString(TEXT("Yes"));
				NewChoice->Slot = CenterRight;
			}
			else if(Index == 1)
			{
				NewChoice->Text = FText::FromString(TEXT("No"));
				NewChoice->Slot = CenterLeft;
			}
			else
			{
				NewChoice->Text = FText::FromString(ChoiceName);
				NewChoice->Slot = Bottom;
			}
			Choices[Index] = NewChoice;

			if (Index >= Branches.Num())
			{
				Branches.EmplaceAt(Index);
			}
			Branches[Index].BranchParam = NewChoice;
		}
	}
}

void UShowChoices::SynchronizeBranches()
{
	for (int32 Index = 0; Index < Choices.Num(); Index++)
	{
		if (Index < Branches.Num())
		{
			if (Branches[Index].BranchParam != nullptr)
			{
				FBranch CorrectBranch;
				int32 BranchIndex = GetBranchByParam(Choices[Index], CorrectBranch);
				if (BranchIndex >= 0)
				{
					Branches.Swap(Index, BranchIndex);
					continue;
				}
				else
				{
					Branches.EmplaceAt(Index);
				}
			}
			Branches[Index].BranchParam = Choices[Index];
		}
	}

	for (int32 BranchIndex = Branches.Num() - 1; BranchIndex >= Choices.Num(); BranchIndex--)
	{
		if (Branches[BranchIndex].LinkedCommand == nullptr && Branches[BranchIndex].BranchParam == nullptr)
		{
			Branches.RemoveAt(BranchIndex);
		}
	}

	if (Branches.Num() > Choices.Num())
	{
		for (int32 ChoiceIndex = Choices.Num(); ChoiceIndex < Branches.Num(); ChoiceIndex++)
		{
			FString ChoiceName = FString::Printf(TEXT("Choice_%i"), ChoiceIndex);
			UDialogueChoice* NewChoice = NewObject<UDialogueChoice>(this, FName(*ChoiceName));
			NewChoice->Slot = EDialogueWheelSlot::Bottom;
			Choices.Add(NewChoice);

			Branches[ChoiceIndex].BranchParam = NewChoice;
		}
	}
}

int32 UShowChoices::GetBranchByParam(const UBranchParameter* BranchParameter, FBranch& Branch) const
{
	for (int32 BranchIndex = 0; BranchIndex < Branches.Num(); BranchIndex++)
	{
		if (Branches[BranchIndex].BranchParam == BranchParameter)
		{
			Branch = Branches[BranchIndex];
			return BranchIndex;
		}
	}
	return -1;
}

#endif