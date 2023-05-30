// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/EnemyHealthBar.h"

void UEnemyHealthBar::UpdateHealth(const float HealthPercentage) const{
	HealthBar->SetPercent(HealthPercentage);
}
