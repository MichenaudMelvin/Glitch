// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MainPlayerController.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Mark/Mark.h"

void AMainPlayerController::BeginPlay(){
	Super::BeginPlay();

	MainPlayer = Cast<AMainPlayer>(GetPawn());
	InteractionTickDelegate.BindDynamic(MainPlayer, &AMainPlayer::InteractionTick);

	CreatePlayerWidgets();

	GameMode = Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	SelectNewGameplayMode(EGameplayMode::Normal);
}

void AMainPlayerController::CreatePlayerWidgets_Implementation(){
	Tchat = Cast<UTchat>(CreateWidget(this, TchatWidgetClass));

	SightWidget = Cast<USightWidget>(CreateWidget(this, SightWidgetClass));

	TimerWidget = Cast<UTimerWidget>(CreateWidget(this, TimerWidgetClass));

	PlayerStatsWidget = Cast<UPlayerStats>(CreateWidget(this, PlayerStatsWidgetClass));
	PlayerStatsWidget->AddToViewport();
	PlayerStatsWidget->UpdateDisplayGolds(MainPlayer->GetGolds());

	WheelWidget = Cast<UWheel>(CreateWidget(this, WheelWidgetWidgetClass));

	HotBarWidget = Cast<UHotBar>(CreateWidget(this, HotBarWidgetWidgetClass));

	PauseWidget = Cast<UPauseMenu>(CreateWidget(this, PauseWidgetClass));

	PopUpWidget = Cast<UPopUpWidget>(CreateWidget(this, PopUpWidgetClass));
}

#pragma region Bind

#pragma region Movement

void AMainPlayerController::SelectNewGameplayMode(const EGameplayMode NewGameplayMode){
	GameplayMode = NewGameplayMode;
	switch (GameplayMode){
		case EGameplayMode::Normal:
			BindNormalMode();
			MainPlayer->CameraAimReverse();
			break;

		case EGameplayMode::Construction:
			BindConstructionMode();
			MainPlayer->CameraAim();
			break;

		case EGameplayMode::Destruction:
			BindNormalMode();
			MainPlayer->CameraAimReverse();
			break;
	}
}

EGameplayMode AMainPlayerController::GetGameplayMode() const{
	return GameplayMode;
}

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

void AMainPlayerController::BindConstruction(){
	GetWorld()->GetTimerManager().SetTimer(PreviewObjectTimerHandle, MainPlayer, &AMainPlayer::PreviewPlacableObject, 0.1f, true);
	OnPlaceObject.AddDynamic(MainPlayer, &AMainPlayer::PlacePlacableActor);
	BindMouseScroll();
}

void AMainPlayerController::UnbindConstruction(){
	GetWorld()->GetTimerManager().ClearTimer(PreviewObjectTimerHandle);
	OnPlaceObject.Clear();

	if(IsValid(MainPlayer->GetPreviewPlacableActor())){
		MainPlayer->StopPreviewMovement();
		MainPlayer->GetPreviewPlacableActor()->ResetActor();
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
	BindOpenSelectionWheel();
	BindMouseScroll();
	BindFastSaveAndLoad();
}

void AMainPlayerController::BindConstructionMode(){
	UnbindAll();
	BindMovement();
	BindCamera();
	BindConstruction();
	BindOpenSelectionWheel();
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
	OnMouseScroll.AddDynamic(HotBarWidget, &UHotBar::Scroll);
	OnMouseScroll.AddDynamic(this, &AMainPlayerController::ShowHotBar);
}

void AMainPlayerController::UnbindMouseScroll(){
	OnMouseScroll.Clear();
}

void AMainPlayerController::BindOpenSelectionWheel(){
	UnbindOpenSelectionWheel();
	OnOpenSelectionWheelPressed.AddDynamic(this, &AMainPlayerController::StartOpenWheelTimer);
	OnOpenSelectionWheelReleased.AddDynamic(this, &AMainPlayerController::CloseWheel);
}

void AMainPlayerController::UnbindOpenSelectionWheel(){
	OnOpenSelectionWheelPressed.Clear();
	OnOpenSelectionWheelReleased.Clear();
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
	UnbindConstruction();
	UnbindOpenSelectionWheel();
	UnbindMouseScroll();
	UnbindFastSaveAndLoad();
}

void AMainPlayerController::PauseGame(){
	UGameplayStatics::SetGamePaused(GetWorld(), !UGameplayStatics::IsGamePaused(GetWorld()));

	SetShowMouseCursor(!bShowMouseCursor);

	PauseWidget->IsInViewport() ? PauseWidget->RemoveFromParent() : PauseWidget->AddToViewport();
}

#pragma endregion

void AMainPlayerController::StartOpenWheelTimer(){
	GetWorld()->GetTimerManager().SetTimer(OpenWheelTimerHandle, this, &AMainPlayerController::OpenWheel, OpenWheelTimer, false, OpenWheelTimer);
}

void AMainPlayerController::OpenWheel(){
	WheelWidget->AddToViewport();
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, WheelWidget, EMouseLockMode::DoNotLock, false);

	UnbindCamera();
	UnbindMouseScroll();

	bShowMouseCursor = true;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), WheelTimeDilation);
}

void AMainPlayerController::CloseWheel(){
	if(GetWorld()->GetTimerManager().GetTimerElapsed(OpenWheelTimerHandle) > -1){
		SelectNewGameplayMode(EGameplayMode::Normal);
	}

	GetWorld()->GetTimerManager().ClearTimer(OpenWheelTimerHandle);
	WheelWidget->RemoveFromParent();

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	BindCamera();

	UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
	bShowMouseCursor = false;
}

void AMainPlayerController::ShowHotBar(float AxisValue){
	GetWorld()->GetTimerManager().ClearTimer(HotBarTimerHandle);

	if(!HotBarWidget->IsInViewport()){
		HotBarWidget->AddToViewport();
	}

	GetWorld()->GetTimerManager().SetTimer(HotBarTimerHandle, HotBarWidget, &UHotBar::PlayFadeOutAnimation, 0.1f, false, 2.0f);

}

UTchat* AMainPlayerController::GetTchatWidget() const{
	return Tchat;
}

USightWidget* AMainPlayerController::GetSightWidget() const{
	return SightWidget;
}

UTimerWidget* AMainPlayerController::GetTimerWidget() const{
	return TimerWidget;
}

UPlayerStats* AMainPlayerController::GetPlayerStatsWidget() const{
	return PlayerStatsWidget;
}


