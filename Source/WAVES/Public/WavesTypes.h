#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Curves/CurveFloat.h"
#include "Sound/SoundBase.h"
#include "WavesTypes.generated.h"

/** Frequencies / colors player can match against. */
UENUM(BlueprintType)
enum class EWaveFrequency : uint8
{
	Blue    UMETA(DisplayName = "Blue"),
	Red     UMETA(DisplayName = "Red"),
	Yellow  UMETA(DisplayName = "Yellow"),
	Green   UMETA(DisplayName = "Green"),
	// add more later if needed
};

/** Spawn pattern hint for the spawner. */
UENUM(BlueprintType)
enum class EWavePattern : uint8
{
	Straight    UMETA(DisplayName = "Straight"),
	Staggered   UMETA(DisplayName = "Staggered"),
	Alternating UMETA(DisplayName = "Alternating"),
	MultiWave   UMETA(DisplayName = "MultiWave")
};

/** Visual theme for quick palette swaps. */
UENUM(BlueprintType)
enum class EVisualTheme : uint8
{
	LoFi    UMETA(DisplayName = "Lo-Fi"),
	Pulse   UMETA(DisplayName = "Pulse")
};

/** Per-frequency definition used by WaveColorSet. */
USTRUCT(BlueprintType)
struct FWaveColorDef
{
	GENERATED_BODY()

	/** Human friendly name (e.g., "Electric Blue"). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName DisplayName = "Blue";

	/** Enum key for frequency matching. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EWaveFrequency Frequency = EWaveFrequency::Blue;

	/** Linear color that drives materials. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor Color = FLinearColor(0.2f, 0.6f, 1.f, 1.f);

	/** Max hue/param delta allowed to still count as a match (gameplay tolerance). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float MatchTolerance = 0.05f;

	/** Optional SFX */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USoundBase> SwapCue = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USoundBase> AbsorbCue = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USoundBase> FailCue = nullptr;
};

/** Runtime spec for a single wave instance. */
USTRUCT(BlueprintType)
struct FWaveSpec
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	EWaveFrequency Frequency = EWaveFrequency::Blue;

	/** Units/sec; movement handled in Tick or in BP. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave", meta = (ClampMin = "0"))
	float Speed = 800.f;

	/** For shader/FX intensity or lane height if you use it. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave", meta = (ClampMin = "0"))
	float Amplitude = 1.0f;

	/** If > 0, auto-destroy after this many seconds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave", meta = (ClampMin = "0"))
	float LifeSeconds = 6.0f;
};

/** Combo → resonance behavior tunables. */
USTRUCT(BlueprintType)
struct FResonanceParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resonance", meta = (ClampMin = "1"))
	int32 ComboThreshold = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resonance", meta = (ClampMin = "0"))
	float DurationSeconds = 10.f;

	/** Extra score % during resonance. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resonance")
	float ScoreBonusPct = 10.f;

	/** If true, any color matches during resonance (arcade-friendly). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resonance")
	bool bAutoMatchDuringResonance = true;
};

/** Global rules that affect energy economy and abilities. */
USTRUCT(BlueprintType)
struct FGameRules
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Energy", meta = (ClampMin = "0", ClampMax = "100"))
	int32 StartEnergy = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Energy")
	int32 EnergyGainOnAbsorb = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Energy")
	int32 EnergyLossOnFail = 5;

	/** Boss fail can cost more; if 0, uses EnergyLossOnFail. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Energy")
	int32 EnergyLossOnBossFail = 10;

	/** Overload pulse cost; drain instantly when triggered. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Overload")
	int32 OverloadEnergyCost = 15;

	/** Optional continuous drain while holding. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hold")
	float HoldEnergyDrainPerSecond = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resonance")
	FResonanceParams Resonance;
};

/** Set of difficulty curves the spawner queries over time. */
USTRUCT(BlueprintType)
struct FDifficultyCurves
{
	GENERATED_BODY()

	/** Seconds between spawns vs elapsed game time. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCurveFloat> SpawnInterval = nullptr;

	/** Wave speed multiplier vs elapsed time. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCurveFloat> WaveSpeed = nullptr;

	/** Complexity step (1=single, 2=dual, …) vs elapsed time. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCurveFloat> ColorComplexity = nullptr;
};
