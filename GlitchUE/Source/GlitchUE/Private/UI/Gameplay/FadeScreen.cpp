// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/FadeScreen.h"

void UFadeScreen::NativeConstruct(){
	Super::NativeConstruct();

	const EUMGSequencePlayMode::Type PlayMode = bFadeIn ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse;

	PlayAnimation(FadeAnimation, 0, 1, PlayMode, FadeTime);
}
