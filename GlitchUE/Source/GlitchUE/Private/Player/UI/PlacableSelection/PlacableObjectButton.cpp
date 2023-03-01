// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UI/PlacableSelection/PlacableObjectButton.h"
#include "Components/TextBlock.h"
#include "Player/MainPlayerController.h"

void UPlacableObjectButton::NativeOnInitialized(){
	Super::NativeOnInitialized();
	
	Name->SetText(FText::FromName(Data->Name));
}

void UPlacableObjectButton::Select(){
	Super::Select();

	if(bApplyWhenSelected){
		MainPlayer->SetPlacableActorData(Data);

		switch (MainPlayerController->GetGameplayMode()){
		case EGameplayMode::Normal: 
			MainPlayerController->SelectNewGameplayMode(EGameplayMode::Construction);
			break;
		case EGameplayMode::Construction: 
			break;
		case EGameplayMode::Destruction: 
			MainPlayerController->SelectNewGameplayMode(EGameplayMode::Construction);
			break;
		}
	}
}

bool UPlacableObjectButton::CompareData(const UPlacableActorData* DataToCompare) const{
	return DataToCompare->Name == Data->Name;
}
