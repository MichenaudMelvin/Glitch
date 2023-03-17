// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/Catalyseur.h"
#include "PaperSpriteComponent.h"
#include "AI/Waves/Spawner.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ACatalyseur::ACatalyseur() {
	TECHMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TECHMesh"));
	TECHMesh->SetCanEverAffectNavigation(false);
	TECHMesh->SetupAttachment(RootComponent);
}

void ACatalyseur::BeginPlay() {
	Super::BeginPlay();

	TArray<AActor*> NexusTemp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANexus::StaticClass(), NexusTemp);

	Nexus = Cast<ANexus>(NexusTemp[0]);

	if (StateAtWave.EnableAtWave == 0) {
		UE_LOG(LogTemp, Fatal, TEXT("LE CATALYSEUR %s NE COMMENCE A AUCUNE VAGUE"), *this->GetName());
	}

	if (StateAtWave.DisableAtWave == 0) {
		UE_LOG(LogTemp, Fatal, TEXT("LE CATALYSEUR %s NE TERMINE A AUCUNE VAGUE"), *this->GetName());
	}

	GenerateCompass();
}

void ACatalyseur::ActiveObjectif(){
	if (Nexus->GetActivableComp()->IsActivated()) {
		for (int i = 0; i < ConstructionZoneList.Num(); i++) {
			ConstructionZoneList[i]->GetActivableComp()->ActivateObject();
		}
	}
}

void ACatalyseur::DesactivateObjectif() {
	for (int i = 0; i < ConstructionZoneList.Num(); i++) {
		ConstructionZoneList[i]->GetActivableComp()->DesactivateObject();
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
		CurrentIconComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

		CurrentSceneComp->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(CurrentSceneComp->GetComponentLocation(), NearInhibiteur[i]->GetActorLocation()));

		SceneComponents.Add(CurrentSceneComp);
		PaperSpriteComponents.Add(CurrentIconComp);
	}
}

void ACatalyseur::DeleteCompass(){
	for(int i = 0; i < SceneComponents.Num(); i++){
		SceneComponents[i]->DestroyComponent();
		PaperSpriteComponents[i]->DestroyComponent();
	}
	
	SceneComponents.Empty();
	PaperSpriteComponents.Empty();
}

FStateAtWave ACatalyseur::GetStateAtWave() const{
	return StateAtWave;
}
