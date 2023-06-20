// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Custom/CustomSlider.h"
#include "Components/HorizontalBoxSlot.h"
#include "Kismet/KismetTextLibrary.h"

UCustomSlider::UCustomSlider(){
	static ConstructorHelpers::FObjectFinder<UFont> FontNBR(TEXT("/Game/UI/Fonts/FONT_NumberFont"));
	check(FontNBR.Succeeded());

	NumberFont.FontObject = FontNBR.Object;
	NumberFont.Size = 30;
}

void UCustomSlider::PostLoad(){
	Super::PostLoad();

	if(!IsValid(Slider)){
		Slider = NewObject<UFocusableSlider>();
		AddChild(Slider);

		Slider->OnValueChanged.AddDynamic(this, &UCustomSlider::UpdateText);

		Slider->SetMinValue(SliderMinValue);

		Slider->SetMaxValue(SliderMaxValue);

		Slider->SetStepSize(StepSize);

		UHorizontalBoxSlot* SliderSlot = Cast<UHorizontalBoxSlot>(Slider->Slot);
		SliderSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		SliderSlot->SetHorizontalAlignment(HAlign_Fill);
		SliderSlot->SetVerticalAlignment(VAlign_Fill);
	}

	if(!IsValid(SliderValue)){
		SliderValue = NewObject<UTextBlock>();
		AddChild(SliderValue);
		SliderValue->SetAutoWrapText(true);
		SliderValue->SetJustification(ETextJustify::Center);
		SliderValue->SetFont(NumberFont);
		UpdateText(0);

		UHorizontalBoxSlot* SliderValueSlot = Cast<UHorizontalBoxSlot>(SliderValue->Slot);
		SliderValueSlot->SetHorizontalAlignment(HAlign_Fill);
		SliderValueSlot->SetVerticalAlignment(VAlign_Center);
		SliderValueSlot->SetPadding(FMargin(50, 0, 50, 0));
	}
}

void UCustomSlider::UpdateText(float Value){
	SliderValue->SetText(UKismetTextLibrary::Conv_FloatToText(Value, HalfToEven, false, true, IntegralDigits, IntegralDigits, FractionalDigits, FractionalDigits));
}

void UCustomSlider::ReceiveFocus(){
	IUIFocus::ReceiveFocus();
}

void UCustomSlider::UnReceiveFocus(){
	IUIFocus::UnReceiveFocus();
}

void UCustomSlider::SetValue(const float Value){
	Slider->SetValue(Value);
	UpdateText(Value);
}

USlider* UCustomSlider::GetSlider() const{
	return Slider;
}
