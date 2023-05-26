// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/Inhibiteur.h"
#include "Gamemodes/GlitchUEGameMode.h"
#include "Engine/Selection.h"
#include "Helpers/FunctionsLibrary/UsefullFunctions.h"
#include "Components/CompassComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FMODBlueprintStatics.h"
#include "PopcornFXAttributeFunctions.h"

AInhibiteur::AInhibiteur(){
	MeshObjectif->SetCanEverAffectNavigation(true);

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> ActivAnim(TEXT("/Game/Meshs/Objectives/Inhibiteur/AS_Inhibiteur"));
	check(ActivAnim.Succeeded());

	static ConstructorHelpers::FObjectFinder<UFMODEvent> SFX(TEXT("/Game/FMOD/Events/SFX/SFX_Free_Interaction"));
	check(SFX.Succeeded());

	ActivationSFX = SFX.Object;

	ActivationAnim = ActivAnim.Object;

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

#if WITH_EDITOR

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
	MeshObjectif->PlayAnimation(ActivationAnim, false);

	const int TargetIndex = UPopcornFXAttributeFunctions::FindAttributeIndex(LockerFX, "Unlocking");
	UPopcornFXAttributeFunctions::SetAttributeAsBool(LockerFX, TargetIndex, true);

	UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), ActivationSFX, GetActorTransform(), true);

	OwnerCatalyseur->AddInhibiteurToActivatedList(this);
	CompassIcon->DestroyComponent();
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
		UUsefullFunctions::OutlineComponent(bOutline, Cast<UPrimitiveComponent>(OwnerCatalyseur->GetRootComponent()));
		UUsefullFunctions::OutlineComponent(bOutline, OwnerCatalyseur->GetTechMesh());
	}
}

void AInhibiteur::PreSave(const ITargetPlatform* TargetPlatform){
	Super::PreSave(TargetPlatform);

	OutlineLinkedObjects(false);
}
#endif
