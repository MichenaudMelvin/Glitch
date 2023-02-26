// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Navigation/NavAreaCostAsOne.h"
#include "AI/Navigation/MainPath.h"

UNavAreaCostAsOne::UNavAreaCostAsOne(){
	FNavigationFilterArea NavFilterArea;
	NavFilterArea.AreaClass = UMainPath::StaticClass();

	NavFilterArea.bOverrideTravelCost = true;
	NavFilterArea.TravelCostOverride = 1;
	NavFilterArea.bIsExcluded = false;

	Areas.Add(NavFilterArea);
}
