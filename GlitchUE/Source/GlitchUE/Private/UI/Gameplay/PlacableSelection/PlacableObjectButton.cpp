// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/PlacableSelection/PlacableObjectButton.h"
#include "UI/Gameplay/PlacableSelection/Wheel.h"
#include "Components/TextBlock.h"
#include "Player/MainPlayerController.h"

void UPlacableObjectButton::NativeOnInitialized(){
	Super::NativeOnInitialized();

	const FString ShowName = Data->Name.ToString() + " (Cost: " + FString::FromInt(Data->Cost) + ")";
	Name->SetText(FText::FromString(ShowName));

	Wheel = MainPlayerController->GetWheelWidget();
}

void UPlacableObjectButton::OnClick(){
	if(MainPlayer->GetPreviewPlacableActor()->CanBePlaced()){
		MainPlayer->PlacePlacableActor();
		MainPlayerController->CloseWheel();
	}
}

void UPlacableObjectButton::Select(){
	Super::Select();

	MainPlayer->SetPlacableActorData(Data);
	Wheel->SetDescription(Data->Description);
}

void UPlacableObjectButton::UnSelect(){
	Super::UnSelect();

	MainPlayer->SetPlacableActorData(nullptr);
	Wheel->SetDescription(FText::FromString(""));
}

void UPlacableObjectButton::BindButtons(){
	Super::BindButtons();

	Image->OnClicked.AddDynamic(this, &UPlacableObjectButton::OnClick);
}

void UPlacableObjectButton::UnbindButtons(){
	Super::UnbindButtons();

	Image->OnClicked.Clear();
}

bool UPlacableObjectButton::CompareData(const UPlacableActorData* DataToCompare) const{
	return DataToCompare->Name == Data->Name;
}

void UPlacableObjectButton::SetWheel(UWheel* NewWheel){
	Wheel = NewWheel;
}
