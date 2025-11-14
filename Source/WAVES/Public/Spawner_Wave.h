#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WavesTypes.h"
#include "Spawner_Wave.generated.h"

class AActor_Wave;
class UDA_DifficultyProfile;

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

protected:
	virtual void BeginPlay() override;

	void StartSpawning();
	void SpawnOnce();
	float ComputeInterval() const;

	FTimerHandle TimerHandle_Spawn;

private:
	FRandomStream RNG;
};
