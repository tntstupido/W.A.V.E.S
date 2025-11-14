#include "PC_Waves.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Pawn_Shield.h"

APC_Waves::APC_Waves() {}

void APC_Waves::BeginPlay()
{
	Super::BeginPlay();

	// Add mapping context at runtime
	if (ULocalPlayer* LP = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsys = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputContext)
			{
				Subsys->AddMappingContext(InputContext, /*Priority*/1);
			}
		}
	}
}

void APC_Waves::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (IA_FreqUp)
			EIC->BindAction(IA_FreqUp, ETriggerEvent::Started, this, &APC_Waves::OnFreqUp);

		if (IA_FreqDown)
			EIC->BindAction(IA_FreqDown, ETriggerEvent::Started, this, &APC_Waves::OnFreqDown);

		if (IA_Hold)
		{
			EIC->BindAction(IA_Hold, ETriggerEvent::Started, this, &APC_Waves::OnHoldPressed);
			EIC->BindAction(IA_Hold, ETriggerEvent::Completed, this, &APC_Waves::OnHoldReleased);
			EIC->BindAction(IA_Hold, ETriggerEvent::Canceled, this, &APC_Waves::OnHoldReleased);
		}

		if (IA_Overload)
			EIC->BindAction(IA_Overload, ETriggerEvent::Started, this, &APC_Waves::OnOverload);
	}

}

void APC_Waves::OnFreqUp()
{
	if (APawn_Shield* P = Cast<APawn_Shield>(GetPawn())) { P->NextFrequency(); }
}
void APC_Waves::OnFreqDown()
{
	if (APawn_Shield* P = Cast<APawn_Shield>(GetPawn())) { P->PrevFrequency(); }
}
void APC_Waves::OnHoldPressed()
{
	if (APawn_Shield* P = Cast<APawn_Shield>(GetPawn())) { P->BeginHold(); }
}
void APC_Waves::OnHoldReleased()
{
	if (APawn_Shield* P = Cast<APawn_Shield>(GetPawn())) { P->EndHold(); }
}
void APC_Waves::OnOverload()
{
	if (APawn_Shield* P = Cast<APawn_Shield>(GetPawn())) { P->TriggerOverload(); }
}
