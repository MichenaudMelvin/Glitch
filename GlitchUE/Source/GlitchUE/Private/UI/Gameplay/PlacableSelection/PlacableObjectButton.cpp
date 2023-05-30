// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/PlacableSelection/PlacableObjectButton.h"
#include "UI/Gameplay/PlacableSelection/Wheel.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MainPlayerController.h"

UPlacableObjectButton::UPlacableObjectButton(){
	static ConstructorHelpers::FObjectFinder<UTexture> Texture(TEXT("/Game/UI/Wheel/T_ConstructionButton"));
	check(Texture.Succeeded());

	WidgetStyle.Normal.SetResourceObject(Texture.Object);
	WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Image;

	WidgetStyle.Hovered.SetResourceObject(Texture.Object);
	WidgetStyle.Hovered.DrawAs = ESlateBrushDrawType::Image;

	WidgetStyle.Pressed.SetResourceObject(Texture.Object);
	WidgetStyle.Pressed.DrawAs = ESlateBrushDrawType::Image;
}

void UPlacableObjectButton::SynchronizeProperties(){
	Super::SynchronizeProperties();

	MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if(!IsValid(MainPlayer)){
		return;
	}

	MainPlayerController = MainPlayer->GetMainPlayerController();

	Wheel = MainPlayerController->GetWheelWidget();

	if(IsValid(Data)){
		const FString ShowName = Data->Name.ToString() + " (Cost: " + FString::FromInt(Data->Cost) + ")";
		Name = NewObject<UTextBlock>();
		AddChild(Name);
		Name->SetText(FText::FromString(ShowName));
		Name->SetAutoWrapText(true);
		Name->SetJustification(ETextJustify::Center);
		Name->Font.OutlineSettings.OutlineSize = 1;
	}
}

void UPlacableObjectButton::OnClick(){
	if(MainPlayer->GetPreviewPlacableActor()->CanBePlaced()){
		MainPlayer->PlacePlacableActor();
		MainPlayerController->CloseWheel();
	}
}

void UPlacableObjectButton::Select(){
	MainPlayer->SetPlacableActorData(Data);
	Wheel->SetDescription(Data->Description);
}

void UPlacableObjectButton::UnSelect(){
	MainPlayer->SetPlacableActorData(nullptr);
	Wheel->SetDescription(FText::FromString(""));
}

void UPlacableObjectButton::BindButtons(){
	UnbindButtons();
	OnHovered.AddDynamic(this, &UPlacableObjectButton::Select);	OnUnhovered.AddDynamic(this, &UPlacableObjectButton::UnSelect);
	OnClicked.AddDynamic(this, &UPlacableObjectButton::OnClick);
}

void UPlacableObjectButton::UnbindButtons(){
	OnHovered.Clear();
	OnUnhovered.Clear();
	OnClicked.Clear();
}

bool UPlacableObjectButton::CompareData(const UPlacableActorData* DataToCompare) const{
	return DataToCompare->Name == Data->Name;
}

void UPlacableObjectButton::SetWheel(UWheel* NewWheel){
	Wheel = NewWheel;
}

void UPlacableObjectButton::ReceiveFocus(){
	Super::ReceiveFocus();

	Select();
}

void UPlacableObjectButton::UnReceiveFocus(){
	Super::UnReceiveFocus();

	UnSelect();
}