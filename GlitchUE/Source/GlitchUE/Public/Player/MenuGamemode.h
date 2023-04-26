// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Saves/SaveInterface.h"
#include "MenuGamemode.generated.h"

UCLASS()
class GLITCHUE_API AMenuGamemode : public AGameModeBase, public ISaveInterface{
	GENERATED_BODY()

public:
	virtual void GlobalWorldLoad(const int Index) override;

	#pragma region ConsoleCommands

	UFUNCTION(Exec)
	void RemoveAllWidgets() const;

	#pragma endregion
};
