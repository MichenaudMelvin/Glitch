// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TutorialPlayerController.h"

void ATutorialPlayerController::BeginPlay(){
	Super::BeginPlay();

	SetCanSave(bAllowSaving);
}

void ATutorialPlayerController::BindMovement(){
	if(bShouldBindMovement){
		Super::BindMovement();
	}
}

void ATutorialPlayerController::BindCamera(){
	if(bShouldBindCamera){
		Super::BindCamera();
	}
}

void ATutorialPlayerController::BindJump(){
	if(bShouldBindJump){
		Super::BindJump();
	}
}

void ATutorialPlayerController::BindSneak(){
	if(bShouldBindJump){
		Super::BindSneak();
	}
}

void ATutorialPlayerController::BindSprint(){
	if(bShouldBindJump){
		Super::BindSprint();
	}
}

void ATutorialPlayerController::BindGlitch(){
	if(bShouldBindGlitch){
		Super::BindGlitch();
	}
}

void ATutorialPlayerController::BindInteraction(){
	if(bShouldBindInteraction){
		Super::BindInteraction();
	}
}

void ATutorialPlayerController::SetCanSave(bool bValue){
	if(!bAllowSaving){
		bValue = false;
	}

	Super::SetCanSave(bValue);
}
