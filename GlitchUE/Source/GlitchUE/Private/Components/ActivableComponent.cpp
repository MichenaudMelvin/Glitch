// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ActivableComponent.h"

UActivableComponent::UActivableComponent(){
	PrimaryComponentTick.bCanEverTick = false;
	State = EState::Desactivated;
}

EState UActivableComponent::GetState(){
	return State;
}

void UActivableComponent::ActivateObject(){
	if(State == EState::Activated){
		return;
	}

	State = EState::Activated;
	OnActivated.Broadcast();
}

void UActivableComponent::DesactivateObject(){
	if(State == EState::Desactivated){
		return;
	}

	State = EState::Desactivated;
	OnDesactivated.Broadcast();
}

bool UActivableComponent::IsActivated() const{
	return State == EState::Activated;
}
