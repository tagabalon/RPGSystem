#include "CoreMinimal.h"

#include "RPGConvoAsset.generated.h"

class UTexture2D;
class USoundBase;

USTRUCT(BlueprintType)
struct FRPGConvoLine
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName CharacterId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText Speaker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue", meta = (MultiLine = true))
	FText Message;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TObjectPtr<UTexture2D> BustSprite = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TObjectPtr<USoundBase> VoiceAudio = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	int32 Column = 0;

	bool UsesActorSpeaker() const
	{
		return !CharacterId.IsNone();
	}
};

USTRUCT(BlueprintType)
struct FRPGConvo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName ConvoId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TArray<FRPGConvoLine> Lines;

	int32 AddLine()
	{
		return Lines.Add(FRPGConvoLine());
	}

	const FRPGConvoLine* GetLine(int32 Index) const
	{
		return Lines.IsValidIndex(Index) ? &Lines[Index] : nullptr;
	}

	int32 GetLineCount() const
	{
		return Lines.Num();
	}
};

UCLASS(BlueprintType)
class RPGSYSTEM_API URPGConvoAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	FName ConvoSetId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	TArray<FRPGConvo> Convos;

	UFUNCTION(BlueprintPure, Category = "Dialogue")
	bool GetConvo(FName ConvoId, FRPGConvo& OutConvo) const
	{
		if (const FRPGConvo* Convo = FindConvo(ConvoId))
		{
			OutConvo = *Convo;
			return true;
		}

		return false;
	}

	const FRPGConvo* FindConvo(FName ConvoId) const
	{
		for (const FRPGConvo& Convo : Convos)
		{
			if (Convo.ConvoId == ConvoId)
			{
				return &Convo;
			}
		}

		return nullptr;
	}

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	int32 AddConvo()
	{
		FRPGConvo NewConvo;
		NewConvo.ConvoId = MakeUniqueConvoId();

		return Convos.Add(NewConvo);
	}

	UFUNCTION(BlueprintPure)
	TArray<FName> GetConvoIds() const
	{
		TArray<FName> ConvoIds;
		for (const FRPGConvo& Convo : Convos)
		{
			ConvoIds.Add(Convo.ConvoId);
		}

		return ConvoIds;
	}

private:

	FName MakeUniqueConvoId() const
	{
		int32 Index = Convos.Num() + 1;

		while (true)
		{
			const FName Candidate =
				FName(*FString::Printf(TEXT("Convo_%03d"), Index));

			if (!FindConvo(Candidate))
			{
				return Candidate;
			}

			Index++;
		}
	}
};
