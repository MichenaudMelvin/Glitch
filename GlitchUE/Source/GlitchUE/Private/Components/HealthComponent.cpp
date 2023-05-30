// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/HealthComponent.h"

#include "Kismet/KismetMathLibrary.h"

UHealthComponent::UHealthComponent(){
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay(){
	Super::BeginPlay();
	CurrentHealth = MaxHealth;

	OriginalMaxHealth = MaxHealth;

	OnHealthChange.Broadcast();
}

void UHealthComponent::TakeDamages(const float DamagesAmount){
	if(!bCanTakeDamages){
		return;
	}

	CurrentHealth = FMath::Clamp((CurrentHealth - DamagesAmount), 0.0f, MaxHealth);

	OnHealthChange.Broadcast();
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
	// à faire
}

bool UHealthComponent::GetCanTakeDamages() const{
	return bCanTakeDamages;
}

float UHealthComponent::GetCurrentHealth() const{
	return CurrentHealth;
}

float UHealthComponent::GetHealthPercentage() const{
	return GetCurrentHealth() * 100 / GetMaxHealth();
}

float UHealthComponent::GetNormalizedHealth() const{
	return UKismetMathLibrary::NormalizeToRange(CurrentHealth, 0, GetMaxHealth());
}

float UHealthComponent::GetMaxHealth() const{
	return MaxHealth;
}

void UHealthComponent::SetCanTakeDamages(bool bValue){
	bCanTakeDamages = bValue;
}

void UHealthComponent::SetMaxHealth(const float NewMaxHealth){
	const float OldMaxHealth = MaxHealth;
	MaxHealth = NewMaxHealth;

	CurrentHealth += NewMaxHealth - OldMaxHealth;

	OnHealthChange.Broadcast();
}

float UHealthComponent::GetOriginalMaxHealth() const{
	return OriginalMaxHealth;
}

void UHealthComponent::ResetHealth(){
	CurrentHealth = MaxHealth;
}


