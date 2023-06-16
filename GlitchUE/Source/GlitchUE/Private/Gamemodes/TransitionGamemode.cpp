// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemodes/TransitionGamemode.h"
#include "Kismet/GameplayStatics.h"

void ATransitionGamemode::BeginPlay(){
	Super::BeginPlay();

	TransitionPlayerController = Cast<ATransitionPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&](){
		if(IsValid(TransitionPlayerController)){
			TchatWidget = TransitionPlayerController->GetTchatWidget();
		}
	}, 0.1f, false);
}