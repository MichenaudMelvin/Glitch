// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/HealthComponent.h"
#include "Components/ActivableComponent.h"
//#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Components/InteractableComponent.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	UInteractableComponent* InteractableComp;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Perception")
	//UAIPerceptionStimuliSourceComponent* AIPerceptionTarget;

	UFUNCTION()
	virtual void ActiveObjectif();

	UFUNCTION()
	virtual void DesactivateObjectif();

	UFUNCTION()
	virtual void OnHealthNull();

	UFUNCTION()
	virtual void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer);

public:
	UActivableComponent* GetActivableComp();
};
