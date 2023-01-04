// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ActivableComponent.h"

UActivableComponent::UActivableComponent(){
	PrimaryComponentTick.bCanEverTick = false;

}

EState UActivableComponent::GetState(){
	return State;
}

void UActivableComponent::ActivateObject(){
	State = EState::CPF_Activated;
	OnActivated.Broadcast();
}

void UActivableComponent::DesactivateObject(){
	State = EState::CPF_Desactivated;
	OnDesactivated.Broadcast();
}
