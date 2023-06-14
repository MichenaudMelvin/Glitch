// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Custom/CustomComboBoxString.h"
#include "Engine/Font.h"

UCustomComboBoxString::UCustomComboBoxString(){
	static ConstructorHelpers::FObjectFinder<UTexture> NormalImage(TEXT("/Game/UI/Menu/T_ComboBoxButton"));
	check(NormalImage.Succeeded());

	WidgetStyle.ComboButtonStyle.ButtonStyle.Normal.SetResourceObject(NormalImage.Object);
	WidgetStyle.ComboButtonStyle.ButtonStyle.Normal.DrawAs = ESlateBrushDrawType::Image;

	static ConstructorHelpers::FObjectFinder<UTexture> HoveredImage(TEXT("/Game/UI/Menu/T_ComboBoxButtonHovered"));
	check(HoveredImage.Succeeded());

	WidgetStyle.ComboButtonStyle.ButtonStyle.Hovered.SetResourceObject(HoveredImage.Object);
	WidgetStyle.ComboButtonStyle.ButtonStyle.Hovered.DrawAs = ESlateBrushDrawType::Image;

	WidgetStyle.ComboButtonStyle.ButtonStyle.Pressed.SetResourceObject(HoveredImage.Object);
	WidgetStyle.ComboButtonStyle.ButtonStyle.Pressed.DrawAs = ESlateBrushDrawType::Image;

	static ConstructorHelpers::FObjectFinder<UTexture> ArrowImage(TEXT("/Game/UI/Menu/T_ArrowDown"));
	check(ArrowImage.Succeeded());

	WidgetStyle.ComboButtonStyle.DownArrowImage.SetResourceObject(ArrowImage.Object);

	const FSlateColor TargetColor = FSlateColor(FLinearColor(0.130136f, 0.234551f, 0.262251f));
	ItemStyle.ActiveBrush.TintColor = TargetColor;
	ItemStyle.ActiveHoveredBrush.TintColor = TargetColor;
	ItemStyle.SelectedTextColor = FSlateColor(FLinearColor::White);

	static ConstructorHelpers::FObjectFinder<UFont> NumberFont(TEXT("/Game/UI/Fonts/FONT_NumberFont"));
	check(NumberFont.Succeeded());

	Font.FontObject = NumberFont.Object;
	Font.Size = 20;

	ForegroundColor = FSlateColor(FLinearColor::White);
}

void UCustomComboBoxString::SynchronizeProperties(){
	Super::SynchronizeProperties();

	NormalStyle = WidgetStyle.ComboButtonStyle.ButtonStyle;
}

void UCustomComboBoxString::ReceiveFocus(){
	IUIFocus::ReceiveFocus();

	WidgetStyle.ComboButtonStyle.ButtonStyle.Normal = NormalStyle.Hovered;
	WidgetStyle.SetComboButtonStyle(WidgetStyle.ComboButtonStyle);
}

void UCustomComboBoxString::UnReceiveFocus(){
	IUIFocus::UnReceiveFocus();

	WidgetStyle.ComboButtonStyle.ButtonStyle.Normal = NormalStyle.Normal;
	WidgetStyle.SetComboButtonStyle(WidgetStyle.ComboButtonStyle);
}
