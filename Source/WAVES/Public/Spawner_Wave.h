#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WavesTypes.h"
#include "Spawner_Wave.generated.h"

class AActor_Wave;
class UDA_DifficultyProfile;
class AConductor_Waves;

UCLASS()
class WAVES_API ASpawner_Wave : public AActor
{
	GENERATED_BODY()
public:
	ASpawner_Wave();

	/** Which wave actor to spawn. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves")
	TSubclassOf<AActor_Wave> WaveClass;

	/** Default interval if no difficulty profile. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves", meta = (ClampMin = "0.05"))
	float SpawnInterval = 1.0f;

	/** Optional difficulty curves to drive interval/speed/complexity. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves|Data")
	TObjectPtr<UDA_DifficultyProfile> Difficulty;

	/** Random stream for reproducible patterns if needed. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves")
	int32 RandomSeed = 1337;

	// Whether this spawner should try to sync with the Conductor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves|Spawner")
	bool bUseMusicConductor = true;

	// -----

	/** Base spawn chance when intensity = 0 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves|Pattern")
	float MinSpawnChance = 0.1f;

	/** Spawn chance when intensity = 1 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves|Pattern")
	float MaxSpawnChance = 0.9f;

	/** If true, always spawn on bar downbeat (BeatInBar == 0) when intensity > 0 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves|Pattern")
	bool bForceSpawnOnDownbeat = false;

	/** Beat-based spawn pattern.
	 * If empty, spawns every beat.
	 * If not empty, uses (BeatIndex + BeatOffset) % PatternLength to decide spawn.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves|Pattern")
	TArray<bool> BeatSpawnPattern;

	/** Shifts pattern in time: positive -> later, negative -> earlier (in beats). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves|Pattern")
	int32 BeatOffset = 0;

	// -----

	/** Target actor the waves are traveling toward (e.g. the shield or planet center) */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Waves|Sync")
	TObjectPtr<AActor> ImpactTarget;

	/** How many beats waves should take from spawn to impact (e.g. 4 beats = 1 bar) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves|Sync", meta = (ClampMin = "1.0", ClampMax = "32.0"))
	float TravelBeats = 4.f;

	/** Auto-adjust wave speed based on BPM + distance */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves|Sync")
	bool bAutoAdjustWaveSpeed = true;



protected:
	virtual void BeginPlay() override;

	void StartSpawning();
	void SpawnOnce();
	float ComputeInterval() const;

	FTimerHandle TimerHandle_Spawn;

	UFUNCTION()
	void HandleBeat(int32 BeatIndex);

private:
	FRandomStream RNG;

	// weak pointer to the Conductor
	TWeakObjectPtr<AConductor_Waves> CachedConductor;
};
