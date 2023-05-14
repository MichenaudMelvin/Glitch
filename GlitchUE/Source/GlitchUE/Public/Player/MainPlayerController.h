// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MainPlayer.h"
#include "CoreMinimal.h"
#include "AbstractPlayerController.h"
#include "GlitchUEGameMode.h"
#include "UI/TimerWidget.h"
#include "UI/PlacableSelection/HotBar.h"
#include "UI/PlacableSelection/Wheel.h"
#include "UI/Player/PlayerStats.h"
#include "UI/Player/SightWidget.h"
#include "UI/Tchat/Tchat.h"
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

#pragma endregion

UENUM(BlueprintType)
enum class EGameplayMode : uint8 {

	Normal,

	Construction,

	Destruction,
};

UCLASS(Abstract)
class GLITCHUE_API AMainPlayerController : public AAbstractPlayerController{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CreatePlayerWidgets();
	virtual void CreatePlayerWidgets_Implementation();

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

	UFUNCTION(BlueprintCallable, Exec, Category = "Saves")
	void BindFastSaveAndLoad();

	UFUNCTION(BlueprintCallable, Exec, Category = "Saves")
	void UnbindFastSaveAndLoad();

	UFUNCTION()
	void FastSave();

	UFUNCTION()
	void FastLoad();

	UPROPERTY(BlueprintReadOnly, Category = "Saves")
	bool bCanSave = true;

	void SetCanSave(const bool bValue);

	#pragma endregion

	#pragma region UI

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|UI")
	FKOnOpenSelectionWheelPressed OnOpenSelectionWheelPressed;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|UI")
	FKOnOpenSelectionWheelReleased OnOpenSelectionWheelReleased;

	#pragma endregion

#pragma endregion

#pragma region DelegatesFunctions

	#pragma region Actions

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	virtual void BindMovement();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void UnbindMovement();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	virtual void BindCamera();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void UnbindCamera();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	virtual void BindJump();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void UnbindJump();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	virtual void BindSneak();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void UnbindSneak();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	virtual void BindSprint();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Exec, Category = "Delegates")
	void BindConstruction();
	void BindConstruction_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Exec, Category = "Delegates")
	void UnbindConstruction();
	void UnbindConstruction_Implementation();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void UnbindSprint();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	virtual void BindGlitch();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void UnbindGlitch();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	virtual void BindInteraction();

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

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	virtual void BindMouseScroll();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void UnbindMouseScroll();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	virtual void BindOpenSelectionWheel();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void UnbindOpenSelectionWheel();

	/**
	 * @brief this one should call by an external element and should not be called in the player controller. To unbind this movement use select GameplayMode();
	 */
	UFUNCTION(BlueprintCallable, Category = "Cling")
	void BindClingMovement();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void UnbindAll();

	#pragma endregion

#pragma endregion

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Exec, Category = "Pause")
	void PauseGame();
	virtual void PauseGame_Implementation();

#pragma region Widgets

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "UI")
	USightWidget* SightWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<USightWidget> SightWidgetClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "UI")
	UTchat* Tchat;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UTchat> TchatWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UTimerWidget* TimerWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UTimerWidget> TimerWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UPlayerStats* PlayerStatsWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UPlayerStats> PlayerStatsWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWheel* WheelWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UWheel> WheelWidgetWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets|Wheel")
	float WheelTimeDilation = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets|Wheel")
	float OpenWheelTimer = 0.05f;

	FTimerHandle OpenWheelTimerHandle;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UHotBar* HotBarWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UHotBar> HotBarWidgetWidgetClass;

	UFUNCTION()
	void StartOpenWheelTimer();

	UFUNCTION()
	void OpenWheel();

	UFUNCTION()
	void CloseWheel();

	UFUNCTION()
	void ShowHotBar(float AxisValue);

	FTimerHandle HotBarTimerHandle;

public:
	UTchat* GetTchatWidget() const;

	USightWidget* GetSightWidget() const;

	UTimerWidget* GetTimerWidget() const;

	UPlayerStats* GetPlayerStatsWidget() const;

#pragma endregion
};
