// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/WaveManager.h"

AWaveManager::AWaveManager(){
	PrimaryActorTick.bCanEverTick = true;

}

void AWaveManager::BeginPlay(){
	Super::BeginPlay();

	//Player = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	
}
