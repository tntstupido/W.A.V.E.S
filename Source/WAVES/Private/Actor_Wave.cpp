#include "Actor_Wave.h"
#include "Components/StaticMeshComponent.h"
#include "ShieldComponent.h"

AActor_Wave::AActor_Wave()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetGenerateOverlapEvents(true);
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AActor_Wave::OnWaveBeginOverlap);
}

void AActor_Wave::BeginPlay()
{
	Super::BeginPlay();
	LifeRemaining = Spec.LifeSeconds;
}

void AActor_Wave::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// simple constant velocity
	const FVector Delta = MoveDir.GetSafeNormal() * Spec.Speed * DeltaSeconds;
	AddActorWorldOffset(Delta, /*bSweep*/false);

	// lifetime
	if (Spec.LifeSeconds > 0.f)
	{
		LifeRemaining -= DeltaSeconds;
		if (LifeRemaining <= 0.f) { Destroy(); }
	}
}

void AActor_Wave::OnWaveBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 /*OtherBodyIndex*/, bool /*bFromSweep*/, const FHitResult& /*SweepResult*/)
{
	if (!OtherActor) return;

	UE_LOG(LogTemp, Log, TEXT("Wave overlapped: %s"), *OtherActor->GetName());
	Destroy();

	// If the other actor has a shield component, attempt absorb.
	if (UShieldComponent* Shield = OtherActor->FindComponentByClass<UShieldComponent>())
	{
		const bool bAbsorbed = Shield->TryAbsorb(Spec.Frequency);
		// Destroy on absorb; keep on fail (so it can damage/continue) — tweak later.
		if (bAbsorbed) { Destroy(); }
	}
}
