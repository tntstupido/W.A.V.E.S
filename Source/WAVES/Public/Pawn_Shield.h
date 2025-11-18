// Source/Waves/Public/Pawn_Shield.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Pawn_Shield.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UShieldComponent;

class USpringArmComponent;
class UCameraComponent;
class USceneComponent;

UCLASS()
class WAVES_API APawn_Shield : public APawn
{
	GENERATED_BODY()

public:
	APawn_Shield();

	/** Collision root */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shield")
	TObjectPtr<UBoxComponent> Collision;

	/** Shield logic component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shield")
	TObjectPtr<UShieldComponent> Shield;

	/** Visual mesh for the planet */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shield|Visual")
	TObjectPtr<UStaticMeshComponent> PlanetMesh;

	/** Root for shield orbit around the planet */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shield|Visual")
	TObjectPtr<USceneComponent> OrbitRoot;

	/** Visual mesh for the shield ring/segment */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shield|Visual")
	TObjectPtr<UStaticMeshComponent> ShieldMesh;

	/** Camera boom positioning the camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	/** Level camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;

	// --- Orbit configuration ---

	/** Distance of shield from planet center (in units) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shield|Orbit")
	float OrbitRadius = 200.f;

	/** Degrees per shield segment (4 segments -> 90 degrees) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shield|Orbit")
	float OrbitStepDegrees = 90.f;

	/** Current segment index [0..3] */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shield|Orbit")
	int32 CurrentSegmentIndex = 0;

	/** Called whenever we change segment/frequency to reposition shield around planet */
	UFUNCTION(BlueprintCallable, Category = "Shield|Orbit")
	void UpdateShieldOrbit();

	// Input API (called from PlayerController)
	UFUNCTION(BlueprintCallable) void NextFrequency();
	UFUNCTION(BlueprintCallable) void PrevFrequency();
	UFUNCTION(BlueprintCallable) void BeginHold();
	UFUNCTION(BlueprintCallable) void EndHold();
	UFUNCTION(BlueprintCallable) void TriggerOverload();

protected:
	virtual void BeginPlay() override;
};
