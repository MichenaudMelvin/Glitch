// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavModifierComponent.h"
#include "Components/InteractableComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "FMODAudioComponent.h"
#include "Navigation/NavLinkProxy.h"
#include "AbstractDoor.generated.h"

USTRUCT(BlueprintType)
struct FBasicDoorData{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bIsOpen = false;

	UPROPERTY()
	int RotationFactor = 0;
};

class AMainPlayer;

UCLASS(Abstract)
class GLITCHUE_API AAbstractDoor : public AActor{
	GENERATED_BODY()

public:
	AAbstractDoor();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnCleanWorld(UWorld* World, bool bSessionEnded, bool bCleanupResources);

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USceneComponent* RootComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* MedFrame;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* TechFrame;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* MedDoor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* TechDoor;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	UFMODAudioComponent* DoorAudioComp;

	UPROPERTY()
	UFMODEvent* OpenDoorSFX;

	UPROPERTY()
	UFMODEvent* CloseDoorSFX;

	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
	UChildActorComponent* NavLinkProxy;

	UPROPERTY(BlueprintReadOnly)
	ANavLinkProxy* NavLink;

	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
	UNavModifierComponent* NavModifier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	UInteractableComponent* DoorInteraction;

	FTimeline OpenDoorTimeline;

	UCurveFloat* ZeroToOneCurve;

	bool bIsOpen = false;

	UPROPERTY(EditDefaultsOnly, Category = "Door|Time")
	float OpenDoorTime = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Door|Noise")
	float OpenDoorNoiseRadius = 200;

	ACharacter* CurrentCharacterOpenningDoor;

	void OpenAndCloseDoor();

	virtual void OpenDoor();

	virtual void CloseDoor();

	UFUNCTION()
	virtual void OpenDoorUpdate(float Alpha);

	UFUNCTION()
	void OpenDoorFinished();

	FTimerHandle OpenDoorAITimerHandle;

	FTimerHandle ResetDoorAITimerHandle;

	// this function cannot be bind in C++, bind it in BeginPlay() in Blueprint
	UFUNCTION(BlueprintCallable)
	void OnAIReachLink(AActor* MovingActor, const FVector& DestinationPoint);

	void ResetReachLink();

	bool bIsLinkReached = false;

	UFUNCTION()
	void OnInteract(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer);

	UFUNCTION(CallInEditor, Category = "Door")
	virtual void OpenDoorEditor();

	UFUNCTION(CallInEditor, Category = "Door")
	virtual void CloseDoorEditor();

	void UpdateNavModiferParameters() const;
};
