#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WavesTypes.h"
#include "DA_DifficultyProfile.generated.h"

/**
 * Curves that drive spawn interval, speed, and color complexity over time.
 */
UCLASS(BlueprintType)
class WAVES_API UDA_DifficultyProfile : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	/** Time-based difficulty curves. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves")
	FDifficultyCurves Curves;

	/** Safe query helpers (return default if curve missing). */

	UFUNCTION(BlueprintCallable, Category = "Waves")
	float GetSpawnIntervalAt(float ElapsedSeconds, float DefaultValue = 1.0f) const
	{
		return Curves.SpawnInterval ? Curves.SpawnInterval->GetFloatValue(ElapsedSeconds) : DefaultValue;
	}

	UFUNCTION(BlueprintCallable, Category = "Waves")
	float GetWaveSpeedMulAt(float ElapsedSeconds, float DefaultValue = 1.0f) const
	{
		return Curves.WaveSpeed ? Curves.WaveSpeed->GetFloatValue(ElapsedSeconds) : DefaultValue;
	}

	/** Complexity is rounded to nearest integer (min 1). */
	UFUNCTION(BlueprintCallable, Category = "Waves")
	int32 GetColorComplexityAt(float ElapsedSeconds, int32 DefaultValue = 1) const
	{
		const float Raw = Curves.ColorComplexity ? Curves.ColorComplexity->GetFloatValue(ElapsedSeconds) : (float)DefaultValue;
		return FMath::Max(1, FMath::RoundToInt(Raw));
	}
};
