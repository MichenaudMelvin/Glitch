// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TransitionPlayerController.h"

#include "Gamemodes/TransitionGamemode.h"
#include "Kismet/GameplayStatics.h"

void ATransitionPlayerController::BeginPlay(){
	Super::BeginPlay();

	TchatWidget = Cast<UTchat>(CreateWidget(this, TchatWidgetClass));
	Gamemode = Cast<ATransitionGamemode>(UGameplayStatics::GetGameMode(this));
}

UTchat* ATransitionPlayerController::GetTchatWidget() const{
	return TchatWidget;
}
