// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UI/Gameplay/Tchat/Tchat.h"
#include "Player/TransitionPlayerController.h"
#include "TransitionGamemode.generated.h"

UCLASS()
class GLITCHUE_API ATransitionGamemode : public AGameModeBase{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	ATransitionPlayerController* TransitionPlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "Tchat")
	UTchat* TchatWidget;
};
