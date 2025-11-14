#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WavesTypes.h"
#include "ShieldComponent.generated.h"

class UDA_WaveColorSet;
class UDA_GameRules;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFrequencyChanged, EWaveFrequency, NewFreq);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbsorb);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFail);

UCLASS(ClassGroup = (Waves), meta = (BlueprintSpawnableComponent))
class WAVES_API UShieldComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UShieldComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves|Data")
	TObjectPtr<UDA_WaveColorSet> ColorSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves|Data")
	TObjectPtr<UDA_GameRules> GameRules;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves")
	EWaveFrequency CurrentFrequency = EWaveFrequency::Blue;

	UPROPERTY(BlueprintAssignable, Category = "Waves") FOnFrequencyChanged OnFrequencyChanged;
	UPROPERTY(BlueprintAssignable, Category = "Waves") FOnAbsorb OnAbsorb;
	UPROPERTY(BlueprintAssignable, Category = "Waves") FOnFail OnFail;

	UFUNCTION(BlueprintCallable, Category = "Waves") void SetFrequency(EWaveFrequency NewFreq);
	UFUNCTION(BlueprintCallable, Category = "Waves") void CycleNext();
	UFUNCTION(BlueprintCallable, Category = "Waves") void CyclePrev();

	/** Returns true if absorbed (match). */
	UFUNCTION(BlueprintCallable, Category = "Waves")
	bool TryAbsorb(EWaveFrequency Incoming);

	// Hold/Overload basic flags for now
	UFUNCTION(BlueprintCallable, Category = "Waves") void BeginHold() { bHolding = true; }
	UFUNCTION(BlueprintCallable, Category = "Waves") void EndHold() { bHolding = false; }

	/** Returns true if enough energy and triggers cost (logic kept minimal). */
	UFUNCTION(BlueprintCallable, Category = "Waves")
	bool TriggerOverload();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waves")
	bool bHolding = false;
};
