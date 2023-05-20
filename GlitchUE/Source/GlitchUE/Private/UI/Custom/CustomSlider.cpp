// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Custom/CustomSlider.h"
#include "Kismet/KismetTextLibrary.h"

void UCustomSlider::NativeOnInitialized(){
	Super::NativeOnInitialized();

	Slider->OnValueChanged.AddDynamic(this, &UCustomSlider::UpdateText);

	Slider->SetMinValue(SliderMinValue);

	Slider->SetMaxValue(SliderMaxValue);
}

void UCustomSlider::UpdateText(float Value){
	SliderValue->SetText(UKismetTextLibrary::Conv_FloatToText(Value, HalfToEven, false, true, IntegralDigits, IntegralDigits, FractionalDigits, FractionalDigits));
}

void UCustomSlider::SetValue(const float Value){
	Slider->SetValue(Value);
	UpdateText(Value);
}

USlider* UCustomSlider::GetSlider() const{
	return Slider;
}
