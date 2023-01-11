// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/HealthComponent.h"

UHealthComponent::UHealthComponent(){
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay(){
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

void UHealthComponent::TakeDamages(float DamagesAmount){
	if(!bCanTakeDamages){
		return;
	}

	CurrentHealth = FMath::Clamp((CurrentHealth - DamagesAmount), 0.0f, MaxHealth);
	
	OnReciveDamages.Broadcast();

	if(CurrentHealth != 0){
		return;
	}

	OnHealthNull.Broadcast();
}

void UHealthComponent::TakeMaxDamages(){
	TakeDamages(CurrentHealth);
}

void UHealthComponent::AddHealth(float HealthAmount){
	// � faire
}

bool UHealthComponent::GetCanTakeDamages(){
	return bCanTakeDamages;
}

void UHealthComponent::SetCanTakeDamages(bool bValue){
	bCanTakeDamages = bValue;
}


