// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/PlacableSelection/PlacableObjectButton.h"
#include "Components/TextBlock.h"
#include "Player/MainPlayerController.h"

void UPlacableObjectButton::NativeOnInitialized(){
	Super::NativeOnInitialized();

	const FString ShowName = Data->Name.ToString() + " (Cost: " + FString::FromInt(Data->Cost) + ")";
	Name->SetText(FText::FromString(ShowName));
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
