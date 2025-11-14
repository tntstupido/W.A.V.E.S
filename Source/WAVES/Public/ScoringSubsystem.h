#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WavesTypes.h"
#include "ScoringSubsystem.generated.h"

/** Lightweight scoring/combo holder for now. */
UCLASS()
class WAVES_API UScoringSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waves")
	int32 Score = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waves")
	int32 ComboCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waves")
	bool bInResonance = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waves")
	float ResonanceEndTime = 0.f;

	UFUNCTION(BlueprintCallable, Category = "Waves")
	void RegisterAbsorb(int32 BaseScore = 100, float BonusPct = 10.f);

	UFUNCTION(BlueprintCallable, Category = "Waves")
	void RegisterFail();

	UFUNCTION(BlueprintPure, Category = "Waves")
	bool IsInResonance(float CurrentTime) const { return bInResonance && CurrentTime < ResonanceEndTime; }

	UFUNCTION(BlueprintCallable, Category = "Waves")
	void EnterResonance(float CurrentTime, float Duration);

	UFUNCTION(BlueprintCallable, Category = "Waves")
	void ExitResonance() { bInResonance = false; ResonanceEndTime = 0.f; }
};
