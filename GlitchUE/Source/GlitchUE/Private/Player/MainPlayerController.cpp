// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/World.h"
#include "Player/MainPlayerController.h"

#pragma region Bind

#pragma region Movement

void AMainPlayerController::BindMovement_Implementation() {
	UnbindMovement_Implementation();
	//OnMoveForward.AddUniqueDynamic(MainPlayer, &AMainPlayer::MoveForward);
	//OnMoveRight.AddDynamic(MainPlayer, &AMainPlayer::MoveRight);
	BindSneak_Implementation();
	BindSprint_Implementation();
	BindJump_Implementation();
}

void AMainPlayerController::UnbindMovement_Implementation() {
	//OnMoveForward.Clear();
	//OnMoveRight.Clear();
	UnbindSneak_Implementation();
	UnbindSprint_Implementation();
	UnbindJump_Implementation();
}

void AMainPlayerController::BindJump_Implementation() {
	UnbindJump_Implementation();
	//OnJumpPressed.AddUniqueDynamic(MainPlayer, &AMainPlayer::Jump);
	//OnJumpReleased.AddDynamic(MainPlayer, &AMainPlayer::StopJumping);
}

void AMainPlayerController::UnbindJump_Implementation() {
	//OnJumpPressed.Clear();
	//OnJumpReleased.Clear();
}

void AMainPlayerController::BindSneak_Implementation(){
	UnbindSneak_Implementation();
}

void AMainPlayerController::UnbindSneak_Implementation() {

}

void AMainPlayerController::BindSprint_Implementation() {
	UnbindSprint_Implementation();
}

void AMainPlayerController::UnbindSprint_Implementation() {

}

void AMainPlayerController::BindInteraction_Implementation() {
	UnbindInteraction_Implementation();
	//OnInteractPlayer.AddDynamic(MainPlayer, &AMainPlayer::Interact);
	GetWorld()->GetTimerManager().SetTimer(InteractionTimer, 0.1f, true, 0.0f);
}

void AMainPlayerController::UnbindInteraction_Implementation() {
	//OnInteractPlayer.Clear();
	InteractionTimer.Invalidate();
}

#pragma endregion

#pragma region Camera

void AMainPlayerController::BindCamera_Implementation() {
	UnbindCamera_Implementation();

	/*
	OnTurn.AddDynamic(MainPlayer, &AMainPlayer::AddControllerYawInput);
	OnTurnRate.AddDynamic(MainPlayer, &AMainPlayer::TurnAtRate);
	
	OnLookUp.AddDynamic(MainPlayer, &AMainPlayer::AddControllerPitchInput);
	OnLookUpRate.AddDynamic(MainPlayer, &AMainPlayer::LookUpAtRate);
	*/
}

void AMainPlayerController::UnbindCamera_Implementation() {
	/*
	OnTurn.Clear();
	OnTurnRate.Clear();
	OnLookUp.Clear();
	OnLookUpRate.Clear();
	*/
}

#pragma endregion

#pragma region Modes

void AMainPlayerController::BindNormalMode_Implementation() {
	UnbindAll_Implementation();
	BindPause_Implementation();
	BindMovement_Implementation();
	BindCamera_Implementation();
	BindInteraction_Implementation();
	// bind glitch
	// bind open selection wheel
}

void AMainPlayerController::BindConstructionMode_Implementation() {
	UnbindAll_Implementation();
	BindMovement_Implementation();
	BindCamera_Implementation();
	// bind construction
	// bind open selection wheel
}


#pragma endregion

void AMainPlayerController::BindPause_Implementation() {
	UnbindPause_Implementation();
	// bind pause
}

void AMainPlayerController::UnbindPause_Implementation() {
	OnPause.Clear();
}

void AMainPlayerController::UnbindAll_Implementation(){
	UnbindMovement_Implementation();
	UnbindCamera_Implementation();
	UnbindInteraction_Implementation();
	// unbind glitch
	// unbind construction
	// unbind open selection wheel
	// unbind rotate objects
}

#pragma endregion