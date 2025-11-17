// Source/Waves/Private/Actor_Wave.cpp
#include "Actor_Wave.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ShieldComponent.h"

AActor_Wave::AActor_Wave()
{
	PrimaryActorTick.bCanEverTick = true;

	// Collision root
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(Collision);

	Collision->InitSphereRadius(100.f);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionResponseToAllChannels(ECR_Overlap);
	Collision->SetGenerateOverlapEvents(true);

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AActor_Wave::HandleOverlap);

	// Visual mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Collision);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AActor_Wave::BeginPlay()
{
	Super::BeginPlay();
	LifeRemaining = Spec.LifeSeconds;
}

void AActor_Wave::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// simple forward movement
	AddActorWorldOffset(MoveDir.GetSafeNormal() * Spec.Speed * DeltaSeconds, false);

	// lifetime
	if (Spec.LifeSeconds > 0.f)
	{
		LifeRemaining -= DeltaSeconds;
		if (LifeRemaining <= 0.f)
		{
			Destroy();
		}
	}
}

void AActor_Wave::SetFrequency(EWaveFrequency NewFreq)
{
	Spec.Frequency = NewFreq;

	UE_LOG(LogTemp, Log, TEXT("Wave %s SetFrequency -> %d"),
		*GetName(), (int32)Spec.Frequency);

	// Notify Blueprint side to update visuals
	OnFrequencyChanged_BP();
}


void AActor_Wave::HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)
	{
		return;
	}

	if (UShieldComponent* Shield = OtherActor->FindComponentByClass<UShieldComponent>())
	{
		const EWaveFrequency WaveFreq = Spec.Frequency;
		const EWaveFrequency ShieldFreq = Shield->GetCurrentFrequency(); // or Shield->CurrentFrequency if public

		const bool bAbsorbed = Shield->TryAbsorb(WaveFreq);

		UE_LOG(LogTemp, Log, TEXT("Overlap: WaveFreq=%d ShieldFreq=%d Absorbed=%s"),
			(int32)WaveFreq,
			(int32)ShieldFreq,
			bAbsorbed ? TEXT("TRUE") : TEXT("FALSE"));

		if (bAbsorbed)
		{
			Destroy();
		}
	}
}

