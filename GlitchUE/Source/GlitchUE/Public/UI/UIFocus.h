// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UIFocus.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUIFocus : public UInterface{
	GENERATED_BODY()
};

class GLITCHUE_API IUIFocus{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ReceiveFocus();

	virtual void UnReceiveFocus();
};
