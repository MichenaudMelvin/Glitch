// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/PlacableSelection/PlacableObjectButton.h"
#include "Components/ButtonSlot.h"
#include "Components/VerticalBoxSlot.h"
#include "UI/Gameplay/PlacableSelection/Wheel.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Font.h"
#include "Player/MainPlayerController.h"

UPlacableObjectButton::UPlacableObjectButton(){
	static ConstructorHelpers::FObjectFinder<UTexture> TextureNormal(TEXT("/Game/UI/Wheel/T_WheelButtonLeft"));
	check(TextureNormal.Succeeded());

	WidgetStyle.Normal.SetResourceObject(TextureNormal.Object);
	WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Image;

	static ConstructorHelpers::FObjectFinder<UTexture> TextureHovered(TEXT("/Game/UI/Wheel/T_WheelButtonLeftHovered"));
	check(TextureHovered.Succeeded());

	WidgetStyle.Hovered.SetResourceObject(TextureHovered.Object);
	WidgetStyle.Hovered.DrawAs = ESlateBrushDrawType::Image;

	WidgetStyle.Pressed.SetResourceObject(TextureHovered.Object);
	WidgetStyle.Pressed.DrawAs = ESlateBrushDrawType::Image;

	static ConstructorHelpers::FObjectFinder<UFont> FontTXT(TEXT("/Game/UI/Fonts/FONT_MenuFont"));
	check(FontTXT.Succeeded());

	TextFont.FontObject = FontTXT.Object;
	TextFont.TypefaceFontName = "Bold";
	TextFont.Size = 26;

	static ConstructorHelpers::FObjectFinder<UFont> FontNBR(TEXT("/Game/UI/Fonts/FONT_NumberFont"));
	check(FontNBR.Succeeded());

	NumberFont.FontObject = FontNBR.Object;
	NumberFont.Size = 26;
}

void UPlacableObjectButton::SynchronizeProperties(){
	Super::SynchronizeProperties();

	MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if(!IsValid(MainPlayer)){
		return;
	}

	MainPlayerController = MainPlayer->GetMainPlayerController();

	Wheel = MainPlayerController->GetWheelWidget();

	if(!IsValid(Data)){
		return;
	}

	const FString ShowName = Data->Name.ToString();
	const FString Cost = "Cost: " + FString::FromInt(Data->Cost);

	if(!IsValid(TextHolder)){
		TextHolder = NewObject<UVerticalBox>();
		AddChild(TextHolder);

		UButtonSlot* TextHolderSlot = Cast<UButtonSlot>(TextHolder->Slot);
		TextHolderSlot->SetHorizontalAlignment(HAlign_Fill);
		TextHolderSlot->SetVerticalAlignment(VAlign_Fill);
	}

	if(!IsValid(PlacableName)){
		PlacableName = NewObject<UTextBlock>();
		TextHolder->AddChild(PlacableName);
		PlacableName->SetText(FText::FromString(ShowName));
		PlacableName->SetAutoWrapText(true);
		PlacableName->SetJustification(ETextJustify::Center);
		PlacableName->SetFont(TextFont);

		UVerticalBoxSlot* PlacableNameSlot = Cast<UVerticalBoxSlot>(PlacableName->Slot);
		PlacableNameSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		PlacableNameSlot->SetHorizontalAlignment(HAlign_Fill);
		PlacableNameSlot->SetVerticalAlignment(VAlign_Bottom);
	}

	if(!IsValid(CostText)){
		CostText = NewObject<UTextBlock>();
		TextHolder->AddChild(CostText);
		CostText->SetText(FText::FromString(Cost));
		CostText->SetAutoWrapText(true);
		CostText->SetJustification(ETextJustify::Center);
		CostText->SetFont(NumberFont);

		UVerticalBoxSlot* CostTextSlot = Cast<UVerticalBoxSlot>(CostText->Slot);
		CostTextSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		CostTextSlot->SetHorizontalAlignment(HAlign_Fill);
		CostTextSlot->SetVerticalAlignment(VAlign_Top);
	}
}

void UPlacableObjectButton::OnClick(){
	if(MainPlayer->GetPreviewPlacableActor()->CanBePlaced()){
		MainPlayer->PlacePlacableActor();
		MainPlayerController->CloseWheel();

		Wheel->ResetLastFocusWidgetIndex();
	}
}

void UPlacableObjectButton::Select(){
	MainPlayer->SetPlacableActorData(Data);
	Wheel->SetDescription(Data);
}

void UPlacableObjectButton::UnSelect(){
	MainPlayer->SetPlacableActorData(nullptr);
	Wheel->SetDescription(nullptr);
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

bool UPlacableObjectButton::CanBuyObject(const int Golds) const{
	return Golds >= Data->Cost;
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