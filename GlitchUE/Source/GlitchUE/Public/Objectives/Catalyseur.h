// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objectives/AbstractObjectif.h"
#include "Components/Waypoint.h"
#include "Components/WidgetComponent.h"
#include "Catalyseur.generated.h"

class AInhibiteur;
class AMainPlayer;

UCLASS()
class GLITCHUE_API ACatalyseur : public AAbstractObjectif{
	GENERATED_BODY()

public:
	ACatalyseur();

	USkeletalMeshComponent* GetTechMesh() const;

	void AddInhibiteurToActivatedList(AInhibiteur* InhibiteurToAdd);

	UCompassComponent* GetCompass() const;

	FCatalyseurData SaveCatalyseur();

	void LoadCatalyseur(const FCatalyseurData NewData);

protected:
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	UFUNCTION()
	void OnCleanWorld(UWorld* World, bool bSessionEnded, bool bCleanupResources);

	virtual void ActiveObjectif() override;

	virtual void DesactivateObjectif() override;

	void StartDesactivationTimer(const float Timer);

	virtual void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer) override;

	virtual void OnSwitchPhases(EPhases CurrentPhase) override;

	bool bWasActivatedInStealthPhase = false;

	UAnimationAsset* ActivationAnim;

	UAnimationAsset* DesactivationAnim;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UPopcornFXEmitterComponent* DesactivationFX;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UPopcornFXEmitterComponent* IdleFX;

	UPROPERTY(EditDefaultsOnly, Category = "Feedback")
	UWaypoint* DesactivationBillboard;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UPopcornFXEmitterComponent* GoldsGenerationFX;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	FLinearColor CannotInteractWithColor = FLinearColor::Red;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	FLinearColor CanInteractWithColor = FLinearColor::Green;

	virtual void HealthNull() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* TECHMesh;

	ANexus* Nexus;

	AMainPlayer* Player;

	AWaveManager* WaveManager;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inhibiteur", meta = (ExposeOnSpawn = "true"))
	TArray<AInhibiteur*> LinkedInhibiteur;

	UPROPERTY()
	UFMODEvent* SoundsGolds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Construction", meta = (ExposeOnSpawn = "true"))
	TArray<AConstructionZone*> ConstructionZoneList;

	TArray<AInhibiteur*> ActivatedInhibiteursList;

	UPROPERTY(EditDefaultsOnly, Category = "Compass")
	UCompassComponent* Compass;

	FTimerHandle MoneyTimerHandle;

	void GenerateMoney();

	void StartGeneratingMoney();

	UPROPERTY(EditDefaultsOnly, Category = "Golds")
	int GeneratedGolds = 100;

	UPROPERTY(EditDefaultsOnly, Category = "Golds")
	int GoldsTick = 10;

	/**
	 * @brief Timer in seconds
	 */
	UPROPERTY(EditAnywhere, Category = "Time", meta = (ClampMin = 0))
	float DesactivationTimer = 60.0f;

	FTimerHandle DesactivationTimerHandle;

#if WITH_EDITORONLY_DATA
	virtual void PreEditChange(FProperty* PropertyAboutToChange) override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	void OnObjectSelected(UObject* Object);

	void OutlineLinkedObjects(const bool bOutline);

	virtual void PreSave(const ITargetPlatform* TargetPlatform) override;
#endif
};
