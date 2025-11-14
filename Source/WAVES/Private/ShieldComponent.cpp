#include "ShieldComponent.h"
#include "DA_WaveColorSet.h"
#include "DA_GameRules.h"

UShieldComponent::UShieldComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UShieldComponent::SetFrequency(EWaveFrequency NewFreq)
{
	if (CurrentFrequency == NewFreq) return;
	CurrentFrequency = NewFreq;
	OnFrequencyChanged.Broadcast(CurrentFrequency);
}

void UShieldComponent::CycleNext()
{
	if (ColorSet)
	{
		SetFrequency(ColorSet->GetNext(CurrentFrequency));
	}
	else
	{
		// fallback cycle through enum order
		int32 Idx = static_cast<int32>(CurrentFrequency);
		Idx = (Idx + 1) % 4; // 4 default colors
		SetFrequency(static_cast<EWaveFrequency>(Idx));
	}
}

void UShieldComponent::CyclePrev()
{
	if (ColorSet)
	{
		SetFrequency(ColorSet->GetPrev(CurrentFrequency));
	}
	else
	{
		int32 Idx = static_cast<int32>(CurrentFrequency);
		Idx = (Idx - 1 + 4) % 4;
		SetFrequency(static_cast<EWaveFrequency>(Idx));
	}
}

bool UShieldComponent::TryAbsorb(EWaveFrequency Incoming)
{
	// Resonance/autopass will be handled later by ScoringSubsystem; keep simple match
	if (Incoming == CurrentFrequency)
	{
		OnAbsorb.Broadcast();
		return true;
	}
	OnFail.Broadcast();
	return false;
}

bool UShieldComponent::TriggerOverload()
{
	// Minimal stub: always allow, cost applied by Game Rules later in controller/game state.
	// Return true to allow BP FX/SFX to fire.
	return true;
}
