// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UsefullFunctions.generated.h"

UCLASS()
class GLITCHUE_API UUsefullFunctions : public UBlueprintFunctionLibrary{
	GENERATED_BODY()
	
	public:
		static void OutlineComponent(bool SetOutline, UPrimitiveComponent* Component);
};
