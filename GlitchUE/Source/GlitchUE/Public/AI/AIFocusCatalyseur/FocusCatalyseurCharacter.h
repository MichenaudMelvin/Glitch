// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/FocusAICharacter.h"
#include "FocusCatalyseurCharacter.generated.h"

UCLASS(Abstract)
class GLITCHUE_API AFocusCatalyseurCharacter : public AFocusAICharacter{
	GENERATED_BODY()

public:
	AFocusCatalyseurCharacter();


protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* ScopeMesh;
};
