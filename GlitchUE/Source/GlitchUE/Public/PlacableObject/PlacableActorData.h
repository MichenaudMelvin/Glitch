// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlacableActorData.generated.h"

UCLASS()
class GLITCHUE_API UPlacableActorData : public UPrimaryDataAsset{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Name")
	FName Name;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Golds")
	int Cost;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Golds")
	UPlacableActorData* NextUpgrade;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Mesh")
	TArray<UStaticMesh*> MeshList;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Mesh")
	UStaticMesh* FullMesh;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Golds")
	bool HasEnoughGolds(int PlayerGolds);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Glitch")
	float GlitchUpgradeDuration;
};
