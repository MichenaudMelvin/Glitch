// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AbstractPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnInteractPlayer);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnPause);

UCLASS(Abstract)
class GLITCHUE_API AAbstractPlayerController : public APlayerController{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|SpecialAbilities")
	FKOnInteractPlayer OnInteractPlayer;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|Pause")
	FKOnPause OnPause;

	// the player controller function is not virtual
	/**
	 * @brief This function is only used to fix when the cursor appears on the left side of the screen.
	 * @param Show show the mouse cursor or not
	 */
	UFUNCTION(BlueprintCallable)
	void ShowMouseCursor(const bool Show = true);
};
