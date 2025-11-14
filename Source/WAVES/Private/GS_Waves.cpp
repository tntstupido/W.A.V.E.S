#include "GS_Waves.h"

AGS_Waves::AGS_Waves()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGS_Waves::BeginPlay()
{
	Super::BeginPlay();
}

void AGS_Waves::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	ElapsedSeconds += DeltaSeconds;
}

void AGS_Waves::SetPlanetEnergy(int32 NewValue)
{
	if (PlanetEnergy == NewValue) return;
	PlanetEnergy = NewValue;
	OnEnergyChanged.Broadcast(PlanetEnergy);
}

void AGS_Waves::AddEnergy(int32 Delta)
{
	SetPlanetEnergy(PlanetEnergy + Delta);
}
