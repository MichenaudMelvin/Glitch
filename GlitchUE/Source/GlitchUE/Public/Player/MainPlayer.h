// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlacableObject/PreviewPlacableActor.h"
#include "Components/InteractableComponent.h"
#include "Components/HealthComponent.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "MainPlayer.generated.h"

class AMainPlayerController;
class AMark;
class AMainAICharacter;

UENUM(BlueprintType)
enum class EPlayerMovementMode : uint8{
	Normal,
	Sneaking,
	Sprinting,
};

UCLASS(config=Game)
class AMainPlayer : public ACharacter{
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

	virtual void Tick(float deltaTime) override;
	virtual void BeginPlay() override;

	#pragma region Camera

public:
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

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FVector AimOffset = FVector(75, 75, 60);

	FTimeline CameraAimTransition;
	
	ETimelineDirection::Type CameraAimTimelineDirection;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Camera")
	ETimelineDirection::Type GetCameraAimDirection();

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

protected:
	EPlayerMovementMode MovementMode;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	EPlayerMovementMode GetMovementMode();

	UFUNCTION(BlueprintCallable)
	void SetMovementMode(EPlayerMovementMode NewMovementMode);

	/** Called for forwards/backward input */
	UFUNCTION(BlueprintCallable)
	void MoveForward(float Value);

	UFUNCTION(BlueprintCallable)
	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	UFUNCTION(BlueprintCallable)
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	UFUNCTION(BlueprintCallable)
	void LookUpAtRate(float Rate);

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

	UPROPERTY(BlueprintReadWrite, Category = "Placable")
	APreviewPlacableActor* PreviewPlacableActor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Health")
	UHealthComponent* HealthComp;

	FVector PlacableActorLocation;

	AMainPlayerController* MainPlayerController;

public:
	AMainPlayerController* GetMainPlayerController();

protected:
	UFUNCTION(BlueprintCallable)
	void PlaceObject();

	UFUNCTION(BlueprintCallable)
	void PreviewObject();

	UPROPERTY(BlueprintReadWrite)
	bool bInvertYAxis;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Construction")
	int Golds = 0;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Exec, Category = "Construction")
	void GiveGolds(int Amount);
	virtual void GiveGolds_Implementation(int Amount);

protected:
	#pragma region Interaction

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
	float InteractionLength = 1000.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Interaction")
	UInteractableComponent* CurrentCheckedObject;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interaction")
	bool InteractionLineTrace(FHitResult& outHit);

public:
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void InteractionTick();

	UFUNCTION(BlueprintCallable, Exec, Category = "Interaction")
	void Interact();

protected:
	UFUNCTION(Category = "Interaction")
	void UnfeedbackCurrentCheckedObject();

	#pragma endregion

#pragma region Mark

protected:
	FTimeline CameraTransitionTL;

	UPROPERTY(BlueprintReadWrite)
	AMark* Mark;

public:

	UFUNCTION(BlueprintCallable, Exec, Category = "Mark")
	void LaunchMark();

	FQuat FindMarkLaunchRotation();

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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Mark")
	float GlitchDashValue;

	void SetMark(AMark* NewMark);

	FVector CurrentCameraPosition;

	FRotator CurrentControlRotation;

	FRotator TargetControlRotation;

	UFUNCTION()
	void LookAtMark(float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Exec, Category = "Mark")
	void StartGlitchDashFX();
	void StartGlitchDashFX_Implementation();

	void GlitchCameraTrace();

	TSet<UStaticMeshComponent*> OverlappedMeshes;

	TArray<ECollisionResponse> OverlappedMeshesCollisionResponse;

	TSet<AMainAICharacter*> OverlappedAICharacters;

	void GlitchTrace();

	void ResetOverlappedMeshes();

	public:
		UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void GlitchUpgrade();
		virtual void GlitchUpgrade_Implementation();

		UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void ResetGlitchUpgrade();
		virtual void ResetGlitchUpgrade_Implementation();

#pragma endregion

#pragma region Others

	UCurveFloat* ZeroToOneCurve;

#pragma endregion

public:
	UFUNCTION(Exec)
	void TestFunction();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

