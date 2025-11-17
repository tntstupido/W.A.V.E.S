// Source/Waves/Private/Pawn_Shield.cpp
#include "Pawn_Shield.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ShieldComponent.h"

APawn_Shield::APawn_Shield()
{
	PrimaryActorTick.bCanEverTick = false;

	// Collision root
	Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	SetRootComponent(Collision);

	Collision->InitCapsuleSize(120.f, 200.f);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionResponseToAllChannels(ECR_Overlap);
	Collision->SetGenerateOverlapEvents(true);

	// Visual mesh
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	VisualMesh->SetupAttachment(Collision);
	VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Shield logic
	Shield = CreateDefaultSubobject<UShieldComponent>(TEXT("Shield"));
}

void APawn_Shield::BeginPlay()
{
	Super::BeginPlay();
}

void APawn_Shield::NextFrequency()
{
	if (Shield) Shield->CycleNext();
}

void APawn_Shield::PrevFrequency()
{
	if (Shield) Shield->CyclePrev();
}

void APawn_Shield::BeginHold()
{
	if (Shield) Shield->BeginHold();
}

void APawn_Shield::EndHold()
{
	if (Shield) Shield->EndHold();
}

void APawn_Shield::TriggerOverload()
{
	if (Shield) Shield->TriggerOverload();
}
