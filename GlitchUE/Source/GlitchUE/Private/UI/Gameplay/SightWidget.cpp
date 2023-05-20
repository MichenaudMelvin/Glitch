// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/SightWidget.h"

void USightWidget::UpdateOpacity(const float OpacityValue) const{
	Sight->SetOpacity(OpacityValue);
}
