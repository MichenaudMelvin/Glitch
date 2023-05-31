// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/Catalyseur.h"
#include "FMODBlueprintStatics.h"
#include "PopcornFXAttributeFunctions.h"
#include "AI/Waves/Spawner.h"
#include "AI/Waves/WaveManager.h"
#include "Components/CompassComponent.h"
#include "Engine/Selection.h"
#include "Helpers/FunctionsLibrary/UsefullFunctions.h"
#include "Objectives/Inhibiteur.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MainPlayer.h"
#include "PlacableObject/ConstructionZone.h"
#include "Kismet/KismetMathLibrary.h"

ACatalyseur::ACatalyseur(){
	TECHMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TECHMesh"));
	TECHMesh->SetCanEverAffectNavigation(false);
	TECHMesh->SetupAttachment(RootComponent);

	DesactivationFX = CreateDefaultSubobject<UPopcornFXEmitterComponent>(TEXT("Desactivation FX"));
	DesactivationFX->SetupAttachment(RootComponent);
	DesactivationFX->bPlayOnLoad = false;

	static ConstructorHelpers::FObjectFinder<UPopcornFXEffect> ShutDownFX(TEXT("/Game/VFX/Particles/FX_Environment/Pk_DeactivatingEffect_Generator"));
	check(ShutDownFX.Succeeded());

	DesactivationFX->SetEffect(ShutDownFX.Object);

	GoldsGenerationFX = CreateDefaultSubobject<UPopcornFXEmitterComponent>(TEXT("Gold Generation FX"));
	GoldsGenerationFX->SetupAttachment(RootComponent);
	GoldsGenerationFX->bPlayOnLoad = false;

	static ConstructorHelpers::FObjectFinder<UPopcornFXEffect> GoldsFX(TEXT("/Game/VFX/Particles/FX_Environment/Pk_GoldGeneration"));
	check(GoldsFX.Succeeded());

	GoldsGenerationFX->SetEffect(GoldsFX.Object);

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> ActivAnim(TEXT("/Game/Meshs/Objectives/Catalyseur/AS_MED_Catalyser_Open"));
	check(ActivAnim.Succeeded());

	ActivationAnim = ActivAnim.Object;

	static ConstructorHelpers::FObjectFinder<UFMODEvent> SFXDeactivation(TEXT("/Game/FMOD/Events/SFX/SFX_generator_deactivation"));
	check(SFXDeactivation.Succeeded());

	DeactivationSFX = SFXDeactivation.Object;

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> DesactivAnim(TEXT("/Game/Meshs/Objectives/Catalyseur/AS_Tech_Catalyser_Close"));
	check(DesactivAnim.Succeeded());

	DesactivationAnim = DesactivAnim.Object;

	NavModifier->SetBoxExtent(FVector(150, 150, 10));

	Compass = CreateDefaultSubobject<UCompassComponent>(TEXT("Compass"));
	Compass->SetCompassOffset(FVector(0, 0, 50));

	#if WITH_EDITORONLY_DATA
		USelection::SelectObjectEvent.AddUObject(this, &ACatalyseur::OnObjectSelected);
		USelection::SelectionChangedEvent.AddUObject(this, &ACatalyseur::OnObjectSelected);
	#endif
}

USkeletalMeshComponent* ACatalyseur::GetTechMesh() const{
	return TECHMesh;
}

void ACatalyseur::BeginPlay(){
	Super::BeginPlay();

	InteractableComp->AddInteractable(TECHMesh);

	TArray<AActor*> NexusTemp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANexus::StaticClass(), NexusTemp);

	Nexus = Cast<ANexus>(NexusTemp[0]);

	TArray<AActor*> WaveManagerTemp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaveManager::StaticClass(), WaveManagerTemp);

	WaveManager = Cast<AWaveManager>(WaveManagerTemp[0]);

	Player = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	for(int i = 0; i < LinkedInhibiteur.Num(); i++){

		#if WITH_EDITOR
		if(!IsValid(LinkedInhibiteur[i])){
			UE_LOG(LogTemp, Warning, TEXT("LE CATALYSEUR %s A UN EMPLACEMENT VIDE D'INHIBITEUR"), *this->GetName());
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("LE CATALYSEUR %s A UN EMPLACEMENT VIDE D'INHIBITEUR"), *this->GetName()));
			continue;
		}
		#endif

		LinkedInhibiteur[i]->SetOwnerCatalyseur(this);
	}

#if WITH_EDITOR
	if(ConstructionZoneList.Num() == 0){
		UE_LOG(LogTemp, Warning, TEXT("LE CATALYSEUR %s N'AFFECTE AUCUNE ZONE DE CONSTRUCTION"), *this->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("LE CATALYSEUR %s N'AFFECTE AUCUNE ZONE DE CONSTRUCTION"), *this->GetName()));
	}
#endif
}

void ACatalyseur::Destroyed(){
	Super::Destroyed();

#if WITH_EDITORONLY_DATA
	OutlineLinkedObjects(false);
#endif
}

void ACatalyseur::ActiveObjectif(){
	MeshObjectif->PlayAnimation(ActivationAnim, false);
	TECHMesh->PlayAnimation(ActivationAnim, false);

	DesactivationFX->StopEmitter();

	for (int i = 0; i < ConstructionZoneList.Num(); i++){
		ConstructionZoneList[i]->GetActivableComp()->ActivateObject();
	}

	GameMode->UpdateActivatedCatalyseurAmount();
	UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), ActivationSFX, GetActorTransform(), true);

	switch (GameMode->GetPhases()){
		case EPhases::Infiltration:
			break;
		case EPhases::TowerDefense:
			StartGeneratingMoney();
			Nexus->UpdateDissolver();
			HealthComp->ResetHealth();
			ToggleActivatedInhibiteursState(true);
			break;
	}
}

void ACatalyseur::DesactivateObjectif(){
	MeshObjectif->PlayAnimation(DesactivationAnim, false);
	TECHMesh->PlayAnimation(DesactivationAnim, false);

	DesactivationFX->StartEmitter();
	const int TargetIndex = UPopcornFXAttributeFunctions::FindAttributeIndex(DesactivationFX, "Color_1");
	UPopcornFXAttributeFunctions::SetAttributeAsLinearColor(DesactivationFX, TargetIndex, CannotInteractWithColor, true);

	for (int i = 0; i < ConstructionZoneList.Num(); i++){
		ConstructionZoneList[i]->GetActivableComp()->DesactivateObject();
	}

	GameMode->UpdateActivatedCatalyseurAmount(false);
	UFMODBlueprintStatics::PlayEvent2D(GetWorld(), DeactivationSFX,true);
	switch (GameMode->GetPhases()){
		case EPhases::Infiltration:
			break;
		case EPhases::TowerDefense:
			GetWorld()->GetTimerManager().ClearTimer(MoneyTimerHandle);
			Nexus->UpdateDissolver();

			InteractableComp->RemoveInteractable(MeshObjectif);
			InteractableComp->RemoveInteractable(TECHMesh);

			GetWorld()->GetTimerManager().SetTimer(DesactivationTimerHandle, [&](){
				InteractableComp->AddInteractable(MeshObjectif);
				InteractableComp->AddInteractable(TECHMesh);

				const int TargetIndex = UPopcornFXAttributeFunctions::FindAttributeIndex(DesactivationFX, "Color_1");
				UPopcornFXAttributeFunctions::SetAttributeAsLinearColor(DesactivationFX, TargetIndex, CanInteractWithColor, true);
			}, DesactivationTimer, false);
			break;
	}
}

void ACatalyseur::ToggleActivatedInhibiteursState(const bool ActivateInhibiteurs){
	for(int i = 0; i < ActivatedInhibiteursList.Num(); i++){
		ActivateInhibiteurs ? ActivatedInhibiteursList[i]->GetActivableComp()->ActivateObject() : ActivatedInhibiteursList[i]->GetActivableComp()->DesactivateObject();
	}
}

void ACatalyseur::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	Super::Interact(MainPlayerController, MainPlayer);

	if(!ActivableComp->IsActivated() && GameMode->GetPhases() == EPhases::TowerDefense){
		ActivableComp->ActivateObject();
		return;
	}

	if(ActivableComp->IsActivated() && WaveManager->IsStopped()){
		WaveManager->NextWave();
	}
}

void ACatalyseur::OnSwitchPhases(EPhases CurrentPhase){
	Super::OnSwitchPhases(CurrentPhase);

	switch (CurrentPhase){
	case EPhases::Infiltration:
		break;
	case EPhases::TowerDefense:
		Compass->DestroyComponent();

		if(ActivableComp->IsActivated()){
			StartGeneratingMoney();
		} else{
			// for now it work, needs to be tested with saves
			InteractableComp->RemoveInteractable(MeshObjectif);
			InteractableComp->RemoveInteractable(TECHMesh);
		}

		break;
	}
}

void ACatalyseur::HealthNull(){
	ActivableComp->DesactivateObject();
}

void ACatalyseur::GenerateMoney(){
	Player->UpdateGolds(GeneratedGolds * ActivatedInhibiteursList.Num(), EGoldsUpdateMethod::ReceiveGolds);
	GoldsGenerationFX->StartEmitter();
}

void ACatalyseur::StartGeneratingMoney(){
	GetWorld()->GetTimerManager().SetTimer(MoneyTimerHandle, this, &ACatalyseur::GenerateMoney, GoldsTick, true);
}

void ACatalyseur::AddInhibiteurToActivatedList(AInhibiteur* InhibiteurToAdd){
	ActivatedInhibiteursList.Add(InhibiteurToAdd);

	if(ActivatedInhibiteursList.Num() == LinkedInhibiteur.Num()){
		ActivableComp->ActivateObject();
	}
}

UCompassComponent* ACatalyseur::GetCompass() const{
	return Compass;
}

#if WITH_EDITORONLY_DATA
void ACatalyseur::PreEditChange(FProperty* PropertyAboutToChange){
	Super::PreEditChange(PropertyAboutToChange);

	OutlineLinkedObjects(false);
}

void ACatalyseur::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent){
	Super::PostEditChangeProperty(PropertyChangedEvent);

	OutlineLinkedObjects(true);
}


void ACatalyseur::OnObjectSelected(UObject* Object){
	if(Object == this){

		OutlineLinkedObjects(true);

	} else if (!IsSelected()){

		OutlineLinkedObjects(false);
	}
}

void ACatalyseur::OutlineLinkedObjects(const bool bOutline){
	for(int i = 0; i < ConstructionZoneList.Num(); i++){
		if(IsValid(ConstructionZoneList[i])){
			UUsefullFunctions::OutlineComponent(bOutline, Cast<UPrimitiveComponent>(ConstructionZoneList[i]->GetRootComponent()));
			UUsefullFunctions::OutlineComponent(bOutline, ConstructionZoneList[i]->GetTechMesh());
		}
	}

	for(int i = 0; i < LinkedInhibiteur.Num(); i++){
		if(IsValid(LinkedInhibiteur[i])){
			UUsefullFunctions::OutlineComponent(bOutline, Cast<UPrimitiveComponent>(LinkedInhibiteur[i]->GetRootComponent()));
		}
	}
}

void ACatalyseur::PreSave(const ITargetPlatform* TargetPlatform){
	Super::PreSave(TargetPlatform);

	OutlineLinkedObjects(false);
}
#endif
