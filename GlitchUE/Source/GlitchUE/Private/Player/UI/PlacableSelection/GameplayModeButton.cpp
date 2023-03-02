// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UI/PlacableSelection/GameplayModeButton.h"
#include "Components/TextBlock.h"

void UGameplayModeButton::NativeOnInitialized(){
	Super::NativeOnInitialized();

	switch (GameplayMode){
	case EGameplayMode::Normal:
		Name->SetText(FText::FromString("NormalMode"));
		break;
	case EGameplayMode::Destruction:
		Name->SetText(FText::FromString("DestructionMode"));
		break;
	}
}

void UGameplayModeButton::Select(){
	Super::Select();

	if(bApplyWhenSelected){
		MainPlayerController->SelectNewGameplayMode(GameplayMode);
	}
}
