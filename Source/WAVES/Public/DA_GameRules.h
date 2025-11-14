#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WavesTypes.h"
#include "DA_GameRules.generated.h"

/**
 * Global gameplay economy and resonance settings.
 */
UCLASS(BlueprintType)
class WAVES_API UDA_GameRules : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves")
	FGameRules Rules;

	/** Convenience getters for Blueprints (clean pins). */
	UFUNCTION(BlueprintPure, Category = "Waves") int32 GetStartEnergy() const { return Rules.StartEnergy; }
	UFUNCTION(BlueprintPure, Category = "Waves") int32 GetEnergyGainOnAbsorb() const { return Rules.EnergyGainOnAbsorb; }
	UFUNCTION(BlueprintPure, Category = "Waves") int32 GetEnergyLossOnFail() const { return Rules.EnergyLossOnFail; }
	UFUNCTION(BlueprintPure, Category = "Waves") int32 GetEnergyLossOnBossFail() const { return Rules.EnergyLossOnBossFail; }
	UFUNCTION(BlueprintPure, Category = "Waves") int32 GetOverloadEnergyCost() const { return Rules.OverloadEnergyCost; }
	UFUNCTION(BlueprintPure, Category = "Waves") float GetHoldDrainPerSecond() const { return Rules.HoldEnergyDrainPerSecond; }
	UFUNCTION(BlueprintPure, Category = "Waves") FResonanceParams GetResonanceParams() const { return Rules.Resonance; }
};
