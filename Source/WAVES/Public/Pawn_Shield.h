#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "WavesTypes.h"
#include "Pawn_Shield.generated.h"

class UStaticMeshComponent;
class UShieldComponent;
class UDA_WaveColorSet;

UCLASS()
class WAVES_API APawn_Shield : public APawn
{
	GENERATED_BODY()
public:
	APawn_Shield();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waves")
	TObjectPtr<UStaticMeshComponent> VisualMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waves")
	TObjectPtr<UShieldComponent> Shield;

	/** Optional override if you want to inject ColorSet here. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves|Data")
	TObjectPtr<UDA_WaveColorSet> ColorSetOverride;

	// Input-driven API (called by controller)
	UFUNCTION(BlueprintCallable) void NextFrequency();
	UFUNCTION(BlueprintCallable) void PrevFrequency();
	UFUNCTION(BlueprintCallable) void BeginHold();
	UFUNCTION(BlueprintCallable) void EndHold();
	UFUNCTION(BlueprintCallable) void TriggerOverload();

protected:
	virtual void BeginPlay() override;
};
