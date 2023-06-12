// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Custom/CustomCheckbox.h"

UCustomCheckbox::UCustomCheckbox(){
	static ConstructorHelpers::FObjectFinder<UTexture> UncheckImage(TEXT("/Game/UI/Settings/T_BoolFalse"));
	check(UncheckImage.Succeeded());

	WidgetStyle.UncheckedImage.SetResourceObject(UncheckImage.Object);
	WidgetStyle.UncheckedImage.ImageSize = FVector2D(CheckBoxSize, CheckBoxSize);

	WidgetStyle.UncheckedHoveredImage.SetResourceObject(UncheckImage.Object);
	WidgetStyle.UncheckedHoveredImage.ImageSize = FVector2D(CheckBoxSize, CheckBoxSize);

	WidgetStyle.UncheckedPressedImage.SetResourceObject(UncheckImage.Object);
	WidgetStyle.UncheckedPressedImage.ImageSize = FVector2D(CheckBoxSize, CheckBoxSize);

	static ConstructorHelpers::FObjectFinder<UTexture> CheckedImage(TEXT("/Game/UI/Settings/T_BoolTrue"));
	check(CheckedImage.Succeeded());

	WidgetStyle.CheckedImage.SetResourceObject(CheckedImage.Object);
	WidgetStyle.CheckedImage.ImageSize = FVector2D(CheckBoxSize, CheckBoxSize);

	WidgetStyle.CheckedHoveredImage.SetResourceObject(CheckedImage.Object);
	WidgetStyle.CheckedHoveredImage.ImageSize = FVector2D(CheckBoxSize, CheckBoxSize);

	WidgetStyle.CheckedPressedImage.SetResourceObject(CheckedImage.Object);
	WidgetStyle.CheckedPressedImage.ImageSize = FVector2D(CheckBoxSize, CheckBoxSize);
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

void UCustomCheckbox::AllowFocus(){
	IUIFocus::AllowFocus();

	IsFocusable = true;
}

void UCustomCheckbox::DenyFocus(){
	IUIFocus::DenyFocus();

	IsFocusable = false;
}
