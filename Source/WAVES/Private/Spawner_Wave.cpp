#include "Spawner_Wave.h"
#include "Actor_Wave.h"
#include "GS_Waves.h"
#include "DA_DifficultyProfile.h"
#include "Engine/World.h"
#include "Conductor_Waves.h"
#include "Kismet/GameplayStatics.h"


ASpawner_Wave::ASpawner_Wave()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASpawner_Wave::BeginPlay()
{
	Super::BeginPlay();

	RNG.Initialize(RandomSeed);
	
	if (bUseMusicConductor)
	{
		AConductor_Waves* Conductor = Cast<AConductor_Waves>(
			UGameplayStatics::GetActorOfClass(this, AConductor_Waves::StaticClass())
		);

		if (Conductor)
		{
			CachedConductor = Conductor;

			UE_LOG(LogTemp, Log, TEXT("Spawner %s bound to Conductor %s"),
				*GetName(), *Conductor->GetName());

			Conductor->OnBeat.AddDynamic(this, &ASpawner_Wave::HandleBeat);
			return;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Spawner %s using internal timer (no Conductor or disabled)."),
		*GetName());

	StartSpawning();
}

void ASpawner_Wave::HandleBeat(int32 BeatIndex)
{
	// 1) Get intensity & beat-in-bar from Conductor
	float Intensity = 1.f;
	int32 BeatInBar = 0;

	if (CachedConductor.IsValid())
	{
		Intensity = CachedConductor->GetIntensityForBeat(BeatIndex);
		BeatInBar = CachedConductor->GetBeatInBar(BeatIndex);
	}

	Intensity = FMath::Clamp(Intensity, 0.f, 1.f);

	// 2) Convert intensity to spawn probability
	const float SpawnChance = FMath::Lerp(MinSpawnChance, MaxSpawnChance, Intensity);

	bool bShouldSpawn = false;

	// 3) Optionally force spawn on downbeat (e.g. kick hits)
	if (bForceSpawnOnDownbeat && BeatInBar == 0 && Intensity > 0.f)
	{
		bShouldSpawn = true;
	}
	else
	{
		const float Roll = FMath::FRand();
		bShouldSpawn = (Roll < SpawnChance);
	}

	// 4) Optional pattern mask (can give some structure)
	if (bShouldSpawn && BeatSpawnPattern.Num() > 0)
	{
		const int32 PatternLength = BeatSpawnPattern.Num();
		if (PatternLength > 0)
		{
			int32 EffectiveBeat = BeatIndex + BeatOffset;

			if (EffectiveBeat < 0)
			{
				int32 NumLoops = (FMath::Abs(EffectiveBeat) / PatternLength) + 1;
				EffectiveBeat += NumLoops * PatternLength;
			}

			const int32 PatternIndex = EffectiveBeat % PatternLength;

			if (!BeatSpawnPattern[PatternIndex])
			{
				bShouldSpawn = false;
			}
		}
	}

	if (bShouldSpawn)
	{
		SpawnOnce();
	}
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
		return;
	}

	UWorld* World = GetWorld();
	if (!World) return;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor_Wave* Wave = World->SpawnActor<AActor_Wave>(WaveClass, GetActorTransform(), Params);
	if (Wave)
	{
		// Set frequency, difficulty, etc.
		const int32 FreqIdx = RNG.RandRange(0, 3);
		const EWaveFrequency NewFreq = static_cast<EWaveFrequency>(FreqIdx);
		Wave->SetFrequency(NewFreq);

		// --- NEW: auto adjust speed for beat-sync travel ---
		if (bAutoAdjustWaveSpeed && ImpactTarget && CachedConductor.IsValid())
		{
			const float BPM = CachedConductor->BPM;
			const float BeatSeconds = (BPM > 0.f) ? 60.f / BPM : 0.6f;
			const float TravelTime = TravelBeats * BeatSeconds;

			const FVector SpawnLocation = GetActorLocation();
			const FVector TargetLocation = ImpactTarget->GetActorLocation();

			// Project distance along the wave movement direction
			const FVector Dir = Wave->MoveDir.GetSafeNormal();
			const float DistanceAlongDir = FVector::DotProduct(TargetLocation - SpawnLocation, Dir);
			const float Distance = FMath::Abs(DistanceAlongDir);

			if (TravelTime > 0.f && Distance > 0.f)
			{
				const float NewSpeed = Distance / TravelTime;
				Wave->Spec.Speed = NewSpeed;

				UE_LOG(LogTemp, Log, TEXT("Spawner %s set Wave %s Speed=%.2f for Distance=%.1f, TravelBeats=%.1f"),
					*GetName(), *Wave->GetName(), NewSpeed, Distance, TravelBeats);
			}
		}
	}
}


