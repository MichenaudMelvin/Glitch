// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/Inhibiteur.h"
#include "Gamemodes/GlitchUEGameMode.h"
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
		USelection::SelectionChangedEvent.AddUObject(this, &AInhibiteur::OnObjectSelected);
	#endif
}

void AInhibiteur::BeginPlay(){
	Super::BeginPlay();

#if WITH_EDITOR

	if(ConstructionZoneList.Num() == 0){
		UE_LOG(LogTemp, Fatal, TEXT("L'INHIBITEUR %s N'AFFECTE AUCUNE ZONE DE CONSTRUCTION"), *this->GetName());
	}

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&](){
		if(!IsValid(OwnerCatalyseur)){
			UE_LOG(LogTemp, Fatal, TEXT("L'INHIBITEUR %s N'EST PAS AFFECTE A UN CATALYSEUR"), *this->GetName());
		}
	}, 0.2f, false);

#endif
}

void AInhibiteur::Destroyed(){
	Super::Destroyed();

#if WITH_EDITORONLY_DATA
	OutlineLinkedObjects(false);
#endif
}

void AInhibiteur::ActiveObjectif(){
	ActivateLinkedElements(true);

	MeshObjectif->PlayAnimation(ActivationAnim, false);

	UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), ActivationSFX, GetActorTransform(), true);

	if(GameMode->GetPhases() == EPhases::Infiltration){
		GameMode->LaunchStealthTimer();

		SpriteReference.DestroyComponents();
	}
}

void AInhibiteur::DesactivateObjectif(){
	ActivateLinkedElements(false);
}

void AInhibiteur::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	if (GameMode->GetPhases() == EPhases::Infiltration && !ActivableComp->IsActivated()){
		ActivableComp->ActivateObject();
	}
}

void AInhibiteur::ActivateLinkedElements(const bool bActivate){
	for (int i = 0; i < ConstructionZoneList.Num(); i++){
		bActivate ? ConstructionZoneList[i]->GetActivableComp()->ActivateObject() : ConstructionZoneList[i]->GetActivableComp()->DesactivateObject();
	}

	if(GameMode->GetPhases() == EPhases::Infiltration){
		bActivate ? OwnerCatalyseur->GetActivableComp()->ActivateObject() : OwnerCatalyseur->GetActivableComp()->DesactivateObject();
		OwnerCatalyseur->AddInhibiteurToActivatedList(this);
	}
}

void AInhibiteur::SetSpriteReference(const FCompassSprite NewSprite){
	SpriteReference = NewSprite;
}

void AInhibiteur::SetOwnerCatalyseur(ACatalyseur* NewOwner){
	OwnerCatalyseur = NewOwner;
}

#if WITH_EDITORONLY_DATA
void AInhibiteur::PreEditChange(FProperty* PropertyAboutToChange){
	Super::PreEditChange(PropertyAboutToChange);

	OutlineLinkedObjects(false);
}

void AInhibiteur::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent){
	Super::PostEditChangeProperty(PropertyChangedEvent);

	OutlineLinkedObjects(true);
}


void AInhibiteur::OnObjectSelected(UObject* Object){
	if (Object == this) {

		OutlineLinkedObjects(true);

	} else if (!IsSelected()){

		OutlineLinkedObjects(false);
	}
}

void AInhibiteur::OutlineLinkedObjects(const bool bOutline){
	for(int i = 0; i < ConstructionZoneList.Num(); i++){
		if(IsValid(ConstructionZoneList[i])){
			UUsefullFunctions::OutlineComponent(bOutline, Cast<UPrimitiveComponent>(ConstructionZoneList[i]->GetRootComponent()));
			UUsefullFunctions::OutlineComponent(bOutline, ConstructionZoneList[i]->GetTechMesh());
		}
	}

	if(IsValid(OwnerCatalyseur)){
		UUsefullFunctions::OutlineComponent(bOutline, Cast<UPrimitiveComponent>(OwnerCatalyseur->GetRootComponent()));
		UUsefullFunctions::OutlineComponent(bOutline, OwnerCatalyseur->GetTechMesh());
	}
}

void AInhibiteur::PreSave(const ITargetPlatform* TargetPlatform){
	Super::PreSave(TargetPlatform);

	OutlineLinkedObjects(false);
}
#endif
