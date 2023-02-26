// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIPatrol/PatrolCharacter.h"

const TArray<APatrolPoint*> APatrolCharacter::GetPatrolPointList(){
	return PatrolPointsList;
}
