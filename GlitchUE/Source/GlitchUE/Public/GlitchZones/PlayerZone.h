// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Player/MainPlayer.h"
#include "PlayerZone.generated.h"

UCLASS(Abstract)
class GLITCHUE_API APlayerZone : public AStaticMeshActor{
	GENERATED_BODY()

public:
	APlayerZone();

protected:
	virtual void BeginPlay() override;

	virtual void OnPlayerEnterZone();

	virtual void OnPlayerExitZone();

	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
	UNavModifierComponent* NavModifier;

#if WITH_EDITORONLY_DATA
	UBillboardComponent* Billboard;
#endif

	AGlitchUEGameMode* GameMode;

	AMainPlayer* MainPlayer;

	FTimerHandle ZoneTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Post Process UI")
	float PostProcessFadeTime = 0.5f;

private:
	UFUNCTION()
	void EnterZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ExitZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
