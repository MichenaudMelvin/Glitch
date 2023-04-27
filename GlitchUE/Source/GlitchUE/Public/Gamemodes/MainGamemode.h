// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameUserSettings.h"
#include "FMODVCA.h"
#include "MainGamemode.generated.h"

UCLASS()
class GLITCHUE_API AMainGamemode : public AGameModeBase{
	GENERATED_BODY()

public:
	AMainGamemode();

protected:
	virtual void BeginPlay() override;

	void UpdateGlobalSettings() const;

	UGameUserSettings* GameUserSettings;

	UPROPERTY()
	UFMODVCA* MasterBank;

	UPROPERTY()
	UFMODVCA* MusicBank;

	UPROPERTY()
	UFMODVCA* SFXBank;

#pragma region ConsoleCommands

private:
	UFUNCTION(Exec)
	void SetGlobalTimeDilation(float TimeDilation) const;

	UFUNCTION(Exec)
	void CrashGame() const;

	UFUNCTION(Exec)
	void CollectGarbage() const;

	// this one should be used for showing the game without UI, it can easily causes glitch otherwise
	// you can use the BindNormalMode command to fix glitch
	UFUNCTION(Exec)
	void RemoveAllWidgets() const;

#pragma endregion
};
