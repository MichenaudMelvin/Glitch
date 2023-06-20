// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Custom/FocusableSlider.h"

UFocusableSlider::UFocusableSlider(){
	static ConstructorHelpers::FObjectFinder<UTexture> BarImage(TEXT("/Game/UI/Settings/T_SliderBar"));
	check(BarImage.Succeeded());

	NormalBarBrush.SetResourceObject(BarImage.Object);
	HoveredBarBrush.SetResourceObject(BarImage.Object);

	static ConstructorHelpers::FObjectFinder<UTexture> ThumbImage(TEXT("/Game/UI/Settings/T_BoolFalse"));
	check(ThumbImage.Succeeded());

	NormalThumbBrush.SetResourceObject(ThumbImage.Object);
	NormalThumbBrush.SetImageSize(FVector2D(33.0f, 33.0f));

	HoveredThumbBrush.SetResourceObject(ThumbImage.Object);
	HoveredThumbBrush.SetImageSize(FVector2D(33.0f, 33.0f));
	HoveredThumbBrush.TintColor = FSlateColor(FLinearColor(0.129412, 0.235294, 0.262745));

	WidgetStyle.SetNormalBarImage(NormalBarBrush);
	WidgetStyle.SetHoveredBarImage(HoveredBarBrush);

	WidgetStyle.SetNormalThumbImage(NormalThumbBrush);
	WidgetStyle.SetHoveredThumbImage(HoveredThumbBrush);
}

void UFocusableSlider::ReceiveFocus(){
	IUIFocus::ReceiveFocus();

	WidgetStyle.SetNormalBarImage(HoveredBarBrush);
	WidgetStyle.SetNormalThumbImage(HoveredThumbBrush);
}

void UFocusableSlider::UnReceiveFocus(){
	IUIFocus::UnReceiveFocus();

	WidgetStyle.SetNormalBarImage(NormalBarBrush);
	WidgetStyle.SetNormalThumbImage(NormalThumbBrush);
}
