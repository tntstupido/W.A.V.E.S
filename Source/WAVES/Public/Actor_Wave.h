// Source/Waves/Public/Actor_Wave.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WavesTypes.h"
#include "Actor_Wave.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class WAVES_API AActor_Wave : public AActor
{
	GENERATED_BODY()

public:
	AActor_Wave();

	/** Collision root */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	TObjectPtr<USphereComponent> Collision;

	/** Visual mesh (no collision) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	TObjectPtr<UStaticMeshComponent> Mesh;

	/** Spec controls speed, amplitude, life, frequency */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	FWaveSpec Spec;

	/** Direction of travel (normalized). Defaults to -X. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	FVector MoveDir = FVector(-1, 0, 0);

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void SetFrequency(EWaveFrequency NewFreq);

	UFUNCTION(BlueprintImplementableEvent, Category = "Wave")
	void OnFrequencyChanged_BP();



protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	/** Called when collision overlaps another actor */
	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	/** Internal lifetime counter */
	float LifeRemaining = -1.f;
};
