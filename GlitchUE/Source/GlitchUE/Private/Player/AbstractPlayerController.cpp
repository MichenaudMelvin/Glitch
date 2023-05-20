// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AbstractPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void AAbstractPlayerController::ShowMouseCursor(const bool bShow, UUserWidget* WidgetToFocus){
	DefaultMouseCursor = bShow ? EMouseCursor::Default : EMouseCursor::None;
	bShowMouseCursor = bShow;

	bShow ? UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, WidgetToFocus, EMouseLockMode::DoNotLock, true) : UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
}
