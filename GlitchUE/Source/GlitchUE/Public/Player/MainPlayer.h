// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/HearingTriggerComponent.h"
#include "PlacableObject/PreviewPlacableActor.h"
#include "Components/InteractableComponent.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "Helpers/UsefulEnums.h"
#include "Player/TargetCameraLocation.h"
#include "Saves/Settings/GameplaySettingsSave.h"
#include "MainPlayer.generated.h"

class AMainPlayerController;
class AGlitchMark;
class AMainAICharacter;
class APursuitDrone;
class UCompassComponent;

UENUM(BlueprintType)
enum class EPlayerMovementMode : uint8{
	Normal,
	Sneaking,
	Sprinting,
};

UENUM(BlueprintType)
enum class EGoldsUpdateMethod : uint8{
	BuyPlacable,
	TakeDamages,
	ReceiveGolds,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnEndAppear);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKOnUpdateGolds, int, Golds);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKOnInitPlayer);

UCLASS(config=Game)
class AMainPlayer : public ACharacter, public IGlitchInterface{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AMainPlayer();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Destroyed() override;

	UFUNCTION()
	void OnCleanWorld(UWorld* World, bool bSessionEnded, bool bCleanupResources);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	UPopcornFXEmitterComponent* SoundFX;

	UPROPERTY(EditDefaultsOnly, Category = "Compass")
	UCompassComponent* Compass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hearing")
	UHearingTriggerComponent* HearingTrigger;

	float RunFXLifeTime;

	float RunFXLifeTimeDeviation;

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Saves")
	void InitializePlayer(const FTransform StartTransform, const FRotator CameraRotation, const FTransform MarkTransform, const bool bIsMarkPlaced);
	void InitializePlayer_Implementation(const FTransform StartTransform, const FRotator CameraRotation, const FTransform MarkTransform, const bool bIsMarkPlaced);

	#pragma region Camera

	UFUNCTION(BlueprintCallable, Exec, Category = "Camera")
	void CameraAim();

	UFUNCTION(BlueprintCallable, Exec, Category = "Camera")
	void CameraAimReverse();

protected:
	UFUNCTION(BlueprintCallable, Exec, Category = "Camera")
	void CameraStopAim();

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void CameraAimUpdate(float Alpha);

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void CameraAimFinished();

	UPROPERTY(EditDefaultsOnly, Category = "Aim")
	FVector AimOffset = FVector(75, 75, 60);

	FTimeline CameraAimTransition;

	UPROPERTY(EditDefaultsOnly, Category = "Aim")
	bool bUseSmoothAim = true;

	UPROPERTY(EditDefaultsOnly, Category = "Aim")
	float CameraAimTime = 0.5f;

	ETimelineDirection::Type CameraAimTimelineDirection;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera")
	ETimelineDirection::Type GetCameraAimDirection() const;

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void CameraZoom(float TargetZoom);

	float TargetZoomValue;

	float CurrentZoomValue;

	FTimeline CameraZoomTransition;

	UFUNCTION()
	void CameraZoomUpdate(float Alpha);

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void CameraFOV(float TargetFOV);

	float TargetFOVValue;

	float CurrentFOVValue;

	FTimeline CameraFOVTransition;

	UFUNCTION()
	void CameraFOVUpdate(float Alpha);

	#pragma endregion

	#pragma region Movement

	EPlayerMovementMode MovementMode;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement|Speed")
	float NormalSpeed = 550;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement|Speed")
	float SneakSpeed = 200;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement|Speed")
	float DashSpeed = 1250;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement|Speed")
	float SprintSpeed = 1000;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement|Speed")
	float SlideSpeed = 1000;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement|Gravity")
	float OriginalGravityScale = 2;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement")
	float OriginalGroundFriction = 10;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement")
	float OriginalBrakingDecelerationWalking = 2048;

	UPROPERTY(EditDefaultsOnly, Category = "Movement|Noise")
	float NormalSpeedNoiseRange = 500;

	UPROPERTY(EditDefaultsOnly, Category = "Movement|Noise")
	float SprintSpeedNoiseRange = 1000;

	UPROPERTY(EditDefaultsOnly, Category = "Movement|Noise")
	float CrouchSpeedNoiseRange = 50;

	UPROPERTY(EditDefaultsOnly, Category = "Movement|Noise")
	float GlitchDashNoiseRange = 750;

	/**
	 * @brief OnLandedEvent: FallingVelocity / JumpNoiseRangeFactor
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Movement|Noise")
	float JumpNoiseRangeFactor = 2;

	void MakeMovementNoise(const float InputRate);

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	EPlayerMovementMode GetMovementMode() const;

	UFUNCTION(BlueprintCallable)
	void SetMovementMode(const EPlayerMovementMode NewMovementMode);

	/** Called for forwards/backward input */
	UFUNCTION(BlueprintCallable)
	void MoveForward(const float Value);

	UFUNCTION(BlueprintCallable)
	/** Called for side to side input */
	void MoveRight(const float Value);

	UFUNCTION()
	void ClingUpDirection(float AxisValue);

	UFUNCTION()
	void ClingRight(float AxisValue);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movevement|Cling")
	void HorizontalCling(const EDirection Direction);
	virtual void HorizontalCling_Implementation(const EDirection Direction);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movevement|Cling")
	void ClingUp();
	virtual void ClingUp_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movevement|Cling")
	void DetachFromEdge();
	virtual void DetachFromEdge_Implementation();

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	UFUNCTION(BlueprintCallable)
	void TurnAtRate(const float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	UFUNCTION(BlueprintCallable)
	void LookUpAtRate(const float Rate);

	virtual void Jump() override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Movement|Cling")
	bool bCanBeAttachedToEdge = true;

	bool bUseCoyoteTime = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement")
	float CoyoteTime = 0.15f;

	FTimerHandle CoyoteTimeTimerHandle;

public:
	virtual void OnWalkingOffLedge_Implementation(const FVector& PreviousFloorImpactNormal, const FVector& PreviousFloorContactNormal, const FVector& PreviousLocation, float TimeDelta) override;

	virtual void AddControllerYawInput(float Val) override;

	virtual void AddControllerPitchInput(float Rate) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Exec, Category = "Movement")
	void Dash();
	void Dash_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Exec, Category = "Movement")
	void SprintRelease();
	void SprintRelease_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Exec, Category = "Movement")
	void SneakPressed();
	void SneakPressed_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Exec, Category = "Movement")
	void SneakReleased();
	void SneakReleased_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Exec, Category = "Movement")
	void SprintToSneak();
	void SprintToSneak_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Exec, Category = "Movement")
	void SneakToSprint();
	void SneakToSprint_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Exec, Category = "Movement")
	void ResetMovement();
	void ResetMovement_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Movement")
	bool CanStandUp();

	virtual void Landed(const FHitResult& Hit) override;

	UFUNCTION()
	void OnSwitchPhases(EPhases NewPhase);

protected:
	#pragma endregion

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float HalfHeight;

	UPROPERTY(BlueprintReadOnly)
	ANexus* Nexus;

	UPROPERTY(BlueprintReadWrite, Category = "Placable")
	APreviewPlacableActor* PreviewPlacableActor;

	UPROPERTY(BlueprintReadOnly)
	bool bIsInGlitchZone = false;

	FVector PlacableActorLocation;

	AMainPlayerController* MainPlayerController;

	AGlitchUEGameMode* GameMode;

public:
	AMainPlayerController* GetMainPlayerController() const;

	bool IsInGlitchZone() const;

	void SetInGlitchZone(const bool bNewValue);

protected:
	bool bInvertYAxis;

	float Sensitivity = 1;

public:
	void SetInvertAxis(const bool bNewValue);

	void SetSensitivity(const float NewSensitivity);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Golds")
	UPopcornFXEmitterComponent* LoseGoldsFX;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Construction")
	int Golds = 1000;

	UPROPERTY(BlueprintReadWrite)
	bool bGoldsCanBeUpdated = true;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Construction")
	UPlacableActorData* CurrentPlacableActorData;

	UPROPERTY(BlueprintReadWrite, Category = "Construction")
	FVector TargetPreviewActorLocation;

	UPROPERTY(BlueprintReadWrite, Category = "Construction")
	AConstructionZone* CurrentFocusedConstructionZone;

public:
	UFUNCTION(BlueprintCallable, Category = "Construction")
	void PlacePlacableActor();

	AConstructionZone* GetCurrentConstructionZone() const;

	void OpenConstructionZone(AConstructionZone* CurrentConstructionZone);

	APreviewPlacableActor* GetPreviewPlacableActor() const;

	void SetPlacableActorData(UPlacableActorData* Data);

	UPlacableActorData* GetCurrentPlacableActorData() const;

	int GetGolds() const;

	UFUNCTION(BlueprintCallable, Exec, Category = "Golds")
	void UpdateGolds(int Amount = 0, const EGoldsUpdateMethod GoldsUpdateMethod = EGoldsUpdateMethod::ReceiveGolds);

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Golds")
	FKOnUpdateGolds OnUpdateGolds;

	UFUNCTION(BlueprintCallable, Exec, Category = "Golds")
	void SetGolds(const int Amount = 1000);

	UFUNCTION(BlueprintCallable, Exec, Category = "Golds")
	void KillPlayer();

	bool CanUpdateGolds() const;

	UFUNCTION(BlueprintCallable, Exec, Category = "Loose")
	void Loose();

	UPROPERTY(EditDefaultsOnly, Category = "Glitch")
	int RemovedGlitchGolds = 100;

protected:
	#pragma region Interaction

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Interaction")
	float InteractionLength = 1000.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Interaction")
	UInteractableComponent* CurrentCheckedObject;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interaction")
	bool InteractionLineTrace(FHitResult& OutHit) const;

	APursuitDrone* CurrentDrone;

public:
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void InteractionTick();

	UFUNCTION(BlueprintCallable, Exec, Category = "Interaction")
	void Interact();

	void SetCurrentDrone(APursuitDrone* NewDrone);

	APursuitDrone* GetCurrentDrone() const;

	void DropDrone(APursuitDrone* NewDrone);

protected:
	UFUNCTION(Category = "Interaction")
	void UnfeedbackCurrentCheckedObject();

	#pragma endregion

#pragma region Mark

	FTimeline CameraTransitionTL;

	UPROPERTY(EditDefaultsOnly, Category = "Glitch Dash")
	float GlitchDashCameraTransition = 1;

	UPROPERTY()
	AGlitchMark* Mark;

	UPROPERTY()
	ATargetCameraLocation* WheelCamera;

public:
	ATargetCameraLocation* GetWheelCamera() const;

	UFUNCTION(BlueprintCallable, Exec, Category = "Mark")
	void LaunchMark();

	UPROPERTY()
	UFMODEvent* TPStart;

	UPROPERTY()
	UFMODEvent* TPEnd;

	FRotator FindMarkLaunchRotation() const;

	UFUNCTION(BlueprintCallable, Exec, Category = "Mark")
	void TPToMark();

	UFUNCTION()
	void EndTL();

	UFUNCTION(BlueprintCallable, Exec, Category = "Mark")
	void UseGlitchPressed();

	UFUNCTION()
	void CanLaunchMark();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Exec, Category = "Movement")
	void ForceStopSneak();
	virtual void ForceStopSneak_Implementation();

	UPROPERTY(BlueprintReadWrite, Category = "Mark")
	FTimerHandle CanLaunchMarkTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Aim")
	float BackToNormalCamAfterLaunchMarkTime = 0.5f;

	FTimerHandle BackToNormalCamTimerHandle;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Mark")
	AGlitchMark* GetMark() const { return Mark; }

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Mark")
	float GlitchDashValue = 400;

	void SetMark(AGlitchMark* NewMark);

	FVector CurrentCameraPosition;

	FRotator CurrentControlRotation;

	FRotator TargetControlRotation;

	UFUNCTION(BlueprintCallable)
	void LookAtTarget(const FRotator TargetRotation, const FOnTimelineEvent FinishedEvent, const float Duration = 1);

	UFUNCTION(BlueprintCallable)
	void StopLookAtTimeline();

	UFUNCTION()
	void LookAtTargetUpdate(float Value);

	UFUNCTION(BlueprintCallable, Exec, Category = "Mark")
	void StartGlitchDashFX() const;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Mark")
	float GlitchDashDuration = 0.15f;

	FTimerHandle GlitchDashTimerHandle;

	UPopcornFXEffect* GlitchDashFXReference;

	UPROPERTY()
	UPopcornFXEmitterComponent* GlitchDashFX;

	UPROPERTY()
	UPopcornFXEmitterComponent* GlitchDashFXBackup;

	void GlitchCameraTrace();

	TSet<UStaticMeshComponent*> OverlappedMeshes;

	TArray<ECollisionResponse> OverlappedMeshesCollisionResponse;

	TSet<AMainAICharacter*> OverlappedAICharacters;

	void GlitchTrace();

	void ResetOverlappedMeshes();

	UFUNCTION(BlueprintNativeEvent)
	void FinishGlitchDash();
	virtual void FinishGlitchDash_Implementation();

	UPROPERTY(EditDefaultsOnly)
	float GlitchUpgradeDuration = 0.5;

	FTimerHandle GlitchUpgradeTimerHandle;

	virtual void ReceiveGlitchUpgrade() override;

	virtual void ResetGlitchUpgrade() override;

	TArray<FTransform> GlitchRewindTransformList;

	FTimerHandle RewindTimer;

	void StartRecord();

	void StopRecord();

	void SelectRandomLocation();

	void RecordRandomLocation();

	UPROPERTY(EditDefaultsOnly)
	float RewindSpacesSave = 1;

	UPROPERTY(EditDefaultsOnly)
	float MaxRewindList = 50;

	UPROPERTY(EditDefaultsOnly, Category = "Post Process UI")
	UMaterialParameterCollection* GlitchMPC;

	UPROPERTY(EditDefaultsOnly, Category = "Post Process UI")
	FWeightedBlendable GlitchPostProcessMaterialUI;

	UPROPERTY(EditDefaultsOnly, Category = "Post Process UI")
	FWeightedBlendable SafePostProcessMaterialUI;

	void EnableGlitchEffect(const bool bEnable, const float EffectDuration, const float GlitchValue = 1);

	void EnableSafeEffect(const bool bEnable, const float EffectDuration, const float SafeValue = 1);

	float TargetPostProcessUIValue;

protected:
	void EnablePostProcessUIEffect(const bool bEnable, FWeightedBlendable PostProcessEffect, const float EffectDuration, const float PostProcessValue);

	FTimeline FadeInGlitchEffectTimeline;

	UFUNCTION()
	void FadeInGlitchEffect(float Value);

	UFUNCTION()
	void EndFadeIn();

	/**
	 * @brief Player start with appear materials
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Appear")
	TArray<UMaterialInterface*> RealPlayerMaterialList;

	UPROPERTY(EditDefaultsOnly, Category = "Appear")
	float AppearTime = 0.25f;

	FTimeline AppearTimeline;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Appear")
	FKOnEndAppear OnEndAppear;

#if WITH_EDITORONLY_DATA
	/**
	 * @brief Even if the value is false it will play in a package game
	 */
	UPROPERTY(EditDefaultsOnly, Transient, Category = "Appear")
	bool bPlayStartAnim = false;
#endif

	void MakeThePlayerAppear();

	UFUNCTION()
	void AppearUpdate(float Value);

	UFUNCTION()
	void EndAppear();

	bool bIsAppearing = false;

public:
	bool IsAppearing() const;

	void UpdateGlitchGaugeFeedback(const float GlitchValue, const float GlitchMaxValue) const;

	void SetGlitchMaterialParameter(const int MaterialIndex, const float Value) const;

#pragma endregion

	#pragma region Others

	UCurveFloat* ZeroToOneCurve;

	UCurveFloat* SmoothZeroToOneCurve;

	#pragma endregion

	#pragma region Saves

	UPROPERTY(BlueprintReadWrite)
	UGameplaySettingsSave* GameplaySettingsSaveSave;

	#pragma endregion

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

