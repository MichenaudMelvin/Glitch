// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AbstractPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Helpers/FunctionsLibrary/UsefulFunctions.h"
#include "Kismet/KismetInputLibrary.h"

void AAbstractPlayerController::BeginPlay(){
	Super::BeginPlay();

	OnAnyKey.AddDynamic(this, &AAbstractPlayerController::PressedAnyKey);

	GameplaySave = Cast<UGameplaySave>(UUsefulFunctions::LoadSave(UGameplaySave::StaticClass(), 0, true));
}

void AAbstractPlayerController::Destroyed(){
	Super::Destroyed();

	UUsefulFunctions::SaveToSlot(GameplaySave, 0);
}

void AAbstractPlayerController::Tick(float DeltaSeconds){
	Super::Tick(DeltaSeconds);

	if(!IsUsingGamepad()){
		return;
	}

	const FVector2D CurrentMousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

	if(GameplaySave->LastedMousePosition != CurrentMousePosition){
		GameplaySave->LastedMousePosition = CurrentMousePosition;
		GameplaySave->LastPressedKey = EKeys::LeftMouseButton;
		OnSwitchToKeyboard.Broadcast();
	}
}

void AAbstractPlayerController::PressedAnyKey(FKey KeyMap){
	// gamepad key
	if((KeyMap.IsGamepadKey() && !GameplaySave->LastPressedKey.IsGamepadKey())){
		OnSwitchToGamepad.Broadcast();
	}

	// keyboard or mouse key
	else if(KeyMap.IsMouseButton() && !GameplaySave->LastPressedKey.IsMouseButton() || (UKismetInputLibrary::Key_IsKeyboardKey(KeyMap) && !UKismetInputLibrary::Key_IsKeyboardKey(GameplaySave->LastPressedKey))){
		OnSwitchToKeyboard.Broadcast();
	}

	GameplaySave->LastPressedKey = KeyMap;
}

void AAbstractPlayerController::ShowMouseCursor(const bool bShow, UUserWidget* WidgetToFocus){
	DefaultMouseCursor = bShow ? EMouseCursor::Default : EMouseCursor::None;
	bShowMouseCursor = bShow;

	bShow ? UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, WidgetToFocus, EMouseLockMode::DoNotLock, true) : UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
}

bool AAbstractPlayerController::IsUsingGamepad() const{
	return GameplaySave->IsUsingGamepad();
}

bool AAbstractPlayerController::IsGameplaySaveValid() const{
	return IsValid(GameplaySave);
}
