// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GeneralDecorators/CheckCatalyseursLeft.h"
#include "AI/AIFocusCatalyseur/FocusCatalyseurController.h"

bool UCheckCatalyseursLeft::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const{
	const TArray<ACatalyseur*> CatalyseurList = Cast<AFocusCatalyseurController>(OwnerComp.GetAIOwner())->GetCatalyseurList();

	for(int i = 0; i < CatalyseurList.Num(); i++){
		if(CatalyseurList[i]->GetActivableComp()->IsActivated()){
			return bExecuteIfCatalyseursRemains;
		}
	}

	return !bExecuteIfCatalyseursRemains;
}
