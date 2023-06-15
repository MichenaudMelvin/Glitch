// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InteractableComponent.h"
#include "GameFramework/Actor.h"
#include "Helpers/UsefulStructs.h"
#include "Player/MainPlayerController.h"
#include "BanLog.generated.h"

UCLASS()
class GLITCHUE_API ABanLog : public AActor{
	GENERATED_BODY()

public:
	ABanLog();

protected:
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditDefaultsOnly, Category = "Root")
	USceneComponent* SceneComp;

	UPROPERTY(EditDefaultsOnly, Category = "Root")
	UActivableComponent* ActivableComp;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* MedMesh;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMesh* MedMeshObject;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* TechMesh;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMesh* TechMeshObject;

	UPROPERTY(EditDefaultsOnly, Category = "Interacable")
	UInteractableComponent* InteractableComp;

	UPROPERTY(EditAnywhere, Category = "Tchat")
	TArray<FTchatStruct> TchatMessageList;

	UFUNCTION()
	void Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer);

	UFUNCTION()
	void ActivateBanLog();

public:
	bool IsActivated() const;

	void RemoveInteraction() const;
};
