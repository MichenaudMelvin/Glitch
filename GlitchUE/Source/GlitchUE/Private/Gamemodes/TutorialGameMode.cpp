// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemodes/TutorialGameMode.h"

ATutorialGameMode::ATutorialGameMode(){
	bUseAutoObjectivesForPlayer = false;
}

void ATutorialGameMode::LaunchStealthTimer(float TimerValue){
	if(CanLaunchStealthTimer){
		Super::LaunchStealthTimer(TimerValue);
	}
}
