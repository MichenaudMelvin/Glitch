// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PatrolPoint.h"
#include "AI/MainAICharacter.h"
#include "Components/HearingComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/SplineMeshActor.h"
#include "PatrolCharacter.generated.h"

UCLASS(Abstract)
class GLITCHUE_API APatrolCharacter : public AMainAICharacter{
	GENERATED_BODY()

public:
	APatrolCharacter();

protected:
	virtual void Destroyed() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Behavior")
	TArray<APatrolPoint*> PatrolPointsList;

	UPROPERTY(EditDefaultsOnly, Category = "Hearing")
	UHearingComponent* HearingComponent;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sight")
	UWidgetComponent* SightWidget;

public:
	TArray<APatrolPoint*> GetPatrolPointList() const;

#if WITH_EDITORONLY_DATA
	virtual void PreEditChange(FProperty* PropertyAboutToChange) override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void PostEditUndo() override;

	void OnObjectSelected(UObject* Object);

	void OutlineLinkedObjects(const bool bOutline);

	virtual void PreSave(const ITargetPlatform* TargetPlatform) override;

	UStaticMesh* CubeMesh;

	TArray<ASplineMeshActor*> SplineList;
#endif
};
