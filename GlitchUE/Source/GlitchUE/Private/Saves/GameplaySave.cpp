// Fill out your copyright notice in the Description page of Project Settings.


#include "Saves/GameplaySave.h"

UGameplaySave::UGameplaySave(){
	SlotName = "GameplaySave";
}

bool UGameplaySave::IsUsingGamepad() const{
	return LastPressedKey.IsGamepadKey();
}
