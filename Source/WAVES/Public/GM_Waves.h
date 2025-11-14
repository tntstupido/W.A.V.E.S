#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_Waves.generated.h"

class UDA_WaveColorSet;
class UDA_DifficultyProfile;
class UDA_GameRules;

UCLASS()
class WAVES_API AGM_Waves : public AGameModeBase
{
	GENERATED_BODY()
public:
	AGM_Waves();

	/** Data assets injected via editor. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Waves|Data")
	TObjectPtr<UDA_WaveColorSet> ColorSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Waves|Data")
	TObjectPtr<UDA_DifficultyProfile> Difficulty;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Waves|Data")
	TObjectPtr<UDA_GameRules> GameRules;

	/** Quick getters for other classes. */
	UFUNCTION(BlueprintPure, Category = "Waves|Data") UDA_WaveColorSet* GetColorSet() const { return ColorSet; }
	UFUNCTION(BlueprintPure, Category = "Waves|Data") UDA_DifficultyProfile* GetDifficulty() const { return Difficulty; }
	UFUNCTION(BlueprintPure, Category = "Waves|Data") UDA_GameRules* GetGameRules() const { return GameRules; }

protected:
	virtual void BeginPlay() override;
};
