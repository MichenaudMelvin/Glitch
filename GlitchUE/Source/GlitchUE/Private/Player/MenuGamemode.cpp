// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MenuGamemode.h"
#include "Helpers/FunctionsLibrary/UsefulFunctions.h"
#include "Kismet/GameplayStatics.h"
#include "Saves/WorldSave.h"

void AMenuGamemode::GlobalWorldLoad(const int Index){
	ISaveInterface::GlobalWorldLoad(Index);

	const UWorldSave* CurrentSave = Cast<UWorldSave>(UUsefulFunctions::LoadSave(UWorldSave::StaticClass(), Index, false));

	if(CurrentSave == nullptr){
		return;
	}

	FString LoadOptions = "WorldSaveLoad|";

	LoadOptions += FString::FromInt(Index);

	UGameplayStatics::OpenLevel(this, FName("Level_1"), true, LoadOptions);
}