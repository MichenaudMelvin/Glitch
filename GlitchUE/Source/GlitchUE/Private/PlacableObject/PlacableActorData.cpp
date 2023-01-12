// Fill out your copyright notice in the Description page of Project Settings.


#include "PlacableObject/PlacableActorData.h"

bool UPlacableActorData::HasEnoughGolds(int PlayerGolds){
    return PlayerGolds >= Cost;
}
