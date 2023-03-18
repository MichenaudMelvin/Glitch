// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/Inhibiteur.h"
#include "GlitchUEGameMode.h"
#include "Kismet/GameplayStatics.h"

AInhibiteur::AInhibiteur(){
	MeshObjectif->SetCanEverAffectNavigation(true);

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> ActivAnim(TEXT("/Game/Meshs/Objectives/Inhibiteur/AS_Inhibiteur"));
	check(ActivAnim.Succeeded());

	ActivationAnim = ActivAnim.Object;
}

void AInhibiteur::BeginPlay(){
	Super::BeginPlay();

#if !UE_BUILD_SHIPPING

	if(ConstructionZoneList.Num() == 0){
		UE_LOG(LogTemp, Fatal, TEXT("L'INHIBITEUR %s N'AFFECTE AUCUNE ZONE DE CONSTRUCTION"), *this->GetName());
	}

#endif
}

void AInhibiteur::ActiveObjectif(){
	ActivateLinkedElements(true);

	MeshObjectif->PlayAnimation(ActivationAnim, false);

	SpriteReference.DestroyComponents();
}

void AInhibiteur::DesactivateObjectif(){
	ActivateLinkedElements(false);
}

void AInhibiteur::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	const AGlitchUEGameMode* Gamemode = Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (Gamemode->GetLevelState() == ELevelState::Normal && Gamemode->GetPhases() == EPhases::Infiltration && !ActivableComp->IsActivated()){
		ActivableComp->ActivateObject();
	}
}

void AInhibiteur::ActivateLinkedElements(const bool bActivate){
	for (int i = 0; i < ConstructionZoneList.Num(); i++) {
		if (bActivate) {
			ConstructionZoneList[i]->GetActivableComp()->ActivateObject();
		}
		else {
			ConstructionZoneList[i]->GetActivableComp()->DesactivateObject();
		}
	}
}

void AInhibiteur::SetSpriteReference(const FCompassSprite NewSprite){
	SpriteReference = NewSprite;
}
