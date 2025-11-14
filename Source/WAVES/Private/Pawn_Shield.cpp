#include "Pawn_Shield.h"
#include "Components/StaticMeshComponent.h"
#include "ShieldComponent.h"
#include "GM_Waves.h"
#include "DA_WaveColorSet.h"
#include "Engine/World.h"

APawn_Shield::APawn_Shield()
{
	PrimaryActorTick.bCanEverTick = false;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	SetRootComponent(VisualMesh);

	Shield = CreateDefaultSubobject<UShieldComponent>(TEXT("Shield"));
}

void APawn_Shield::BeginPlay()
{
	Super::BeginPlay();

	// Inject ColorSet from GameMode if not set
	if (!Shield->ColorSet)
	{
		if (AGM_Waves* GM = GetWorld() ? GetWorld()->GetAuthGameMode<AGM_Waves>() : nullptr)
		{
			Shield->ColorSet = GM->GetColorSet();
		}
	}
	// Manual override (if you set it in BP)
	if (ColorSetOverride) { Shield->ColorSet = ColorSetOverride; }
}

void APawn_Shield::NextFrequency() { Shield->CycleNext(); }
void APawn_Shield::PrevFrequency() { Shield->CyclePrev(); }
void APawn_Shield::BeginHold() { Shield->BeginHold(); }
void APawn_Shield::EndHold() { Shield->EndHold(); }
void APawn_Shield::TriggerOverload() { Shield->TriggerOverload(); }
