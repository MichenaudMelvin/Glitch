// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlacableActorData.h"
#include "Components/HealthComponent.h"
#include "AI/MainAICharacter.h"
#include "Objectives/Nexus.h"
#include "Components/AudioComponent.h"
#include "Components/TimelineComponent.h"
#include "NavModifierComponent.h"
#include "PopcornFXEmitter.h"
#include "PlacableActor.generated.h"

class AMainPlayerController;
class AMainPlayer;
class UInteractableComponent;
class AConstructionZone;

USTRUCT(BlueprintType)
struct FPlacableActorCreation{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<APlacableActor> Class;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UPlacableActorData* Data;
};

UCLASS()
class GLITCHUE_API APlacableActor : public AActor{
	GENERATED_BODY()
	
public:	
	APlacableActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	UPlacableActorData* CurrentData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	UInteractableComponent* InteractableComp;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Data")
	UAudioComponent* AudioComp;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Navigation")
	UNavModifierComponent* NavModifierComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Name")
	FName Name;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float AttackRange;

	UPROPERTY(BlueprintReadOnly, Category = "FX")
	UPopcornFXEmitterComponent* AttackFX;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	TSet<AMainAICharacter*> AIList;

	UPROPERTY(BlueprintReadOnly, Category = "Nexus", meta = (ExposeOnSpawn = "true"))
	ANexus* Nexus;

	UFUNCTION(BlueprintCallable, Category = "Appearence")
	virtual void SetMesh();

	UFUNCTION()
	virtual void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer);

	void SellObject(AMainPlayer* MainPlayer);

	FTimeline FadeInAppearence;

	UFUNCTION()
	void FadeIn(float Alpha);

	UCurveFloat* ZeroToOneCurve;

	UMaterialParameterCollection* AppearenceMaterialCollection;

	AConstructionZone* AffectedConstructionZone;

	UFUNCTION(BlueprintCallable, Category = "Appearence")
	virtual void SetObjectMaterial(UMaterialInterface* NewMaterial);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Appearence")
	void EndAppearence();
	virtual void EndAppearence_Implementation();

public:
	UFUNCTION(BlueprintCallable, Category = "PlayerActions")
	virtual void SetData(UPlacableActorData* NewData);

	void SetConstructionZone(AConstructionZone* NewConstructionZone);

	UFUNCTION(BlueprintCallable, Category = "PlayerActions")
	void Upgrade();

	UFUNCTION(BlueprintCallable, Category = "Glitch")
	virtual void GlitchUpgrade();
};
