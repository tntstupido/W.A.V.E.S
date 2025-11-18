// Source/Waves/Private/Conductor_Waves.cpp

#include "Conductor_Waves.h"
#include "Kismet/GameplayStatics.h"
#include "DA_WavesMusicTrack.h"

AConductor_Waves::AConductor_Waves()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AConductor_Waves::BeginPlay()
{
	Super::BeginPlay();

	// If a TrackConfig is provided, pull BPM/BeatsPerBar/Music from it
	if (TrackConfig)
	{
		if (TrackConfig->MusicTrack)
		{
			MusicTrack = TrackConfig->MusicTrack;
		}

		BPM = TrackConfig->BPM;
		BeatsPerBar = TrackConfig->BeatsPerBar;
	}

	// Play music
	if (MusicTrack)
	{
		UGameplayStatics::PlaySound2D(this, MusicTrack);
	}

	// Set up beat timer
	const float BeatLength = (BPM > 0.f) ? 60.f / BPM : 0.5f;

	BeatCounter = 0;
	BarCounter = 0;

	if (BeatLength > 0.f)
	{
		GetWorldTimerManager().SetTimer(
			TimerHandle_Beat,
			this,
			&AConductor_Waves::HandleBeat,
			BeatLength,
			true
		);
	}
}

void AConductor_Waves::HandleBeat()
{
	// Broadcast simple beat index (0,1,2,...)
	OnBeat.Broadcast(BeatCounter);

	// Compute beat within bar
	const int32 BeatInBar = (BeatsPerBar > 0) ? (BeatCounter % BeatsPerBar) : BeatCounter;

	// Advance bar counter when a new bar starts
	if (BeatInBar == 0 && BeatCounter > 0)
	{
		++BarCounter;
	}

	OnBar.Broadcast(BarCounter, BeatInBar);

	++BeatCounter;
}

float AConductor_Waves::GetIntensityForBeat(int32 BeatIndex) const
{
	if (!TrackConfig || TrackConfig->Bars.Num() == 0 || BeatsPerBar <= 0)
	{
		return 1.f; // default intensity
	}

	const int32 BarIndex = FMath::Clamp(BeatIndex / BeatsPerBar, 0, TrackConfig->Bars.Num() - 1);
	return TrackConfig->Bars[BarIndex].Intensity;
}

int32 AConductor_Waves::GetBeatInBar(int32 BeatIndex) const
{
	if (BeatsPerBar <= 0)
	{
		return BeatIndex;
	}

	return BeatIndex % BeatsPerBar;
}
