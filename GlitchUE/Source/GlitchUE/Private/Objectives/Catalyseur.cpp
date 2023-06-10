// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/Catalyseur.h"
#include "FMODBlueprintStatics.h"
#include "PopcornFXAttributeFunctions.h"
#include "AI/Waves/Spawner.h"
#include "AI/Waves/WaveManager.h"
#include "Components/CompassComponent.h"
#include "Engine/Selection.h"
#include "Helpers/FunctionsLibrary/UsefulFunctions.h"
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

	IdleFX = CreateDefaultSubobject<UPopcornFXEmitterComponent>(TEXT("Idle FX"));
	IdleFX->SetupAttachment(RootComponent);
	IdleFX->SetRelativeLocation(FVector(0, 0, 90));

	static ConstructorHelpers::FObjectFinder<UPopcornFXEffect> CatalyseurFX(TEXT("/Game/VFX/Particles/FX_Environment/Pk_Catalyseur"));
	check(CatalyseurFX.Succeeded());

	IdleFX->SetEffect(CatalyseurFX.Object);

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

	DesactivationBillboard = CreateDefaultSubobject<UWaypoint>(TEXT("Desactivation Billboard"));
	DesactivationBillboard->SetupAttachment(RootComponent);
	DesactivationBillboard->SetRelativeLocation(FVector(0, 0, 50));

	static ConstructorHelpers::FObjectFinder<UMaterial> DesactivationMaterial(TEXT("/Game/Meshs/Materials/Objectives/M_CatalyseurDesactivation"));
	check(DesactivationMaterial.Succeeded());

	DesactivationBillboard->AddElement(DesactivationMaterial.Object, nullptr, true, 0.05f, 0.1f,nullptr);
	DesactivationBillboard->DrawWaypoint(false);

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

	IdleFX->OnEmissionStops.AddDynamic(this, &ACatalyseur::DestroyFX);

	TArray<AActor*> NexusTemp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANexus::StaticClass(), NexusTemp);

	Nexus = Cast<ANexus>(NexusTemp[0]);

	TArray<AActor*> WaveManagerTemp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaveManager::StaticClass(), WaveManagerTemp);

	WaveManager = Cast<AWaveManager>(WaveManagerTemp[0]);

	Player = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	MeshObjectif->PlayAnimation(DesactivationAnim, false);
	TECHMesh->PlayAnimation(DesactivationAnim, false);
	MeshObjectif->SetPosition(1);
	TECHMesh->SetPosition(1);

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

	for (int i = 0; i < ConstructionZoneList.Num(); i++){
		ConstructionZoneList[i]->GetActivableComp()->ActivateObject();
	}

	GameMode->UpdateActivatedCatalyseurAmount();
	UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), ActivationSFX, GetActorTransform(), true);

	switch (GameMode->GetPhases()){
		case EPhases::Infiltration:
			bWasActivatedInStealthPhase = true;

			UPopcornFXAttributeFunctions::SetAttributeAsBool(IdleFX, UPopcornFXAttributeFunctions::FindAttributeIndex(IdleFX, "FreePseudos"), true);

			break;
		case EPhases::TowerDefense:
			DesactivationFX->StopEmitter();

			StartGeneratingMoney();
			Nexus->UpdateDissolver();
			HealthComp->ResetHealth();
			DesactivationBillboard->DrawWaypoint(false);

			InteractableComp->Unfeedback();
			InteractableComp->RemoveInteractable(MeshObjectif);
			InteractableComp->RemoveInteractable(TECHMesh);
			break;
	}
}

void ACatalyseur::DesactivateObjectif(){
	MeshObjectif->PlayAnimation(DesactivationAnim, false);
	TECHMesh->PlayAnimation(DesactivationAnim, false);

	DesactivationFX->StartEmitter();
	const int TargetIndex = UPopcornFXAttributeFunctions::FindAttributeIndex(DesactivationFX, "Color_1");
	UPopcornFXAttributeFunctions::SetAttributeAsLinearColor(DesactivationFX, TargetIndex, CannotInteractWithColor, false);

	for (int i = 0; i < ConstructionZoneList.Num(); i++){
		ConstructionZoneList[i]->GetActivableComp()->DesactivateObject();
	}

	GameMode->UpdateActivatedCatalyseurAmount(false);
	UFMODBlueprintStatics::PlayEvent2D(GetWorld(), DeactivationSFX,true);
	switch (GameMode->GetPhases()){
		case EPhases::Infiltration:
			// not supposed to happen but anyway
			bWasActivatedInStealthPhase = false;
			break;
		case EPhases::TowerDefense:
			GetWorld()->GetTimerManager().ClearTimer(MoneyTimerHandle);
			Nexus->UpdateDissolver();

			DesactivationBillboard->DrawWaypoint(true);
			StartDesactivationTimer(DesactivationTimer);
			break;
	}
}

void ACatalyseur::StartDesactivationTimer(const float Timer){
	GetWorld()->GetTimerManager().SetTimer(DesactivationTimerHandle, [&](){
		InteractableComp->AddInteractable(MeshObjectif);
		InteractableComp->AddInteractable(TECHMesh);

		const int ColorIndex = UPopcornFXAttributeFunctions::FindAttributeIndex(DesactivationFX, "Color_1");
		UPopcornFXAttributeFunctions::SetAttributeAsLinearColor(DesactivationFX, ColorIndex, CanInteractWithColor, false);
	}, Timer, false);
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
		IdleFX->StopEmitter(true);

		if(ActivableComp->IsActivated()){
			StartGeneratingMoney();
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

FCatalyseurData ACatalyseur::SaveCatalyseur(){
	FCatalyseurData Data;

	Data.bIsActivated = GetActivableComp()->IsActivated();
	Data.bWasActivatedInStealthPhase = bWasActivatedInStealthPhase;
	Data.DesactivationTimer = GetWorld()->GetTimerManager().GetTimerRemaining(DesactivationTimerHandle);
	Data.Health = GetHealthComp()->GetCurrentHealth();

	return Data;
}

void ACatalyseur::LoadCatalyseur(const FCatalyseurData NewData){
	if(NewData.bIsActivated){
		GetActivableComp()->ActivateObject();
	}

	else if(bWasActivatedInStealthPhase){
		StartDesactivationTimer(NewData.DesactivationTimer);
	}

	GetHealthComp()->SetHealth(NewData.Health);
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
			UUsefulFunctions::OutlineComponent(bOutline, Cast<UPrimitiveComponent>(ConstructionZoneList[i]->GetRootComponent()));
			UUsefulFunctions::OutlineComponent(bOutline, ConstructionZoneList[i]->GetTechMesh());
		}
	}

	for(int i = 0; i < LinkedInhibiteur.Num(); i++){
		if(IsValid(LinkedInhibiteur[i])){
			UUsefulFunctions::OutlineComponent(bOutline, Cast<UPrimitiveComponent>(LinkedInhibiteur[i]->GetRootComponent()));
		}
	}
}

void ACatalyseur::PreSave(const ITargetPlatform* TargetPlatform){
	Super::PreSave(TargetPlatform);

	OutlineLinkedObjects(false);
}
#endif
