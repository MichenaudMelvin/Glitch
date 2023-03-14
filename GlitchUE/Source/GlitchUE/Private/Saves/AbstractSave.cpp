// Fill out your copyright notice in the Description page of Project Settings.


#include "Saves/AbstractSave.h"

FString UAbstractSave::GetSlotName() const{
	return SlotName;
}

int UAbstractSave::GetIndex() const{
	return Index;
}
