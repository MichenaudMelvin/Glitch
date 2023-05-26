// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FMODBlueprintStatics.h"
#include "FMODEvent.h"
#include "Gamemodes/GlitchUEGameMode.h"
#include "Objectives/AbstractObjectif.h"
#include "Nexus.h"
#include "Components/CompassIcon.h"
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

protected:
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	virtual void ActiveObjectif() override;

	virtual void DesactivateObjectif() override;

	void ToggleActivatedInhibiteursState(const bool ActivateInhibiteurs = true);

	virtual void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer) override;

	UFUNCTION()
	void OnSwitchPhases(EPhases CurrentPhase);

	UAnimationAsset* ActivationAnim;

	UAnimationAsset* DesactivationAnim;

	virtual void HealthNull() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* TECHMesh;

	ANexus* Nexus;

	AMainPlayer* Player;

	AWaveManager* WaveManager;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inhibiteur", meta = (ExposeOnSpawn = "true"))
	TArray<AInhibiteur*> LinkedInhibiteur;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Construction", meta = (ExposeOnSpawn = "true"))
	TArray<AConstructionZone*> ConstructionZoneList;

	TArray<AInhibiteur*> ActivatedInhibiteursList;

	UPROPERTY(EditDefaultsOnly, Category = "Inhibiteur")
	UStaticMesh* InhibiteurMesh;

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
	UPROPERTY(EditDefaultsOnly, Category = "Time", meta = (ClampMin = 0))
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
