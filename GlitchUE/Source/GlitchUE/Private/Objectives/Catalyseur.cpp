// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/Catalyseur.h"
#include "PaperSpriteComponent.h"
#include "AI/Waves/Spawner.h"
#include "AI/Waves/WaveManager.h"
#include "Engine/Selection.h"
#include "Helpers/FunctionsLibrary/UsefullFunctions.h"
#include "Objectives/Inhibiteur.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

FCompassSprite::FCompassSprite(){
	SceneComponent = nullptr;
	PaperSpriteComponent = nullptr;
}

FCompassSprite::FCompassSprite(USceneComponent* SceneComp, UPaperSpriteComponent* PaperSpriteComp){
	SceneComponent = SceneComp;
	PaperSpriteComponent = PaperSpriteComp;
}

void FCompassSprite::DestroyComponents(){
	SceneComponent->DestroyComponent();
	PaperSpriteComponent->DestroyComponent();
}

ACatalyseur::ACatalyseur() {
	TECHMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TECHMesh"));
	TECHMesh->SetCanEverAffectNavigation(false);
	TECHMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> ActivAnim(TEXT("/Game/Meshs/Objectives/Catalyseur/AS_MED_Catalyser_Open"));
	check(ActivAnim.Succeeded());

	ActivationAnim = ActivAnim.Object;

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> DesactivAnim(TEXT("/Game/Meshs/Objectives/Catalyseur/AS_Tech_Catalyser_Close"));
	check(DesactivAnim.Succeeded());

	DesactivationAnim = DesactivAnim.Object;

	#if WITH_EDITORONLY_DATA
		USelection::SelectObjectEvent.AddUObject(this, &ACatalyseur::OnObjectSelected);
	#endif
}

void ACatalyseur::BeginPlay() {
	Super::BeginPlay();

	TArray<AActor*> NexusTemp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANexus::StaticClass(), NexusTemp);

	Nexus = Cast<ANexus>(NexusTemp[0]);

	TArray<AActor*> WaveManagerTemp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaveManager::StaticClass(), WaveManagerTemp);

	WaveManager = Cast<AWaveManager>(WaveManagerTemp[0]);

#if !UE_BUILD_SHIPPING

	if (StateAtWave.EnableAtWave == 0) {
		UE_LOG(LogTemp, Fatal, TEXT("LE CATALYSEUR %s NE COMMENCE A AUCUNE VAGUE"), *this->GetName());
	}

	if (StateAtWave.DisableAtWave == 0) {
		UE_LOG(LogTemp, Fatal, TEXT("LE CATALYSEUR %s NE TERMINE A AUCUNE VAGUE"), *this->GetName());
	}

#endif

	GenerateCompass();
}

void ACatalyseur::ActiveObjectif(){
	if (Nexus->GetActivableComp()->IsActivated()) {
		for (int i = 0; i < ConstructionZoneList.Num(); i++) {
			ConstructionZoneList[i]->GetActivableComp()->ActivateObject();
		}

		DeleteCompass();

		MeshObjectif->PlayAnimation(ActivationAnim, false);
		TECHMesh->PlayAnimation(ActivationAnim, false);
	}
}

void ACatalyseur::DesactivateObjectif() {
	for (int i = 0; i < ConstructionZoneList.Num(); i++) {
		ConstructionZoneList[i]->GetActivableComp()->DesactivateObject();
	}

	MeshObjectif->PlayAnimation(DesactivationAnim, false);
	TECHMesh->PlayAnimation(DesactivationAnim, false);
}

void ACatalyseur::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	Super::Interact(MainPlayerController, MainPlayer);

	if(ActivableComp->IsActivated() && WaveManager->IsStopped()){
		WaveManager->NextWave();
	}
}

void ACatalyseur::HealthNull(){
	ActivableComp->DesactivateObject();
}

void ACatalyseur::GenerateCompass(){
	const FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules::KeepWorldTransform;

	const FVector ActorOffset = FVector(0, 0, 80);

	const FTransform SpriteTransform = FTransform(FRotator(0, 90, 0), FVector(CompassRadius, 0, 0), FVector::OneVector);

	for(int i = 0; i < NearInhibiteur.Num(); i++){
		USceneComponent* CurrentSceneComp = Cast<USceneComponent>(AddComponentByClass(USceneComponent::StaticClass(), false, FTransform::Identity, false));

		CurrentSceneComp->AttachToComponent(MeshObjectif, AttachmentTransformRules);

		CurrentSceneComp->SetRelativeLocation(ActorOffset);

		UPaperSpriteComponent* CurrentIconComp = Cast<UPaperSpriteComponent>(AddComponentByClass(UPaperSpriteComponent::StaticClass(), false, FTransform::Identity, false));

		CurrentIconComp->AttachToComponent(CurrentSceneComp, AttachmentTransformRules);

		CurrentIconComp->SetSprite(InhibiteurSprite);
		CurrentIconComp->SetRelativeTransform(SpriteTransform);
		CurrentIconComp->SetCollisionResponseToAllChannels(ECR_Ignore);

		CurrentSceneComp->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(CurrentSceneComp->GetComponentLocation(), NearInhibiteur[i]->GetActorLocation()));

		const FCompassSprite NewCompassSprite = FCompassSprite(CurrentSceneComp, CurrentIconComp);
		CompassSpriteList.Add(NewCompassSprite);

		NearInhibiteur[i]->SetSpriteReference(CompassSpriteList[i]);
	}
}

void ACatalyseur::DeleteCompass(){
	for(int i = 0; i < CompassSpriteList.Num(); i++){
		CompassSpriteList[i].DestroyComponents();
	}

	CompassSpriteList.Empty();
}

FStateAtWave ACatalyseur::GetStateAtWave() const{
	return StateAtWave;
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
	if (Object == this) {

		OutlineLinkedObjects(true);

	} else if (!IsSelected()){

		OutlineLinkedObjects(false);
	}
}

void ACatalyseur::OutlineLinkedObjects(const bool bOutline){
	for(int i = 0; i < NearInhibiteur.Num(); i++){
		if(IsValid(NearInhibiteur[i])){
			UUsefullFunctions::OutlineComponent(bOutline, Cast<UPrimitiveComponent>(NearInhibiteur[i]->GetRootComponent()));
		}
	}

	for(int i = 0; i < ConstructionZoneList.Num(); i++){
		if(IsValid(ConstructionZoneList[i])){
			UUsefullFunctions::OutlineComponent(bOutline, Cast<UPrimitiveComponent>(ConstructionZoneList[i]->GetRootComponent()));
			UUsefullFunctions::OutlineComponent(bOutline, ConstructionZoneList[i]->GetTechMesh());
		}
	}
}
#endif
