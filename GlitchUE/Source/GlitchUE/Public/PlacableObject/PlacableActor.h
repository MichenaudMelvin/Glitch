// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gamemodes/GlitchInterface.h"
#include "GameFramework/Actor.h"
#include "PlacableActorData.h"
#include "Objectives/Nexus.h"
#include "Components/TimelineComponent.h"
#include "PopcornFXEmitter.h"
#include "Saves/TowerDefenseSave.h"
#include "PlacableActor.generated.h"

class AMainPlayerController;
class AMainPlayer;
class UInteractableComponent;
class AConstructionZone;
class AMainAICharacter;
class APursuitDrone;

USTRUCT(BlueprintType)
struct FPlacableActorCreation{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<APlacableActor> Class;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UPlacableActorData* Data = NewObject<UPlacableActorData>();
};

UCLASS(Abstract)
class GLITCHUE_API APlacableActor : public AActor, public IGlitchInterface{
	GENERATED_BODY()

public:
	APlacableActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Data")
	UPlacableActorData* CurrentData;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Data")
	UInteractableComponent* InteractableComp;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Data")
	UAudioComponent* AudioComp;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Name")
	FName Name;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float AttackRange;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float AttackRate;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float Damages;

	UPROPERTY(BlueprintReadOnly, Category = "FX")
	UPopcornFXEmitterComponent* AttackFX;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	UAnimSequenceBase* AttackAnimation;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	UAnimSequenceBase* IdleAnimation;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	TSet<AMainAICharacter*> AIList;

	UPROPERTY(BlueprintReadOnly, Category = "Nexus", meta = (ExposeOnSpawn = "true"))
	ANexus* Nexus;

	UPROPERTY(BlueprintReadOnly, Category = "Player", meta = (ExposeOnSpawn = "true"))
	AMainPlayer* Player;

	UFUNCTION(BlueprintCallable, Category = "Appearance")
	virtual void SetMesh();

	UFUNCTION()
	virtual void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer);

	UFUNCTION()
	void SellDestroy();

	float GlitchGaugeValueOnDestruct;

	FTimeline FadeInAppearance;

	UPROPERTY(EditDefaultsOnly, Category = "Appearance")
	float AppearanceTime = 1.5;

	UStaticMeshComponent* WireframeMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Appearance")
	UMaterialInstance* WireframeMaterial;

public:
	virtual void Appear(const bool ReverseEffect, const FOnTimelineEvent AppearFinishEvent);

	void SellObject();

	void SetMissingData(ANexus* NewNexus, AMainPlayer* MainPlayer);

protected:
	bool bIsAppearing = false;

	UFUNCTION()
	virtual void FadeIn(float Alpha);

	UFUNCTION(BlueprintCallable, Category = "Appearance")
	virtual void EndAppearance();

	UCurveFloat* ZeroToOneCurve;

	UMaterialParameterCollection* AppearanceMaterialCollection;

	AConstructionZone* AffectedConstructionZone;

	APursuitDrone* CurrentDrone;

	UFUNCTION(BlueprintCallable, Category = "Appearance")
	virtual void SetObjectMaterial(UMaterialInterface* NewMaterial);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Attack();
	virtual void Attack_Implementation();

	UFUNCTION()
	virtual void OnReachVision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnLeaveVision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void AttachDroneToPlacable(APursuitDrone* NewDrone);

public:
	void AddDrone(AMainPlayer* MainPlayer);

	/**
	 * @brief 
	 * @param MainPlayer making MainPlayer valid will set MainPlayer's current drone
	 */
	virtual void RemoveDrone(AMainPlayer* MainPlayer);

	UFUNCTION(BlueprintCallable, Category = "PlayerActions")
	virtual void SetData(UPlacableActorData* NewData);

	void SetConstructionZone(AConstructionZone* NewConstructionZone);

	UFUNCTION(BlueprintCallable, Category = "PlayerActions")
	void Upgrade();

	UFUNCTION(BlueprintCallable, Category = "Glitch")
	virtual void ReceiveGlitchUpgrade() override;

	virtual void ResetGlitchUpgrade() override;

	virtual FPlacableActorSaveData SavePlacable();

	virtual void InitializePlacable(const FPlacableActorSaveData NewData, TArray<AActor*> PursuitDroneList);

	// used for delegates
	UFUNCTION()
	void CallDestroy();
};
