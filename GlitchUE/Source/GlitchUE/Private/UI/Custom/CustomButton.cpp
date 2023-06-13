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

	NormalStyle = WidgetStyle;
}

void UCustomButton::BlockButton(const bool bUnbindHovering){
	bIsBlocked = true;
	bUnbindHovering ? SetVisibility(ESlateVisibility::HitTestInvisible) : SetVisibility(ESlateVisibility::Visible);

	NormalStyle.Normal.TintColor = BlockedColor;
	NormalStyle.Hovered.TintColor = BlockedColor;
	NormalStyle.Pressed.TintColor = BlockedColor;
	SetStyle(NormalStyle);
}

void UCustomButton::UnblockButton(){
	bIsBlocked = false;
	SetVisibility(ESlateVisibility::Visible);

	NormalStyle.Normal.TintColor = FSlateColor(FLinearColor::White);
	NormalStyle.Hovered.TintColor = FSlateColor(FLinearColor::White);
	NormalStyle.Pressed.TintColor = FSlateColor(FLinearColor::White);
	SetStyle(NormalStyle);
}

bool UCustomButton::IsBlocked() const{
	return bIsBlocked;
}

void UCustomButton::ReceiveFocus(){
	IUIFocus::ReceiveFocus();

	WidgetStyle.Normal = NormalStyle.Hovered;
	SetStyle(WidgetStyle);
}

void UCustomButton::UnReceiveFocus(){
	IUIFocus::UnReceiveFocus();

	WidgetStyle.Normal = NormalStyle.Normal;
	SetStyle(WidgetStyle);
}