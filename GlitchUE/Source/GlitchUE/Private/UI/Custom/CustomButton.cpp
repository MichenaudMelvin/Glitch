// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Custom/CustomButton.h"

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
