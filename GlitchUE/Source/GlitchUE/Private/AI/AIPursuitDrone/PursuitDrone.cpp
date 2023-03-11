// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIPursuitDrone/PursuitDrone.h"
#include "Components/CapsuleComponent.h"
#include "Player/MainPlayer.h"

APursuitDrone::APursuitDrone(){
	PrimaryActorTick.bCanEverTick = false;

	InteractableComp = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interaction"));
}

void APursuitDrone::BeginPlay(){
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APursuitDrone::OnTouchSomething);
}

void APursuitDrone::OnTouchSomething(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if(OtherActor->IsA(AMainPlayer::StaticClass())){
		AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);

		if(!Player->IsInGlitchZone() && Player->GetHealthComp()->GetCanTakeDamages()){
			Player->GetHealthComp()->TakeDamages(AIController->GetDamages());
			Destroy();
		}
	}
}

void APursuitDrone::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	Blackboard->SetValueAsBool("IsPickedUp", true);
}

UInteractableComponent* APursuitDrone::GetInteractableComp() const{
	return InteractableComp;
}
