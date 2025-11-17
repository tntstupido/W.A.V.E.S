// Source/Waves/Public/Pawn_Shield.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Pawn_Shield.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;
class UShieldComponent;

UCLASS()
class WAVES_API APawn_Shield : public APawn
{
	GENERATED_BODY()

public:
	APawn_Shield();

	/** Collision root */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shield")
	TObjectPtr<UCapsuleComponent> Collision;

	/** Visual mesh (no collision) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shield")
	TObjectPtr<UStaticMeshComponent> VisualMesh;

	/** Shield logic component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shield")
	TObjectPtr<UShieldComponent> Shield;

	// Input API (called from PlayerController)
	UFUNCTION(BlueprintCallable) void NextFrequency();
	UFUNCTION(BlueprintCallable) void PrevFrequency();
	UFUNCTION(BlueprintCallable) void BeginHold();
	UFUNCTION(BlueprintCallable) void EndHold();
	UFUNCTION(BlueprintCallable) void TriggerOverload();

protected:
	virtual void BeginPlay() override;
};
