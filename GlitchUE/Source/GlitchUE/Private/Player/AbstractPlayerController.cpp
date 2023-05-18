// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AbstractPlayerController.h"

void AAbstractPlayerController::ShowMouseCursor(const bool Show){
	DefaultMouseCursor = Show ? EMouseCursor::Default : EMouseCursor::None;
	bShowMouseCursor = Show;
}
