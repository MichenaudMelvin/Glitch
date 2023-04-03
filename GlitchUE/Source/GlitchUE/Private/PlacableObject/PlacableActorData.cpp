// Fill out your copyright notice in the Description page of Project Settings.

#include "PlacableObject/PlacableActorData.h"

#if WITH_EDITOR
void UPlacableActorData::PostLoad(){
    Super::PostLoad();

    RealGlitchDamages = Damages + GlitchDamages;
    RealGlitchAttackRate = AttackRate + GlitchAttackRate;
    RealGlitchAttackRange = AttackRange + GlitchAttackRange;

    RealBoostDroneDamages = Damages + BoostDroneDamages;
    RealBoostDroneAttackRate = AttackRate + BoostDroneAttackRate;
    RealBoostDroneAttackRange = AttackRange + BoostDroneAttackRange;
}

void UPlacableActorData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent){
    Super::PostEditChangeProperty(PropertyChangedEvent);

    RealGlitchDamages = Damages + GlitchDamages;
    RealGlitchAttackRate = AttackRate + GlitchAttackRate;
    RealGlitchAttackRange = AttackRange + GlitchAttackRange;

    RealBoostDroneDamages = Damages + BoostDroneDamages;
    RealBoostDroneAttackRate = AttackRate + BoostDroneAttackRate;
    RealBoostDroneAttackRange = AttackRange + BoostDroneAttackRange;
}
#endif

bool UPlacableActorData::HasEnoughGolds(const int PlayerGolds) const{
    return PlayerGolds >= Cost;
}
