// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlacableObject/PreviewPlacableActor.h"
#include "Components/InteractableComponent.h"
#include "Components/HealthComponent.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "Saves/SettingsSave.h"
#include "MainPlayer.generated.h"

class AMainPlayerController;
class AMark;
class AMainAICharacter;
class APursuitDrone;

UENUM(BlueprintType)
enum class EPlayerMovementMode : uint8{
	Normal,
	Sneaking,
	Sprinting,
};

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

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UPopcornFXEmitterComponent* RunFX;

	float RunFXLifeTime;

	float RunFXLifeTimeDeviation;

public:
	void InitializePlayer(const FTransform StartTransform, const FRotator CameraRotation, const FTransform MarkTransform, const bool bIsMarkPlaced);

	#pragma region Camera

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Exec, Category = "Camera")
	void CameraAim();
	virtual void CameraAim_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Exec, Category = "Camera")
	void CameraAimReverse();
	virtual void CameraAimReverse_Implementation();

protected:
	UFUNCTION(BlueprintCallable, Exec, Category = "Camera")
	void CameraStopAim();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera")
	void CameraAimUpdate(float Alpha);
	virtual void CameraAimUpdate_Implementation(float Alpha);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera")
	void CameraAimFinished();
	virtual void CameraAimFinished_Implementation();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FVector AimOffset = FVector(75, 75, 60);

	FTimeline CameraAimTransition;
	
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

	UPROPERTY(EditDefaultsOnly, Category = "Movement|Speed")
	float GlitchSpeed = 1500;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement|Gravity")
	float OriginalGravityScale = 2;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement")
	float OriginalGroundFriction = 10;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement")
	float OriginalBrakingDecelerationWalking = 2048;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement|Noise")
	float NormalSpeedNoiseRange = 500;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement|Noise")
	float SprintSpeedNoiseRange = 1000;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement|Noise")
	float CrouchSpeedNoiseRange = 50;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement|Noise")
	float GlitchDashNoiseRange = 750;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement|Noise")
	float JumpNoiseRange = 500;

	void MakeMovementNoise();

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

	bool bUseCoyoteTime = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement")
	float CoyoteTime = 0.15f;

	virtual void OnWalkingOffLedge_Implementation(const FVector& PreviousFloorImpactNormal, const FVector& PreviousFloorContactNormal, const FVector& PreviousLocation, float TimeDelta) override;

	virtual void AddControllerYawInput(float Val) override;

	virtual void AddControllerPitchInput(float Rate) override;

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
	void ResetMovement();
	void ResetMovement_Implementation();

protected:
	#pragma endregion

	UPROPERTY(BlueprintReadOnly)
	ANexus* Nexus;

	UPROPERTY(BlueprintReadWrite, Category = "Placable")
	APreviewPlacableActor* PreviewPlacableActor;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	UHealthComponent* HealthComp;

	UPROPERTY(BlueprintReadOnly)
	bool bIsInGlitchZone = false;

	FVector PlacableActorLocation;

	AMainPlayerController* MainPlayerController;

public:
	AMainPlayerController* GetMainPlayerController() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UHealthComponent* GetHealthComp() const;

	bool IsInGlitchZone() const;

	void SetInGlitchZone(const bool bNewValue);

protected:
	UFUNCTION(BlueprintCallable)
	void PlaceObject();

	UFUNCTION(BlueprintCallable)
	void PreviewObject();

	UPROPERTY(BlueprintReadWrite)
	bool bInvertYAxis;

	float Sensibility = 1;

public:
	void SetInvertAxis(const bool bNewValue);

	void SetSensibility(const float NewSensibility);

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Construction")
	int Golds = 0;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Construction")
	UPlacableActorData* CurrentPlacableActorData;

public:
	void SetPlacableActorData(UPlacableActorData* Data);

	UPlacableActorData* GetCurrentPlacableActorData() const;

	int GetGolds() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Exec, Category = "Construction")
	void GiveGolds(const int Amount);
	virtual void GiveGolds_Implementation(const int Amount);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Exec, Category = "Loose")
	void Loose();

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

	UPROPERTY(BlueprintReadWrite)
	AMark* Mark;

public:

	UFUNCTION(BlueprintCallable, Exec, Category = "Mark")
	void LaunchMark();

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* TPStart;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* TPFinal;

	FQuat FindMarkLaunchRotation() const;

	UFUNCTION(BlueprintCallable, Exec, Category = "Mark")
	void TPToMark();

	UFUNCTION()
	void EndTL();

	UFUNCTION(BlueprintCallable, Exec, Category = "Mark")
	void UseGlitchPressed();

	UFUNCTION(BlueprintCallable, Exec, Category = "Mark")
	void UseGlitchReleassed();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Mark")
	AMark* GetMark() const { return Mark; }

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Mark")
	float GlitchDashValue;

	void SetMark(AMark* NewMark);

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

	UPopcornFXEffect* GlichDashFXReference;

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

	UPROPERTY(EditDefaultsOnly)
	float GlitchUpgradeDuration = 0.5;

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

	UPROPERTY(EditDefaultsOnly, Category = "GlitchUI")
	UMaterialParameterCollection* GlitchMPC;

	UPROPERTY(EditDefaultsOnly, Category = "GlitchUI")
	FWeightedBlendable PostProcessMaterialUI;

	void EnableGlitchEffect(const bool bEnable, const float EffectDuration, const float GlitchValue = 1);

	float TargetGlitchUIValue;

protected:
	FTimeline FadeInGlitchEffectTimeline;

	UFUNCTION()
	void FadeInGlitchEffect(float Value);

	UFUNCTION()
	void EndFadeIn();

	UPROPERTY(EditDefaultsOnly)
	TArray<UMaterialInterface*> GlitchedMaterialList;

public:
	void UpdateGlitchGaugeFeedback(const float GlitchValue, const float GlitchMaxValue);

	void SetGlitchMaterialParameter(const int MaterialIndex, const float Value) const;

#pragma endregion

	#pragma region Others

	UCurveFloat* ZeroToOneCurve;

	#pragma endregion

	#pragma region Saves

	UPROPERTY(BlueprintReadWrite)
	USettingsSave* SettingsSave;

	#pragma endregion

	UFUNCTION(Exec)
	void TestFunction();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

