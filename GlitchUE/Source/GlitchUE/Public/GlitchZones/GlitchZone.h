// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GlitchUEGameMode.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Components/TimelineComponent.h"
#include "GlitchZone.generated.h"

UCLASS()
class GLITCHUE_API AGlitchZone : public AStaticMeshActor{
	GENERATED_BODY()

public:
	AGlitchZone();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* TriggerBox;

	AGlitchUEGameMode* GameMode;

	FTimerHandle GlitchZoneTimer;

	UPROPERTY(EditDefaultsOnly, Category = "GlitchZone")
	float ResetLevelStateDuration = 5;

	FTimerHandle GlitchGaugeTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Glitch")
	float GlitchGaugeTick = 0.2;

	UPROPERTY(EditDefaultsOnly, Category = "Glitch")
	float GlitchGaugeValueToAddEveryTick = 100;

	UFUNCTION()
	void EnterGlitchZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ExitGlitchZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditDefaultsOnly, Category = "GlitchUI")
	UMaterialParameterCollection* GlitchMPC;

	AMainPlayer* MainPlayer;

	UPROPERTY(EditDefaultsOnly, Category = "GlitchUI")
	FWeightedBlendable PostProcessMaterialUI;

	FTimeline FadeInGlitchEffectTimeline;

	UCurveFloat* ZeroToOneCurve;

	UPROPERTY(EditDefaultsOnly, Category = "GlitchUI")
	float GlitchFadeTime = 0.5;

	void EnableGlitchEffect(const bool bEnable);

	UFUNCTION()
	void FadeInGlitchEffect(float Value);

	UFUNCTION()
	void EndFadeIn();
};
