#include "ScoringSubsystem.h"

void UScoringSubsystem::RegisterAbsorb(int32 BaseScore, float BonusPct)
{
	ComboCount++;
	const float Bonus = ComboCount >= 5 ? BonusPct : 0.f;
	Score += BaseScore + FMath::RoundToInt(BaseScore * (Bonus / 100.f));
}

void UScoringSubsystem::RegisterFail()
{
	ComboCount = 0;
}

void UScoringSubsystem::EnterResonance(float CurrentTime, float Duration)
{
	bInResonance = true;
	ResonanceEndTime = CurrentTime + Duration;
}
