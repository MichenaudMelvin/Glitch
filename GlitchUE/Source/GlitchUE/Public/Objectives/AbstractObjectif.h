// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/HealthComponent.h"
#include "Components/ActivableComponent.h"
//#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "AbstractObjectif.generated.h"

UCLASS(Abstract)
class GLITCHUE_API AAbstractObjectif : public AActor{
	GENERATED_BODY()
	
public:	
	AAbstractObjectif();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* MeshObjectif;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Health")
	UHealthComponent* HealthComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Actiable")
	UActivableComponent* ActivableComp;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Perception")
	//UAIPerceptionStimuliSourceComponent* AIPerceptionTarget;

	virtual void ActiveObjectif();

	virtual void DesactivateObjectif();

	virtual void OnHealthNull();

public:	
	virtual void Tick(float DeltaTime) override;

	UActivableComponent* GetActivableComp();
};
