// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MainPlayer.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

#pragma region Delegates

#pragma region Movement

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKOnMoveForward, float, AxisValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKOnMoveRight, float, AxisValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnJumpPressed);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnJumpReleased);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnSneakPressed);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnSneakReleased);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnSprint);

#pragma endregion

#pragma region Camera

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKOnTurn, float, AxisValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKOnTurnRate, float, AxisValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKOnLookUp, float, AxisValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKOnLookUpRate, float, AxisValue);

#pragma endregion

#pragma region SpecialAbilities

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnInteractPlayer);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnPlaceObject);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnUseGlitchPressed);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnUseGlitchReleassed);

#pragma endregion

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnPause);

#pragma endregion

UCLASS()
class GLITCHUE_API AMainPlayerController : public APlayerController{
	GENERATED_BODY()


protected:
	AMainPlayer* MainPlayer;

#pragma region Deletages

	#pragma region Movement

public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|Movement")
	FKOnMoveForward OnMoveForward;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|Movement")
	FKOnMoveRight OnMoveRight;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|Movement")
	FKOnJumpPressed OnJumpPressed;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|Movement")
	FKOnJumpReleased OnJumpReleased;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|Movement")
	FKOnSneakPressed OnSneakPressed;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|Movement")
	FKOnSneakReleased OnSneakReleased;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|Movement")
	FKOnSprint OnSprint;

	#pragma endregion

	#pragma region Camera

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|Camera")
	FKOnTurn OnTurn;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|Camera")
	FKOnTurnRate OnTurnRate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|Camera")
	FKOnLookUp OnLookUp;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|Camera")
	FKOnLookUpRate OnLookUpRate;

	#pragma endregion

	#pragma region SpecialAbilities

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|SpecialAbilities")
	FKOnInteractPlayer OnInteractPlayer;

	FTimerHandle InteractionTimer;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|SpecialAbilities")
	FKOnPlaceObject OnPlaceObject;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|SpecialAbilities")
	FKOnUseGlitchPressed OnUseGlitchPressed;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|SpecialAbilities")
	FKOnUseGlitchReleassed OnUseGlitchReleassed;

	#pragma endregion

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|Pause")
	FKOnPause OnPause;

#pragma endregion

#pragma region DelegatesFunctions

	#pragma region Actions

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Delegates")
	void BindMovement();
	virtual void BindMovement_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Delegates")
	void UnbindMovement();
	virtual void UnbindMovement_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Delegates")
	void BindCamera();
	virtual void BindCamera_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Delegates")
	void UnbindCamera();
	virtual void UnbindCamera_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Delegates")
	void BindJump();
	virtual void BindJump_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Delegates")
	void UnbindJump();
	virtual void UnbindJump_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Delegates")
	void BindSneak();
	virtual void BindSneak_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Delegates")
	void UnbindSneak();
	virtual void UnbindSneak_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Delegates")
	void BindSprint();
	virtual void BindSprint_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Delegates")
	void UnbindSprint();
	virtual void UnbindSprint_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Delegates")
	void BindInteraction();
	virtual void BindInteraction_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Delegates")
	void UnbindInteraction();
	virtual void UnbindInteraction_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Delegates")
	void BindNormalMode();
	virtual void BindNormalMode_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Delegates")
	void BindConstructionMode();
	virtual void BindConstructionMode_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Delegates")
	void BindPause();
	virtual void BindPause_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Delegates")
	void UnbindPause();
	virtual void UnbindPause_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Delegates")
	void UnbindAll();
	virtual void UnbindAll_Implementation();

	#pragma endregion

#pragma endregion

};
