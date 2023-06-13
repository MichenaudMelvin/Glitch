// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Custom/CustomButton.h"

UCustomButton::UCustomButton(){
	static ConstructorHelpers::FObjectFinder<UTexture> NormalImage(TEXT("/Game/UI/Menu/T_MenuButton"));
	check(NormalImage.Succeeded());

	WidgetStyle.Normal.SetResourceObject(NormalImage.Object);
	WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Image;

	static ConstructorHelpers::FObjectFinder<UTexture> HoveredImage(TEXT("/Game/UI/Menu/T_MenuButtonHover"));
	check(HoveredImage.Succeeded());

	WidgetStyle.Hovered.SetResourceObject(HoveredImage.Object);
	WidgetStyle.Hovered.DrawAs = ESlateBrushDrawType::Image;

	static ConstructorHelpers::FObjectFinder<UTexture> PressedImage(TEXT("/Game/UI/Menu/T_MenuButtonOnClick"));
	check(PressedImage.Succeeded());

	WidgetStyle.Pressed.SetResourceObject(PressedImage.Object);
	WidgetStyle.Pressed.DrawAs = ESlateBrushDrawType::Image;
}

void UCustomButton::SynchronizeProperties(){
	Super::SynchronizeProperties();

	SavedStyle = WidgetStyle;
}

void UCustomButton::ReceiveFocus(){
	IUIFocus::ReceiveFocus();

	WidgetStyle.Normal = SavedStyle.Hovered;
	SetStyle(WidgetStyle);
}

void UCustomButton::UnReceiveFocus(){
	IUIFocus::UnReceiveFocus();

	WidgetStyle.Normal = SavedStyle.Normal;
	SetStyle(WidgetStyle);
}