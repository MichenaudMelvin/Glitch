// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GlitchInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGlitchInterface : public UInterface{
	GENERATED_BODY()
};


class GLITCHUE_API IGlitchInterface{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ReciveGlitchUpgrade();

	virtual void ResetGlitchUpgrade();
};
