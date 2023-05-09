// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PatrolPoint.h"
#include "AI/MainAICharacter.h"
#include "Engine/SplineMeshActor.h"
#include "PatrolCharacter.generated.h"

UCLASS(Abstract)
class GLITCHUE_API APatrolCharacter : public AMainAICharacter{
	GENERATED_BODY()

public:
	APatrolCharacter();

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Behavior")
	TArray<APatrolPoint*> PatrolPointsList;

public:
	TArray<APatrolPoint*> GetPatrolPointList() const;

#if WITH_EDITORONLY_DATA
	virtual void PreEditChange(FProperty* PropertyAboutToChange) override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	void OnObjectSelected(UObject* Object);

	void OutlineLinkedObjects(const bool bOutline);

	virtual void PreSave(const ITargetPlatform* TargetPlatform) override;

	UStaticMesh* CubeMesh;

	TArray<ASplineMeshActor*> SplineList;
#endif
};
