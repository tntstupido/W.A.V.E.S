#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PC_Waves.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class WAVES_API APC_Waves : public APlayerController
{
	GENERATED_BODY()
public:
	APC_Waves();

	/** Assign in BP_PC_Waves. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> InputContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_FreqUp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_FreqDown;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Hold;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Overload;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// Handlers
	void OnFreqUp();
	void OnFreqDown();
	void OnHoldPressed();
	void OnHoldReleased();
	void OnOverload();
};
