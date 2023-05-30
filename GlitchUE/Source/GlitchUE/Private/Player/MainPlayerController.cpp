// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MainPlayerController.h"
#include "Gamemodes/GlitchUEGameMode.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Mark/Mark.h"

void AMainPlayerController::BeginPlay(){
	Super::BeginPlay();

	MainPlayer = Cast<AMainPlayer>(GetPawn());

	#if WITH_EDITOR
		if(!IsValid(MainPlayer)){
			UE_LOG(LogTemp, Warning, TEXT("Player not valid"));
			return;
		}
	#endif

	InteractionTickDelegate.BindDynamic(MainPlayer, &AMainPlayer::InteractionTick);

	CreatePlayerWidgets();

	GameMode = Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&](){
		ShowMouseCursor(false, nullptr);
	}, 0.01f, false);
}

void AMainPlayerController::CreatePlayerWidgets_Implementation(){
	Tchat = Cast<UTchat>(CreateWidget(this, TchatWidgetClass));

	SightWidget = Cast<USightWidget>(CreateWidget(this, SightWidgetClass));

	TimerWidget = Cast<UTimerWidget>(CreateWidget(this, TimerWidgetClass));

	PlayerStatsWidget = Cast<UPlayerStats>(CreateWidget(this, PlayerStatsWidgetClass));
	PlayerStatsWidget->AddToViewport();
	PlayerStatsWidget->UpdateDisplayGolds(MainPlayer->GetGolds());

	WheelWidget = Cast<UWheel>(CreateWidget(this, WheelWidgetWidgetClass));

	PauseWidget = Cast<UPauseMenu>(CreateWidget(this, PauseWidgetClass));

	AdditionalMessageWidget = Cast<UAdditionalMessage>(CreateWidget(this, AdditionalMessageWidgetClass));

	PopUpWidget = Cast<UPopUpWidget>(CreateWidget(this, PopUpWidgetClass));
}

#pragma region Bind

#pragma region Movement

void AMainPlayerController::BindFastSaveAndLoad(){
	UnbindFastSaveAndLoad();
	OnFastSave.AddDynamic(this, &AMainPlayerController::FastSave);
	OnFastLoad.AddDynamic(this, &AMainPlayerController::FastLoad);
}

void AMainPlayerController::UnbindFastSaveAndLoad(){
	OnFastSave.Clear();
	OnFastLoad.Clear();
}

void AMainPlayerController::FastSave(){
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Saving"));
	GameMode->GlobalWorldSave(0);
}

void AMainPlayerController::FastLoad(){
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Loading"));
	GameMode->GlobalWorldLoad(0);
}

void AMainPlayerController::SetCanSave(const bool bValue){
	bCanSave = bValue;
	bCanSave ? BindFastSaveAndLoad() : UnbindFastSaveAndLoad();
}

bool AMainPlayerController::CanSave() const{
	return bCanSave;
}

void AMainPlayerController::BindMovement(){
	UnbindMovement();
	OnMoveForward.AddUniqueDynamic(MainPlayer, &AMainPlayer::MoveForward);
	OnMoveRight.AddDynamic(MainPlayer, &AMainPlayer::MoveRight);
	BindSneak();
	BindSprint();
	BindJump();
}

void AMainPlayerController::UnbindMovement(){
	OnMoveForward.Clear();
	OnMoveRight.Clear();
	UnbindSneak();
	UnbindSprint();
	UnbindJump();
}

void AMainPlayerController::BindJump(){
	UnbindJump();
	OnJumpPressed.AddUniqueDynamic(MainPlayer, &AMainPlayer::Jump);
	OnJumpReleased.AddDynamic(MainPlayer, &AMainPlayer::StopJumping);
}

void AMainPlayerController::UnbindJump(){
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

void AMainPlayerController::UnbindSneak(){
	OnSneakPressed.Clear();
	OnSneakReleased.Clear();
}

void AMainPlayerController::BindSprint(){
	UnbindSprint();
	switch (MainPlayer->GetMovementMode()){
		case EPlayerMovementMode::Normal:
			OnSprint.AddDynamic(MainPlayer, &AMainPlayer::Dash);
			break;
	case EPlayerMovementMode::Sneaking:
			OnSprint.AddDynamic(MainPlayer, &AMainPlayer::SneakToSprint);
			break;
		case EPlayerMovementMode::Sprinting:
			OnSprint.AddDynamic(MainPlayer, &AMainPlayer::Dash);
			break;
	}
}

void AMainPlayerController::UnbindSprint(){
	OnSprint.Clear();
}

void AMainPlayerController::BindGlitch(){
	UnbindGlitch();
	if (MainPlayer->GetMark()->GetIsMarkPlaced()){
		OnUseGlitchPressed.AddDynamic(MainPlayer, &AMainPlayer::TPToMark);
	} else{
		OnUseGlitchPressed.AddDynamic(MainPlayer, &AMainPlayer::UseGlitchPressed);
		OnUseGlitchReleased.AddDynamic(MainPlayer, &AMainPlayer::UseGlitchReleassed);
	}
}

void AMainPlayerController::UnbindGlitch(){
	OnUseGlitchPressed.Clear();
	OnUseGlitchReleased.Clear();
}

void AMainPlayerController::BindInteraction(){
	UnbindInteraction();
	OnInteractPlayer.AddDynamic(MainPlayer, &AMainPlayer::Interact);
	GetWorld()->GetTimerManager().SetTimer(InteractionTimer, InteractionTickDelegate, 0.1f, true, 0.0f);
}

void AMainPlayerController::UnbindInteraction(){
	OnInteractPlayer.Clear();
	GetWorldTimerManager().ClearTimer(InteractionTimer);
}

#pragma endregion

#pragma region Camera

void AMainPlayerController::BindCamera(){
	UnbindCamera();

	OnTurn.AddDynamic(MainPlayer, &AMainPlayer::AddControllerYawInput);
	OnTurnRate.AddDynamic(MainPlayer, &AMainPlayer::TurnAtRate);

	OnLookUp.AddDynamic(MainPlayer, &AMainPlayer::AddControllerPitchInput);
	OnLookUpRate.AddDynamic(MainPlayer, &AMainPlayer::LookUpAtRate);
}

void AMainPlayerController::UnbindCamera(){
	OnTurn.Clear();
	OnTurnRate.Clear();
	OnLookUp.Clear();
	OnLookUpRate.Clear();
}

#pragma endregion

#pragma region Modes

void AMainPlayerController::BindNormalMode(){
	UnbindAll();
	BindPause();
	BindMovement();
	BindCamera();
	BindInteraction();
	BindGlitch();
	BindMouseScroll();
	BindFastSaveAndLoad();
}

#pragma endregion

void AMainPlayerController::BindPause(){
	UnbindPause();
	OnPause.AddDynamic(this, &AMainPlayerController::PauseGame);
}

void AMainPlayerController::UnbindPause(){
	OnPause.Clear();
}

void AMainPlayerController::BindMouseScroll(){
	UnbindMouseScroll();
}

void AMainPlayerController::UnbindMouseScroll(){
	OnMouseScroll.Clear();
}

void AMainPlayerController::BindClingMovement(){
	UnbindMovement();
	UnbindInteraction();
	UnbindGlitch();

	OnMoveForward.AddDynamic(MainPlayer, &AMainPlayer::ClingUp);
	OnMoveRight.AddDynamic(MainPlayer, &AMainPlayer::ClingRight);
}

void AMainPlayerController::UnbindAll(){
	UnbindMovement();
	UnbindCamera();
	UnbindInteraction();
	UnbindGlitch();
	UnbindMouseScroll();
	UnbindFastSaveAndLoad();
	UnbindPause();
}

void AMainPlayerController::PauseGame(){
	UGameplayStatics::SetGamePaused(GetWorld(), !UGameplayStatics::IsGamePaused(GetWorld()));

	PauseWidget->IsInViewport() ? PauseWidget->RemoveFromParent() : PauseWidget->AddToViewport();

	ShowMouseCursor(!bShowMouseCursor, PauseWidget);
	PrimaryActorTick;
}

#pragma endregion

void AMainPlayerController::OpenWheel(){
	UnbindAll();
	WheelWidget->AddToViewport();

	OnPause.AddDynamic(this, &AMainPlayerController::CloseWheel);
	OnUseGlitchPressed.AddDynamic(this, &AMainPlayerController::CloseWheel);

	ShowMouseCursor(true, WheelWidget);
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), WheelTimeDilation);
}

void AMainPlayerController::CloseWheel(){
	BindNormalMode();
	UnbindGlitch();
	OnUseGlitchReleased.AddDynamic(this, &AMainPlayerController::BindGlitch);

	WheelWidget->RemoveFromParent();
	CameraBlend(MainPlayer, CloseWheelBlend);
	MainPlayer->GetPreviewPlacableActor()->ResetActor();

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

	ShowMouseCursor(false, nullptr);
}

bool AMainPlayerController::IsWheelOpened() const{
	return WheelWidget->IsInViewport();
}

void AMainPlayerController::CameraBlend(AActor* BlendTarget, const float BlendTime){
	SetViewTargetWithBlend(BlendTarget, BlendTime, VTBlend_EaseInOut, 1);
}

UTchat* AMainPlayerController::GetTchatWidget() const{
	return Tchat;
}

USightWidget* AMainPlayerController::GetSightWidget() const{
	return SightWidget;
}

UWheel* AMainPlayerController::GetWheelWidget() const{
	return WheelWidget;
}

float AMainPlayerController::GetWheelTimeDilation() const{
	return WheelTimeDilation;
}

UTimerWidget* AMainPlayerController::GetTimerWidget() const{
	return TimerWidget;
}

UPlayerStats* AMainPlayerController::GetPlayerStatsWidget() const{
	return PlayerStatsWidget;
}

UAdditionalMessage* AMainPlayerController::GetAdditionalMessageWidget() const{
	return AdditionalMessageWidget;
}


