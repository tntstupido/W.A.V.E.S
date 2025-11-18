// Source/Waves/Public/WavesMusicTrack.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_WavesMusicTrack.generated.h"

class USoundBase;

USTRUCT(BlueprintType)
struct FWavesMusicBarInfo
{
	GENERATED_BODY()

	/** Intensity of this bar: 0 = calm, 1 = maximum chaos */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Music")
	float Intensity = 1.f;
};

/**
 * Describes one music track: audio + BPM + per-bar intensities.
 */
UCLASS(BlueprintType)
class WAVES_API UDA_WavesMusicTrack : public UDataAsset
{
	GENERATED_BODY()

public:
	/** Optional ID/name for this track */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Music")
	FName TrackId;

	/** Audio asset for this track */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Music")
	TObjectPtr<USoundBase> MusicTrack;

	/** BPM of the track (e.g. 100) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Music", meta = (ClampMin = "40.0", ClampMax = "200.0"))
	float BPM = 120.f;

	/** Beats per bar (e.g. 4 for 4/4) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Music", meta = (ClampMin = "1", ClampMax = "16"))
	int32 BeatsPerBar = 4;

	/** Bar-based intensity curve over the song */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Music")
	TArray<FWavesMusicBarInfo> Bars;
};
