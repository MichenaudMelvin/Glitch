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

	UFUNCTION(BlueprintCallable)
	bool CheckSpotSpace();

	FVector OriginalLocation;

	bool bInConstructionZone;

	UMaterial* HologramMaterial;

	void SetMesh() override;

public:
	UFUNCTION(BlueprintCallable)
	void SetInConstructionZone(bool bNewValue);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PreviewPlacable")
	bool CanBePlaced();

	UFUNCTION(BlueprintCallable, Category = "PreviewPlacable")
	void ResetActor();
};
