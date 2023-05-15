// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GlitchUEGameMode.h"
#include "NavModifierComponent.h"
#include "Engine/StaticMeshActor.h"
#include "GlitchZone.generated.h"

UCLASS()
class GLITCHUE_API AGlitchZone : public AStaticMeshActor{
	GENERATED_BODY()

public:
	AGlitchZone();

protected:
	virtual void BeginPlay() override;

#if WITH_EDITORONLY_DATA
	UBillboardComponent* Billboard;
#endif

	AGlitchUEGameMode* GameMode;

	AAudioManager* AudioManager;

	UPROPERTY(EditDefaultsOnly);
	UNavModifierComponent* NavModifierComp;

	FTimerHandle GlitchGaugeTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Glitch")
	float GlitchGaugeTick = 0.2;

	UPROPERTY(EditDefaultsOnly, Category = "Glitch")
	float GlitchGaugeValueToAddAtStart = 500;

	UPROPERTY(EditDefaultsOnly, Category = "Glitch")
	float GlitchGaugeValueToAddEveryTick = 100;

	UFUNCTION()
	void EnterGlitchZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ExitGlitchZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	AMainPlayer* MainPlayer;

	UPROPERTY(EditDefaultsOnly, Category = "GlitchUI")
	float GlitchFadeTime = 0.5;
};
