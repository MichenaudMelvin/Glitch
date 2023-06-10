// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MainPlayer.h"
#include "CoreMinimal.h"
#include "AbstractPlayerController.h"
#include "UI/Gameplay/AdditionalMessage.h"
#include "UI/Gameplay/TimerWidget.h"
#include "UI/Menu/PauseMenu.h"
#include "UI/Gameplay/PlacableSelection/Wheel.h"
#include "UI/Gameplay/PlayerStats.h"
#include "UI/Gameplay/PopUpWidget.h"
#include "UI/Gameplay/SightWidget.h"
#include "UI/Gameplay/WaypointIndication.h"
#include "UI/Gameplay/ConditionScreen/LooseScreen.h"
#include "UI/Gameplay/ConditionScreen/WinScreen.h"
#include "UI/Gameplay/Tchat/Tchat.h"
#include "MainPlayerController.generated.h"

#pragma region Delegates

#pragma region Movement

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKOnMoveForward, float, AxisValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKOnMoveRight, float, AxisValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnJumpPressed);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnJumpReleased);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnSneakPressed);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnSneakReleased);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnSprintPressed);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnSprintReleased);

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

#pragma endregion

DECLARE_DELEGATE_OneParam(FInputSwitchInventoryDelegate, const int32);

UCLASS(Abstract)
class GLITCHUE_API AMainPlayerController : public AAbstractPlayerController{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	void CreatePlayerWidgets();

	UPROPERTY(BlueprintReadOnly)
	AGlitchUEGameMode* GameMode;

	UPROPERTY(BlueprintReadOnly)
	AMainPlayer* MainPlayer;

	FTimerDynamicDelegate InteractionTickDelegate;

public:

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
	FKOnSprintPressed OnSprintPressed;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates|Movement")
	FKOnSprintReleased OnSprintReleased;

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

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Saves")
	bool bCanSave = true;

public:
	void SetCanSave(const bool bValue);

	bool CanSave() const;

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
	void BindPause();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void UnbindPause();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	virtual void BindMouseScroll();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void UnbindMouseScroll();

	/**
	 * @brief this one should call by an external element and should not be called in the player controller. To unbind this movement use select GameplayMode();
	 */
	UFUNCTION(BlueprintCallable, Category = "Cling")
	void BindClingMovement();

	UFUNCTION(BlueprintCallable, Exec, Category = "Delegates")
	void UnbindAll(bool bUnbindPause = false);

	#pragma endregion

#pragma endregion

	UFUNCTION(BlueprintCallable, Exec, Category = "Pause")
	void PauseGame();

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

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UPauseMenu* PauseWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UPauseMenu> PauseWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UAdditionalMessage* AdditionalMessageWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UAdditionalMessage> AdditionalMessageWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UPopUpWidget* PopUpWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UPopUpWidget> PopUpWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float CloseWheelBlend = 0.5f;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWaypointIndication* WaypointIndicationWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UWaypointIndication> WaypointIndicationWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	ULooseScreen* LooseScreenWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<ULooseScreen> LooseScreenWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWinScreen* WinScreenWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UWinScreen> WinScreenWidgetClass;

public:
	UFUNCTION()
	void OpenWheel();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	void CloseWheel();

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	bool IsWheelOpened() const;

	void CameraBlend(AActor* BlendTarget, const float BlendTime);

	UTchat* GetTchatWidget() const;

	USightWidget* GetSightWidget() const;

	UWheel* GetWheelWidget() const;

	float GetWheelTimeDilation() const;

	UTimerWidget* GetTimerWidget() const;

	UPlayerStats* GetPlayerStatsWidget() const;

	UAdditionalMessage* GetAdditionalMessageWidget() const;

	UWaypointIndication* GetWaypointIndicationWidget() const;

	ULooseScreen* GetLooseScreen() const;

	UWinScreen* GetWinScreen() const;

#pragma endregion
};
