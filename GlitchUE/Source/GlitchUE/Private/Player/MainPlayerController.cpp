// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MainPlayerController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Mark/Mark.h"

void AMainPlayerController::BeginPlay() {
	Super::BeginPlay();
	
	MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));

	InteractionTickDelegate.BindDynamic(MainPlayer, &AMainPlayer::InteractionTick);
	SelectNewGameplayMode(EGameplayMode::CPF_Normal);
}

#pragma region Bind

#pragma region Movement

void AMainPlayerController::SelectNewGameplayMode(EGameplayMode NewGameplayMode){
	GameplayMode = NewGameplayMode;
	switch (GameplayMode){
		case EGameplayMode::CPF_Normal:
			BindNormalMode();
			//CameraAimRevrse
			break;
		
		case EGameplayMode::CPF_Construction:
			BindConstructionMode();
			//CamreaAim
			break;
	}
}

EGameplayMode AMainPlayerController::GetGameplayMode(){
	return GameplayMode;
}

void AMainPlayerController::BindMovement() {
	UnbindMovement();
	OnMoveForward.AddUniqueDynamic(MainPlayer, &AMainPlayer::MoveForward);
	OnMoveRight.AddDynamic(MainPlayer, &AMainPlayer::MoveRight);
	BindSneak();
	BindSprint();
	BindJump();
}

void AMainPlayerController::UnbindMovement() {
	OnMoveForward.Clear();
	OnMoveRight.Clear();
	UnbindSneak();
	UnbindSprint();
	UnbindJump();
}

void AMainPlayerController::BindJump() {
	UnbindJump();
	OnJumpPressed.AddUniqueDynamic(MainPlayer, &AMainPlayer::Jump);
	OnJumpReleased.AddDynamic(MainPlayer, &AMainPlayer::StopJumping);
}

void AMainPlayerController::UnbindJump() {
	OnJumpPressed.Clear();
	OnJumpReleased.Clear();
}

void AMainPlayerController::BindSneak(){
	UnbindSneak();
	OnSneakReleased.AddDynamic(MainPlayer, &AMainPlayer::SneakReleased);

	switch (MainPlayer->GetMovementMode()){
	case EPlayerMovementMode::Normal:
		OnSneakPressed.AddDynamic(MainPlayer, &AMainPlayer::SneakPressed);
		break;
	case EPlayerMovementMode::Sneaking:
		OnSneakPressed.AddDynamic(MainPlayer, &AMainPlayer::SneakPressed);
		break;
	case EPlayerMovementMode::Sprinting:
		OnSneakPressed.AddDynamic(MainPlayer, &AMainPlayer::SprintToSneak);
		break;
	}
}

void AMainPlayerController::UnbindSneak() {
	OnSneakPressed.Clear();
	OnSneakReleased.Clear();
}

void AMainPlayerController::BindSprint_Implementation() {
	UnbindSprint();
	//switch (){
	//
	//}
}

void AMainPlayerController::BindConstruction_Implementation(){}

void AMainPlayerController::UnbindConstruction_Implementation(){}

void AMainPlayerController::UnbindSprint() {
	OnSprint.Clear();
}

void AMainPlayerController::BindGlitch(){
	UnbindGlitch();
	if (MainPlayer->GetMark()->GetIsMarkPlaced()) {
		OnUseGlitchPressed.AddDynamic(MainPlayer, &AMainPlayer::TPToMark);
	} else {
		OnUseGlitchPressed.AddDynamic(MainPlayer, &AMainPlayer::UseGlitchPressed);
		OnUseGlitchReleased.AddDynamic(MainPlayer, &AMainPlayer::UseGlitchReleassed);
	}
}

void AMainPlayerController::UnbindGlitch(){
	OnUseGlitchPressed.Clear();
	OnUseGlitchReleased.Clear();
}

void AMainPlayerController::BindInteraction() {
	UnbindInteraction();
	OnInteractPlayer.AddDynamic(MainPlayer, &AMainPlayer::Interact);
	GetWorld()->GetTimerManager().SetTimer(InteractionTimer, InteractionTickDelegate, 0.1f, true, 0.0f);
}

void AMainPlayerController::UnbindInteraction() {
	OnInteractPlayer.Clear();
	GetWorldTimerManager().ClearTimer(InteractionTimer);
}

#pragma endregion

#pragma region Camera

void AMainPlayerController::BindCamera() {
	UnbindCamera();

	OnTurn.AddDynamic(MainPlayer, &AMainPlayer::AddControllerYawInput);
	OnTurnRate.AddDynamic(MainPlayer, &AMainPlayer::TurnAtRate);
	
	OnLookUp.AddDynamic(MainPlayer, &AMainPlayer::AddControllerPitchInput);
	OnLookUpRate.AddDynamic(MainPlayer, &AMainPlayer::LookUpAtRate);
}

void AMainPlayerController::UnbindCamera() {
	OnTurn.Clear();
	OnTurnRate.Clear();
	OnLookUp.Clear();
	OnLookUpRate.Clear();
}

#pragma endregion

#pragma region Modes

void AMainPlayerController::BindNormalMode() {
	UnbindAll();
	BindPause();
	BindMovement();
	BindCamera();
	BindInteraction();
	BindGlitch();
	BindOpenSelectionWheel();
}

void AMainPlayerController::BindConstructionMode() {
	UnbindAll();
	BindMovement();
	BindCamera();
	BindConstruction();
	BindOpenSelectionWheel();
}


#pragma endregion

void AMainPlayerController::BindPause() {
	UnbindPause();
	OnPause.AddDynamic(this, &AMainPlayerController::PauseGame);
}

void AMainPlayerController::UnbindPause() {
	OnPause.Clear();
}

void AMainPlayerController::BindOpenSelectionWheel_Implementation(){}

void AMainPlayerController::UnbindOpenSelectionWheel_Implementation(){}

void AMainPlayerController::UnbindAll(){
	UnbindMovement();
	UnbindCamera();
	UnbindInteraction();
	UnbindGlitch();
	UnbindConstruction();
	UnbindOpenSelectionWheel();
	// unbind rotate objects
}

void AMainPlayerController::PauseGame_Implementation(){
	UGameplayStatics::SetGamePaused(GetWorld(), !UGameplayStatics::IsGamePaused(GetWorld()));
	SetShowMouseCursor(!bShowMouseCursor);
}

#pragma endregion