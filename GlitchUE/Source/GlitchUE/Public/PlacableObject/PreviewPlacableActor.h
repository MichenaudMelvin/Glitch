// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlacableObject/PlacableActor.h"
#include "PreviewPlacableActor.generated.h"

UCLASS(Abstract)
class GLITCHUE_API APreviewPlacableActor : public APlacableActor{
	GENERATED_BODY()

public:
	APreviewPlacableActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* BaseMesh;

	AMainPlayer* MainPlayer;

	FVector OriginalLocation;

	UMaterial* HologramMaterial;

	void SetMesh() override;

	UPROPERTY(EditDefaultsOnly, Category = "Color")
	FVector CanPlaceColor = FVector(0, 0.247059, 1);

	UPROPERTY(EditDefaultsOnly, Category = "Color")
	FVector CannotPlaceColor = FVector(1, 0,  0);

public:
	virtual void SetData(UPlacableActorData* NewData) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PreviewPlacable")
	bool CanBePlaced() const;

	UFUNCTION(BlueprintCallable, Category = "PreviewPlacable")
	void ChooseColor() const;

	UFUNCTION(BlueprintCallable, Category = "PreviewPlacable")
	void ResetActor();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PreviewPlacable")
	FVector GetOriginalLocation() const;

	UStaticMeshComponent* GetPreviewMesh() const;

	void SetPlayer(AMainPlayer* NewPlayer);
};
