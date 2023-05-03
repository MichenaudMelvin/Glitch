// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objectives/AbstractObjectif.h"
#include "Nexus.h"
#include "Components/CompassIcon.h"
#include "PlacableObject/ConstructionZone.h"
#include "Catalyseur.generated.h"

class AInhibiteur;

USTRUCT()
struct FCompassSprite{
	GENERATED_BODY()

	/**
	 * @brief Do not use this constructor
	 */
	FCompassSprite();

	FCompassSprite(USceneComponent* SceneComp, UPaperSpriteComponent* PaperSpriteComp);

	USceneComponent* SceneComponent;

	UPaperSpriteComponent* PaperSpriteComponent;

	void DestroyComponents();
};

UCLASS()
class GLITCHUE_API ACatalyseur : public AAbstractObjectif{
	GENERATED_BODY()

public:
	ACatalyseur();

	USkeletalMeshComponent* GetTechMesh() const;

protected:
	virtual void BeginPlay() override;

	virtual void ActiveObjectif() override;

	virtual void DesactivateObjectif() override;

	virtual void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UAnimationAsset* ActivationAnim;

	UAnimationAsset* DesactivationAnim;

	virtual void HealthNull() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* TECHMesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* CatalyeurZone;

	ANexus* Nexus;

	AWaveManager* WaveManager;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inhibiteur", meta = (ExposeOnSpawn = "true"))
	TArray<AInhibiteur*> LinkedInhibiteur;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Inhibiteur")
	UPaperSprite* InhibiteurSprite;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Inhibiteur")
	float CompassRadius = 100;

	void GenerateCompass();

	void DeleteCompass();

	TArray<FCompassSprite> CompassSpriteList;

	FTimerHandle CatalyeurZoneTimer;

	/**
	 * @brief Timer in seconds 
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Catalyseur Zone")
	float ResetLevelStateDuration = 1;

	UFUNCTION()
	virtual void EnterCatalyseurZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void ExitCatalyseurZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

#if WITH_EDITORONLY_DATA
	virtual void PreEditChange(FProperty* PropertyAboutToChange) override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	void OnObjectSelected(UObject* Object);

	void OutlineLinkedObjects(const bool bOutline);
#endif
};
