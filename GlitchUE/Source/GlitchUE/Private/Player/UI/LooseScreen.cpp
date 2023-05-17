// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UI/LooseScreen.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MainPlayerController.h"

void ULooseScreen::NativeConstruct(){
	Super::NativeConstruct();

	AMainPlayerController* Controller = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	Controller->UnbindAll();
}
