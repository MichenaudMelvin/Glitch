// Fill out your copyright notice in the Description page of Project Settings.


#include "PlacableObject/Trap.h"
#include "Components/InteractableComponent.h"

ATrap::ATrap(){
	ActivableComp = CreateDefaultSubobject<UActivableComponent>(TEXT("Activable"));
}

void ATrap::BeginPlay(){
	Super::BeginPlay();
}

void ATrap::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	if (ActivableComp->GetState() == EState::CPF_Desactivated) {
		ActivableComp->ActivateObject();

		FTimerHandle TimerHandle;

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
			ActivableComp->DesactivateObject();
		}, TrapDuration, false);
	}
}

void ATrap::GlitchUpgrade(){
	TrapAttackRate = Cast<UTrapData>(CurrentData)->UpgradedGlitchAttackRate;

	Super::GlitchUpgrade();
}

void ATrap::SetData(UPlacableActorData* NewData){
	Super::SetData(NewData);

	UTrapData* Data = Cast<UTrapData>(NewData);

	Damages = Data->TrapDamages;
	TrapDuration = Data->TrapDuration;
	TrapAttackRate = Data->TrapAttackRate;
}
