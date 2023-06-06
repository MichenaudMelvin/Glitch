// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gamemodes/MainGamemode.h"
#include "Saves/SaveInterface.h"
#include "MenuGamemode.generated.h"

UCLASS()
class GLITCHUE_API AMenuGamemode : public AMainGamemode, public ISaveInterface{
	GENERATED_BODY()

public:
	virtual void GlobalWorldLoad(const int Index) override;
};
