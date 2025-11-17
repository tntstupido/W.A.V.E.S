#include "Spawner_Wave.h"
#include "Actor_Wave.h"
#include "GS_Waves.h"
#include "DA_DifficultyProfile.h"
#include "Engine/World.h"

ASpawner_Wave::ASpawner_Wave()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASpawner_Wave::BeginPlay()
{
	Super::BeginPlay();
	RNG.Initialize(RandomSeed);
	StartSpawning();
}

void ASpawner_Wave::StartSpawning()
{
	const float FirstDelay = ComputeInterval();
	GetWorldTimerManager().SetTimer(TimerHandle_Spawn, this, &ASpawner_Wave::SpawnOnce, FirstDelay, false);
}

float ASpawner_Wave::ComputeInterval() const
{
	if (const UWorld* World = GetWorld())
	{
		if (const AGS_Waves* GS = World->GetGameState<AGS_Waves>())
		{
			if (Difficulty)
			{
				return FMath::Max(0.05f, Difficulty->GetSpawnIntervalAt(GS->ElapsedSeconds, SpawnInterval));
			}
		}
	}
	return SpawnInterval;
}

void ASpawner_Wave::SpawnOnce()
{
	if (!WaveClass)
	{
		StartSpawning();
		return;
	}

	UWorld* World = GetWorld();
	if (!World) { return; }

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor_Wave* Wave = World->SpawnActor<AActor_Wave>(WaveClass, GetActorTransform(), Params);
	if (Wave)
	{
		// Random frequency 0..3
		const int32 FreqIdx = RNG.RandRange(0, 3);
		const EWaveFrequency NewFreq = static_cast<EWaveFrequency>(FreqIdx);

		Wave->SetFrequency(NewFreq);

		UE_LOG(LogTemp, Log, TEXT("Spawner %s created wave %s with Freq=%d"),
			*GetName(), *Wave->GetName(), FreqIdx);

		// Optional: speed multiplier from difficulty
		if (const AGS_Waves* GS = World->GetGameState<AGS_Waves>())
		{
			if (Difficulty)
			{
				const float Mul = Difficulty->GetWaveSpeedMulAt(GS->ElapsedSeconds, 1.0f);
				Wave->Spec.Speed *= Mul;
			}
		}
	}

	// schedule next
	const float NextInterval = ComputeInterval();
	GetWorldTimerManager().SetTimer(TimerHandle_Spawn, this, &ASpawner_Wave::SpawnOnce, NextInterval, false);
}

