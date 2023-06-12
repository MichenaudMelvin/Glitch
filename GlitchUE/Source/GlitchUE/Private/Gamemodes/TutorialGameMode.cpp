// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemodes/TutorialGameMode.h"

void ATutorialGameMode::LaunchStealthTimer(float TimerValue){
	if(CanLaunchStealthTimer){
		Super::LaunchStealthTimer(TimerValue);
	}
}
