// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MainPlayer.h"
#include "CoreMinimal.h"
#include "GlitchUEGameMode.h"
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKOnMouseScroll, float, AxisValue);

#pragma endregion

#pragma region SpecialAbilities

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnInteractPlayer);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnPlaceObject);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnUseGlitchPressed);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnUseGlitchReleased);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnFastSave);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnFastLoad);

#pragma endregion

#pragma region UI

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnOpenSelectionWheelPressed);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnOpenSelectionWheelReleased);

#pragma endregion

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnPause);

#pragma endregion

UENUM(BlueprintType)
enum class EGameplayMode : uint8 {

	Normal,

	Construction,

	Destruction,
};

UCLASS()
class GLITCHUE_API AMainPlayerController : public APlayerController{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	AGlitchUEGameMode* GameMode;

	UPROPERTY(BlueprintReadOnly)
	AMainPlayer* MainPlayer;

	FTimerDynamicDelegate InteractionTickDelegate;

	EGameplayMode GameplayMode;

public:
	UFUNCTION(BlueprintCallable, Exec, Category = "Gameplay")
	void SelectNewGameplayMode(const EGameplayMode NewGameplayMode);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gameplay")
	EGameplayMode GetGameplayMode() const;

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

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|Camera")
	FKOnMouseScroll OnMouseScroll;

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
	FKOnUseGlitchReleased OnUseGlitchReleased;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|SpecialAbilities")
	FKOnFastSave OnFastSave;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|SpecialAbilities")
	FKOnFastLoad OnFastLoad;

	#pragma endregion

	#pragma region UI

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|UI")
	FKOnOpenSelectionWheelPressed OnOpenSelectionWheelPressed;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|UI")
	FKOnOpenSelectionWheelReleased OnOpenSelectionWheelReleased;

	#pragma endregion

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|Pause")
	FKOnPause OnPause;

#pragma endregion

#pragma region DelegatesFunctions

	#pragma region Actions

public:
	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void BindMovement();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void UnbindMovement();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void BindCamera();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void UnbindCamera();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void BindJump();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void UnbindJump();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void BindSneak();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void UnbindSneak();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Exec, Category = "Delegates")
	void BindSprint();
	void BindSprint_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Exec, Category = "Delegates")
	void BindConstruction();
	void BindConstruction_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Exec, Category = "Delegates")
	void UnbindConstruction();
	void UnbindConstruction_Implementation();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void UnbindSprint();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void BindGlitch();
	
	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void UnbindGlitch();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void BindInteraction();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void UnbindInteraction();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void BindNormalMode();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void BindConstructionMode();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void BindPause();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void UnbindPause();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Exec, Category = "Delegates")
	void BindMouseScroll();
	virtual void BindMouseScroll_Implementation();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void UnbindMouseScroll();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Exec, Category = "Delegates")
	void BindOpenSelectionWheel();
	void BindOpenSelectionWheel_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Exec, Category = "Delegates")
	void UnbindOpenSelectionWheel();
	void UnbindOpenSelectionWheel_Implementation();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void UnbindAll();

	#pragma endregion

#pragma endregion

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Exec, Category = "Pause")
	void PauseGame();
	virtual void PauseGame_Implementation();
};
