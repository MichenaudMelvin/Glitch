// Fill out your copyright notice in the Description page of Project Settings.


#include "Helpers/FunctionsLibrary/UsefullFunctions.h"

void UUsefullFunctions::OutlineComponent(bool SetOutline, UPrimitiveComponent* Component){
	if (!IsValid(Component)){
		return;
	}

	Component->SetRenderCustomDepth(SetOutline);
	Component->SetCustomDepthStencilValue(SetOutline ? 2 : 0);
}
