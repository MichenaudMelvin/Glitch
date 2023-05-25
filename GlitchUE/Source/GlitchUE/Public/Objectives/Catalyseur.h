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

USTRUCT()
struct FCompassSprite{
	GENERATED_BODY()

	/**
	 * @brief Do not use this constructor
	 */
	FCompassSprite();

	FCompassSprite(USceneComponent* SceneComp, UStaticMeshComponent* StaticMeshComp);

	UPROPERTY()
	USceneComponent* SceneComponent;

	UPROPERTY()
	UStaticMeshComponent* StaticMeshComponent;

	void DestroyComponents() const;
};

UCLASS()
class GLITCHUE_API ACatalyseur : public AAbstractObjectif{
	GENERATED_BODY()

public:
	ACatalyseur();

	USkeletalMeshComponent* GetTechMesh() const;

	void AddInhibiteurToActivatedList(AInhibiteur* InhibiteurToAdd);

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

	TArray<AInhibiteur*> ActivatedInhibiteursList;

	UPROPERTY(EditDefaultsOnly, Category = "Inhibiteur")
	UStaticMesh* InhibiteurMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Inhibiteur")
	float InhibiteurIconScale = 0.5;

	UPROPERTY(EditDefaultsOnly, Category = "Inhibiteur")
	float CompassRadius = 100;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	UFMODEvent* ActivationSFX;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	UFMODEvent* DeactivationSFX;

	void GenerateCompass();

	void DeleteCompass();

	UPROPERTY()
	TArray<FCompassSprite> CompassSpriteList;

	FTimerHandle MoneyTimerHandle;

	void GenerateMoney();

	void StartGeneratingMoney();

	UPROPERTY(EditDefaultsOnly, Category = "Golds")
	int GeneratedGolds = 100;

	UPROPERTY(EditDefaultsOnly, Category = "Golds")
	int GoldsBonus = 500;

	UPROPERTY(EditDefaultsOnly, Category = "Golds")
	int GoldsTick = 10;

#if WITH_EDITORONLY_DATA
	virtual void PreEditChange(FProperty* PropertyAboutToChange) override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	void OnObjectSelected(UObject* Object);

	void OutlineLinkedObjects(const bool bOutline);

	virtual void PreSave(const ITargetPlatform* TargetPlatform) override;
#endif
};
