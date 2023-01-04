// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlacableObject/PlacableObject.h"
#include "Components/InteractableComponent.h"
#include "GameFramework/Character.h"
#include "MainPlayerController.h"
#include "Mark/Mark.h"
#include "Components/TimelineComponent.h"
#include "MainPlayer.generated.h"

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

	virtual void BeginPlay() override;

	#pragma region Movement

public:
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

protected:
	#pragma endregion

	UPROPERTY(BlueprintReadWrite, Category = "Placable")
	UPlacableObject* PlacableActor;

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

	UPROPERTY(BlueprintReadWrite, Category = "Construction")
	int Golds = 0;

	UFUNCTION(BlueprintCallable, Exec, Category = "Construction")
	void GiveGolds(int Amount);

	#pragma region Interaction

	UPROPERTY(BlueprintReadWrite, Category = "Interaction")
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
	UTimelineComponent* CameraTransitionTL;

	//pourquoi "class" 
	UPROPERTY(BlueprintReadWrite)
	class AMark* Mark;

public:

	UFUNCTION(BlueprintCallable, Exec, Category = "Mark")
	void LaunchMark();

	FQuat FindMarkLaunchRotation();

	UFUNCTION(BlueprintCallable, Exec, Category = "Mark")
	void TPToMark();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Exec, Category = "Mark")
	void UseGlitchPressed();
	void UseGlitchPressed_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Exec, Category = "Mark")
	void UseGlitchReleassed();
	void UseGlitchReleassed_Implementation();

	void SetMark(AMark* NewMark);

	FVector CurrentCameraPosition;

	FRotator CurrentControlRotation;

	FRotator TargetControlRotation;

	UFUNCTION()
	void LookAtMark(float Value);

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
	// je pige pas
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Mark")
	FORCEINLINE class AMark* GetMark() const { return Mark; }
};

