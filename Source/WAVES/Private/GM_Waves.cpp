#include "GM_Waves.h"
#include "GS_Waves.h"
#include "DA_GameRules.h"

AGM_Waves::AGM_Waves()
{
	GameStateClass = AGS_Waves::StaticClass();
}

void AGM_Waves::BeginPlay()
{
	Super::BeginPlay();

	// Initialize planet energy from rules (if available).
	if (AGS_Waves* GS = GetGameState<AGS_Waves>())
	{
		const int32 StartEnergy = (GameRules ? GameRules->GetStartEnergy() : 100);
		GS->SetPlanetEnergy(StartEnergy);
		GS->ResetElapsedTime();
	}
}
