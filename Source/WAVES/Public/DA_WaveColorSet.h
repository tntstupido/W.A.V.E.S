#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WavesTypes.h"
#include "DA_WaveColorSet.generated.h"


/**
 * Data asset listing all playable frequencies with colors & SFX.
 */
UCLASS(BlueprintType)
class WAVES_API UDA_WaveColorSet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	/** Order matters if you cycle with Next/Prev. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves")
	TArray<FWaveColorDef> Colors;

	/** Utility: get definition by enum. Returns empty struct if not found. */
	UFUNCTION(BlueprintCallable, Category = "Waves")
	FWaveColorDef FindByFrequency(EWaveFrequency Freq) const;

	/** Returns index in Colors array or INDEX_NONE. */
	UFUNCTION(BlueprintCallable, Category = "Waves")
	int32 GetIndexByFrequency(EWaveFrequency Freq) const;

	/** Circular next/prev helpers for UI/controller cycling. */
	UFUNCTION(BlueprintCallable, Category = "Waves")
	EWaveFrequency GetNext(EWaveFrequency Current) const;

	UFUNCTION(BlueprintCallable, Category = "Waves")
	EWaveFrequency GetPrev(EWaveFrequency Current) const;
};
