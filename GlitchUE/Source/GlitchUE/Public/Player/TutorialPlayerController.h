// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/MainPlayerController.h"
#include "TutorialPlayerController.generated.h"

UCLASS(Abstract)
class GLITCHUE_API ATutorialPlayerController : public AMainPlayerController{
	GENERATED_BODY()

protected:
	virtual void BindMovement() override;

	virtual void BindCamera() override;

	virtual void BindJump() override;

	virtual void BindSneak() override;

	virtual void BindSprint() override;

	virtual void BindGlitch() override;

	virtual void BindInteraction() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Binds")
	bool bShouldBindMovement = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Binds")
	bool bShouldBindCamera = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Binds")
	bool bShouldBindJump = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Binds")
	bool bShouldBindSneak = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Binds")
	bool bShouldBindSprint = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Binds")
	bool bShouldBindGlitch = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Binds")
	bool bShouldBindInteraction = true;
};
