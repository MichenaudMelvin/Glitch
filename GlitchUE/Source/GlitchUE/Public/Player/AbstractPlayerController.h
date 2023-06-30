// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Saves/GameplaySave.h"
#include "AbstractPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnInteractPlayer);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKOnAnyKey, FKey, Key);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnPause);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnSwitchToKeyboard);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKOnMouseScroll, float, AxisValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnSwitchToGamepad);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKOnGamepadAxis, float, Axis);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKOnGamepadTriggers, float, Axis);

UCLASS(Abstract)
class GLITCHUE_API AAbstractPlayerController : public APlayerController{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void PressedAnyKey(FKey KeyMap);

	UPROPERTY()
	UGameplaySave* GameplaySave;

public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|SpecialAbilities")
	FKOnInteractPlayer OnInteractPlayer;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|Pause")
	FKOnPause OnPause;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|AnyKey")
	FKOnAnyKey OnAnyKey;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|Keyboard")
	FKOnSwitchToKeyboard OnSwitchToKeyboard;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|Keyboard")
	FKOnMouseScroll OnMouseScroll;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|Gamepad")
	FKOnSwitchToGamepad OnSwitchToGamepad;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|Gamepad")
	FKOnGamepadAxis OnGamepadAxis;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|Gamepad")
	FKOnGamepadTriggers OnGamepadTriggers;

	// the player controller function is not virtual
	/**
	 * @brief This function is only used to fix when the cursor appears on the left side of the screen.
	 * @param bShow show the mouse cursor or not
	 * @param WidgetToFocus if bShow is false it can be nullptr
	 */
	UFUNCTION(BlueprintCallable)
	void ShowMouseCursor(const bool bShow = false, UUserWidget* WidgetToFocus = nullptr);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gamepad")
	bool IsUsingGamepad();
};
