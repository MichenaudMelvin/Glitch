// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/Inhibiteur.h"

AInhibiteur::AInhibiteur() {
	InteractableComp = CreateDefaultSubobject<UInteractableComponent>(TEXT("InhibiteurInteraction"));
}