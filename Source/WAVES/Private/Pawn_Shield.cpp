// Source/Waves/Private/Pawn_Shield.cpp
#include "Pawn_Shield.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ShieldComponent.h"

#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

APawn_Shield::APawn_Shield()
{
	PrimaryActorTick.bCanEverTick = false;

	// Collision root
	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(Collision);

	// Set box size (extent is half-size in each axis)
	Collision->InitBoxExtent(FVector(120.f, 120.f, 200.f));

	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionResponseToAllChannels(ECR_Overlap);
	Collision->SetGenerateOverlapEvents(true);

	// Shield logic
	Shield = CreateDefaultSubobject<UShieldComponent>(TEXT("Shield"));

	// --- Planet mesh ---
	PlanetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlanetMesh"));
	PlanetMesh->SetupAttachment(RootComponent);
	PlanetMesh->SetRelativeLocation(FVector::ZeroVector);
	PlanetMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// --- Orbit root (for rotating shield around planet) ---
	OrbitRoot = CreateDefaultSubobject<USceneComponent>(TEXT("OrbitRoot"));
	OrbitRoot->SetupAttachment(RootComponent);
	OrbitRoot->SetRelativeLocation(FVector::ZeroVector);

	// --- Shield mesh ---

	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
	ShieldMesh->SetupAttachment(OrbitRoot);
	// Place shield at +X radius, then we will rotate OrbitRoot around Z to make it orbit
	ShieldMesh->SetRelativeLocation(FVector(OrbitRadius, 0.f, 0.f));
	ShieldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// --- Camera ---
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 1200.f; // distance from planet
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = false;

	// Place camera slightly back and up, looking towards planet
	SpringArm->SetRelativeLocation(FVector(-400.f, 0.f, 400.f));
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
}

void APawn_Shield::BeginPlay()
{
	Super::BeginPlay();
}

void APawn_Shield::NextFrequency()
{
	if (Shield) Shield->CycleNext();

	CurrentSegmentIndex = (CurrentSegmentIndex + 1) % 4;

	UpdateShieldOrbit();
}

void APawn_Shield::PrevFrequency()
{
	if (Shield) Shield->CyclePrev();

	CurrentSegmentIndex = (CurrentSegmentIndex - 1 + 4) % 4;

	UpdateShieldOrbit();
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

void APawn_Shield::UpdateShieldOrbit()
{
	// Wrap to [0..3] just in case
	CurrentSegmentIndex = (CurrentSegmentIndex % 4 + 4) % 4;

	const float YawDegrees = OrbitStepDegrees * CurrentSegmentIndex;

	// Rotate around Z (Yaw) to move shield around planet
	const FRotator NewRot(0.f, YawDegrees, 0.f);
	OrbitRoot->SetRelativeRotation(NewRot);

	// Make sure shield is at correct radius (if OrbitRadius changed)
	ShieldMesh->SetRelativeLocation(FVector(OrbitRadius, 0.f, 0.f));
}
