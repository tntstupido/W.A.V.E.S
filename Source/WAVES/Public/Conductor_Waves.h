// Source/Waves/Public/Conductor_Waves.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Conductor_Waves.generated.h"

class USoundBase;
class UDA_WavesMusicTrack;    // NEW

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeat, int32, BeatIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBar, int32, BarIndex, int32, BeatInBar);

UCLASS()
class WAVES_API AConductor_Waves : public AActor
{
	GENERATED_BODY()

public:
	AConductor_Waves();

	/** Optional music track config: defines BPM, BeatsPerBar, Bars, MusicTrack */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Music")
	TObjectPtr<UDA_WavesMusicTrack> TrackConfig;

	/** The music track to play at BeginPlay (can be overridden by TrackConfig) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Music")
	TObjectPtr<USoundBase> MusicTrack;

	/** BPM of the track (if TrackConfig is set, this is taken from there) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Music", meta = (ClampMin = "40.0", ClampMax = "200.0"))
	float BPM = 120.f;

	/** Beats per bar (if TrackConfig is set, this is taken from there) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Music", meta = (ClampMin = "1", ClampMax = "16"))
	int32 BeatsPerBar = 4;

	/** Fired every beat: 0,1,2,... */
	UPROPERTY(BlueprintAssignable, Category = "Music")
	FOnBeat OnBeat;

	/** Fired every beat with bar info */
	UPROPERTY(BlueprintAssignable, Category = "Music")
	FOnBar OnBar;

	/** Intensity (0-1) for a given beat index, based on TrackConfig bars */
	UFUNCTION(BlueprintCallable, Category = "Music")
	float GetIntensityForBeat(int32 BeatIndex) const;

	/** Beat index within bar for a given global beat index */
	UFUNCTION(BlueprintCallable, Category = "Music")
	int32 GetBeatInBar(int32 BeatIndex) const;

protected:
	virtual void BeginPlay() override;

private:
	FTimerHandle TimerHandle_Beat;

	int32 BeatCounter = 0;
	int32 BarCounter = 0;

	void HandleBeat();
};
