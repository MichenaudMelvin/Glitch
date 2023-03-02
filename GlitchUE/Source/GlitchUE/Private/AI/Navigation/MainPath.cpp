// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Navigation/MainPath.h"

UMainPath::UMainPath(){
	DefaultCost = 0.1f;
	FixedAreaEnteringCost = 0;

	DrawColor = FColor(61, 126, 255, 38);
}
