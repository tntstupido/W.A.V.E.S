#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GS_Waves.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnergyChanged, int32, NewEnergy);

UCLASS()
class WAVES_API AGS_Waves : public AGameStateBase
{
	GENERATED_BODY()
public:
	AGS_Waves();

	/** 0..100 by default, but not enforced here. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waves")
	int32 PlanetEnergy = 100;

	/** Broadcast whenever energy changes. */
	UPROPERTY(BlueprintAssignable, Category = "Waves")
	FOnEnergyChanged OnEnergyChanged;

	/** Elapsed time for difficulty curves. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waves")
	float ElapsedSeconds = 0.f;

	UFUNCTION(BlueprintCallable, Category = "Waves")
	void SetPlanetEnergy(int32 NewValue);

	UFUNCTION(BlueprintCallable, Category = "Waves")
	void AddEnergy(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Waves")
	void ResetElapsedTime() { ElapsedSeconds = 0.f; }

protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
};
