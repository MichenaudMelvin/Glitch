// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Custom/CustomCheckbox.h"

UCustomCheckbox::UCustomCheckbox(){
	static ConstructorHelpers::FObjectFinder<UTexture> UncheckImage(TEXT("/Game/UI/Settings/T_BoolFalse"));
	check(UncheckImage.Succeeded());

	WidgetStyle.UncheckedImage.SetResourceObject(UncheckImage.Object);

	WidgetStyle.UncheckedHoveredImage.SetResourceObject(UncheckImage.Object);

	WidgetStyle.UncheckedPressedImage.SetResourceObject(UncheckImage.Object);

	static ConstructorHelpers::FObjectFinder<UTexture> CheckedImage(TEXT("/Game/UI/Settings/T_BoolTrue"));
	check(CheckedImage.Succeeded());

	WidgetStyle.CheckedImage.SetResourceObject(CheckedImage.Object);

	WidgetStyle.CheckedHoveredImage.SetResourceObject(CheckedImage.Object);

	WidgetStyle.CheckedPressedImage.SetResourceObject(CheckedImage.Object);
}

void UCustomCheckbox::SynchronizeProperties(){
	Super::SynchronizeProperties();

	SavedStyle = WidgetStyle;
}

void UCustomCheckbox::ReceiveFocus(){
	IUIFocus::ReceiveFocus();

	IsChecked() ? WidgetStyle.CheckedImage = SavedStyle.CheckedHoveredImage : WidgetStyle.UncheckedImage = SavedStyle.UncheckedHoveredImage;
}

void UCustomCheckbox::UnReceiveFocus(){
	IUIFocus::UnReceiveFocus();

	IsChecked() ? WidgetStyle.CheckedImage = SavedStyle.CheckedImage : WidgetStyle.UncheckedImage = SavedStyle.UncheckedImage;
}
