// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/Inhibiteur.h"
#include "GlitchUEGameMode.h"
#include "Engine/Selection.h"
#include "Helpers/FunctionsLibrary/UsefullFunctions.h"
#include "Kismet/GameplayStatics.h"

AInhibiteur::AInhibiteur(){
	MeshObjectif->SetCanEverAffectNavigation(true);

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> ActivAnim(TEXT("/Game/Meshs/Objectives/Inhibiteur/AS_Inhibiteur"));
	check(ActivAnim.Succeeded());

	ActivationAnim = ActivAnim.Object;

	#if WITH_EDITORONLY_DATA
		USelection::SelectObjectEvent.AddUObject(this, &AInhibiteur::OnObjectSelected);
	#endif
}

void AInhibiteur::BeginPlay(){
	Super::BeginPlay();

#if WITH_EDITOR

	if(ConstructionZoneList.Num() == 0){
		UE_LOG(LogTemp, Fatal, TEXT("L'INHIBITEUR %s N'AFFECTE AUCUNE ZONE DE CONSTRUCTION"), *this->GetName());
	}

#endif
}

void AInhibiteur::ActiveObjectif(){
	ActivateLinkedElements(true);

	MeshObjectif->PlayAnimation(ActivationAnim, false);

	if(IsValid(SpriteReference.SceneComponent) && IsValid(SpriteReference.PaperSpriteComponent)){
		SpriteReference.DestroyComponents();
	}
}

void AInhibiteur::DesactivateObjectif(){
	ActivateLinkedElements(false);
}

void AInhibiteur::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	const AGlitchUEGameMode* Gamemode = Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (Gamemode->GetPhases() == EPhases::Infiltration && !ActivableComp->IsActivated()){
		ActivableComp->ActivateObject();
	}
}

void AInhibiteur::ActivateLinkedElements(const bool bActivate){
	for (int i = 0; i < ConstructionZoneList.Num(); i++) {
		if (bActivate) {
			ConstructionZoneList[i]->GetActivableComp()->ActivateObject();
		} else {
			ConstructionZoneList[i]->GetActivableComp()->DesactivateObject();
		}
	}
}

void AInhibiteur::SetSpriteReference(const FCompassSprite NewSprite){
	SpriteReference = NewSprite;
}

#if WITH_EDITORONLY_DATA
void AInhibiteur::OnObjectSelected(UObject* Object){
	if (Object == this) {

		for(int i = 0; i < ConstructionZoneList.Num(); i++){
			UUsefullFunctions::OutlineComponent(true, Cast<UPrimitiveComponent>(ConstructionZoneList[i]->GetRootComponent()));
		}

	} else if (!IsSelected()){

		for(int i = 0; i < ConstructionZoneList.Num(); i++){
			UUsefullFunctions::OutlineComponent(false, Cast<UPrimitiveComponent>(ConstructionZoneList[i]->GetRootComponent()));
		}
	}
}
#endif
