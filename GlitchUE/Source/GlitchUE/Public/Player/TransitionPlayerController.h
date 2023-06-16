// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/AbstractPlayerController.h"
#include "UI/Gameplay/Tchat/Tchat.h"
#include "TransitionPlayerController.generated.h"

class ATransitionGamemode;

UCLASS(Abstract)
class GLITCHUE_API ATransitionPlayerController : public AAbstractPlayerController{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "Tchat")
	UTchat* TchatWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Tchat")
	TSubclassOf<UTchat> TchatWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "Gamemode")
	ATransitionGamemode* Gamemode;

public:
	UTchat* GetTchatWidget() const;
};
