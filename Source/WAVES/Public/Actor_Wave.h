#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WavesTypes.h"
#include "Actor_Wave.generated.h"

class UStaticMeshComponent;

UCLASS()
class WAVES_API AActor_Wave : public AActor
{
	GENERATED_BODY()
public:
	AActor_Wave();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waves")
	TObjectPtr<UStaticMeshComponent> Mesh;

	/** Spec controls speed, amplitude, life. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
	FWaveSpec Spec;

	/** Direction of travel (normalized). Defaults to -X. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
	FVector MoveDir = FVector(-1, 0, 0);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void OnWaveBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	float LifeRemaining = -1.f;
};
