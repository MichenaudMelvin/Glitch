// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/Inhibiteur.h"
#include "Gamemodes/GlitchUEGameMode.h"
#include "Engine/Selection.h"
#include "Helpers/FunctionsLibrary/UsefulFunctions.h"
#include "Components/CompassComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FMODBlueprintStatics.h"
#include "PopcornFXAttributeFunctions.h"

AInhibiteur::AInhibiteur(){
	MeshObjectif->SetCanEverAffectNavigation(false);

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> ActivAnim(TEXT("/Game/Meshs/Objectives/Inhibiteur/AS_Inhibiteur"));
	check(ActivAnim.Succeeded());

	ActivationAnim = ActivAnim.Object;

	NavModifier->SetBoxExtent(FVector(75, 75, 10));

	CompassIcon = CreateDefaultSubobject<UCompassIcon>(TEXT("Compass Icon"));
	CompassIcon->SetupAuto(false);
	CompassIcon->SetShouldUseTick(false);
	CompassIcon->SetOwnerClass(ACatalyseur::StaticClass());
	CompassIcon->SetDrawDistance(0);

	LockerFX = CreateDefaultSubobject<UPopcornFXEmitterComponent>(TEXT("Locker FX"));
	LockerFX->SetupAttachment(RootComponent);
	LockerFX->SetRelativeLocation(FVector(0, 0, 150));

	static ConstructorHelpers::FObjectFinder<UPopcornFXEffect> FX(TEXT("/Game/VFX/Particles/FX_Environment/Pk_Unlocking"));
	check(FX.Succeeded());

	LockerFX->Effect = FX.Object;

	#if WITH_EDITORONLY_DATA
		USelection::SelectObjectEvent.AddUObject(this, &AInhibiteur::OnObjectSelected);
		USelection::SelectionChangedEvent.AddUObject(this, &AInhibiteur::OnObjectSelected);
	#endif
}

void AInhibiteur::BeginPlay(){
	Super::BeginPlay();

	InteractableComp->AddInteractable(MeshObjectif);

	LockerFX->OnEmissionStops.AddDynamic(this, &AInhibiteur::DestroyFX);

#if WITH_EDITOR

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&](){
		if(!IsValid(OwnerCatalyseur)){
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow,FString::Printf(TEXT("L'INHIBITEUR %s N'EST PAS AFFECTE A UN CATALYSEUR"), *this->GetName()));
			UE_LOG(LogTemp, Warning, TEXT("L'INHIBITEUR %s N'EST PAS AFFECTE A UN CATALYSEUR"), *this->GetName());
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
	MeshObjectif->PlayAnimation(ActivationAnim, false);

	const int TargetIndex = UPopcornFXAttributeFunctions::FindAttributeIndex(LockerFX, "Unlocking");
	UPopcornFXAttributeFunctions::SetAttributeAsBool(LockerFX, TargetIndex, true);

	FMODAudioComp->SetEvent(ActivationSFX);
	FMODAudioComp->Play();

	OwnerCatalyseur->AddInhibiteurToActivatedList(this);

	InteractableComp->Unfeedback();
	InteractableComp->RemoveInteractable(MeshObjectif);

	if(IsValid(CompassIcon)){
		CompassIcon->DestroyComponent();
	}
}

void AInhibiteur::OnSwitchPhases(EPhases CurrentPhase){
	Super::OnSwitchPhases(CurrentPhase);

	if(CurrentPhase == EPhases::TowerDefense){
		DestroyInhibteur();
	}
}

void AInhibiteur::DestroyInhibteur(){
	Destroy();
}

void AInhibiteur::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	if (GameMode->GetPhases() == EPhases::Infiltration && !ActivableComp->IsActivated()){
		ActivableComp->ActivateObject();
	}
}

void AInhibiteur::SetOwnerCatalyseur(ACatalyseur* NewOwner){
	OwnerCatalyseur = NewOwner;
	CompassIcon->SetCompassOwner(OwnerCatalyseur->GetCompass());
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
	if(IsValid(OwnerCatalyseur)){
		UUsefulFunctions::OutlineComponent(bOutline, Cast<UPrimitiveComponent>(OwnerCatalyseur->GetRootComponent()));
		UUsefulFunctions::OutlineComponent(bOutline, OwnerCatalyseur->GetTechMesh());
	}
}

void AInhibiteur::PreSave(const ITargetPlatform* TargetPlatform){
	Super::PreSave(TargetPlatform);

	OutlineLinkedObjects(false);
}
#endif
