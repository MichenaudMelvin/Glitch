// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlacableObject/PlacableActor.h"
#include "PreviewPlacableActor.generated.h"

UCLASS()
class GLITCHUE_API APreviewPlacableActor : public APlacableActor{
	GENERATED_BODY()

public:
	APreviewPlacableActor();

protected:
	virtual void BeginPlay() override;

	AMainPlayer* MainPlayer;

	UFUNCTION(BlueprintCallable)
	bool CheckSpotSpace();

	FVector OriginalLocation;

	bool bInConstructionZone;

	UMaterial* HologramMaterial;

	void SetMesh() override;

public:
	virtual void SetData(UPlacableActorData* NewData) override;

	UFUNCTION(BlueprintCallable)
	void SetInConstructionZone(bool bNewValue);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PreviewPlacable")
	bool CanBePlaced();

	UFUNCTION(BlueprintCallable, Category = "PreviewPlacable")
	void ChooseColor();

	UFUNCTION(BlueprintCallable, Category = "PreviewPlacable")
	void ResetActor();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PreviewPlacable")
	FVector GetOriginalLocation();

	UStaticMeshComponent* GetPreviewMesh() const;

	void SetPlayer(AMainPlayer* NewPlayer);
};
