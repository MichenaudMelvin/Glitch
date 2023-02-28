// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UI/PlacableSelection/HotBar.h"
#include "Helpers/FunctionsLibrary/UsefullFunctions.h"
#include "Player/MainPlayerController.h"
#include "Player/UI/PlacableSelection/SelectionButton.h"

void UHotBar::NativeConstruct(){
	Super::NativeConstruct();

	PlayAnimation(FadeIn, 0, 1, EUMGSequencePlayMode::Forward, 2, false);
	SelectIndex();
}

void UHotBar::Scroll(const float Value){
	ButtonList[CurrentIndex]->UnSelect();

	const int IntValue = static_cast<float>(Value);

	switch (IntValue){
	case -1:
		CurrentIndex--;
		break;

	case 1:
		CurrentIndex++;
		break;
	}

	CurrentIndex = UUsefullFunctions::ClampIntToArrayLength(CurrentIndex, ButtonList.Num());

	ButtonList[CurrentIndex]->Select();
}

void UHotBar::SelectIndex(){
	switch (MainPlayerController->GetGameplayMode()){
	case EGameplayMode::Normal: 
		NormalModeButton->Select();
		CurrentIndex = 0;
		break;
	case EGameplayMode::Construction: 
		for(int i = 0; i < ButtonList.Num(); i++){
			if(ButtonList[i]->IsA(UPlacableObjectButton::StaticClass())){
				if(Cast<UPlacableObjectButton>(ButtonList[i])->CompareData(MainPlayer->GetCurrentPlacableActorData())){
					CurrentIndex = i;
					return;
				}
			}
		}
		break;
	case EGameplayMode::Destruction: 
		DestructionModeButton->Select();
		CurrentIndex = ButtonList.Num() - 1;
		break;
	}
}

void UHotBar::PlayFadeOutAnimation(){
	PlayAnimation(FadeIn, 0, 1, EUMGSequencePlayMode::Reverse, 1, false);

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
		RemoveFromParent();
	}, FadeIn->GetEndTime(), false);
}
