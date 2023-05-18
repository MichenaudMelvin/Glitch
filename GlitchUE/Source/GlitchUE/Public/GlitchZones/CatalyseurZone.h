// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GlitchUEGameMode.h"
#include "NavModifierComponent.h"
#include "Engine/StaticMeshActor.h"
#include "CatalyseurZone.generated.h"

UCLASS()
class GLITCHUE_API ACatalyseurZone : public AStaticMeshActor{
	GENERATED_BODY()

public:
	ACatalyseurZone();

protected:
	virtual void BeginPlay() override;

#if WITH_EDITORONLY_DATA
	UBillboardComponent* Billboard;
#endif

	UPROPERTY(EditDefaultsOnly);
	UNavModifierComponent* NavModifierComp;

	/**
	 * @brief Timer in seconds 
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Catalyseur Zone")
	float ResetLevelStateDuration = 1;

	FTimerHandle CatalyeurZoneTimer;

	AGlitchUEGameMode* GameMode;

	UPROPERTY(EditDefaultsOnly, Category = "Post Process UI")
	float SafeFadeTime = 0.5;

	UFUNCTION()
	virtual void EnterZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void ExitZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnSwitchPhases(EPhases NewPhase);
};
