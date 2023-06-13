// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelElements/BanLog.h"

#include "Kismet/GameplayStatics.h"

ABanLog::ABanLog(){
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	SetRootComponent(SceneComp);

	SceneComp->SetMobility(EComponentMobility::Static);

	MedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Med Mesh"));
	MedMesh->SetupAttachment(SceneComp);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshMed(TEXT("/Game/Meshs/Narrative_Ressources/SM_Tree_MED"));
	check(MeshMed.Succeeded());

	MedMeshObject = MeshMed.Object;
	MedMesh->SetStaticMesh(MedMeshObject);
	MedMesh->SetGenerateOverlapEvents(false);

	MedMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	MedMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	MedMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	MedMesh->SetMobility(EComponentMobility::Static);

	TechMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tech Mesh"));
	TechMesh->SetupAttachment(SceneComp);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshTech(TEXT("/Game/Meshs/Narrative_Ressources/SM_Tree_TECH"));
	check(MeshTech.Succeeded());

	TechMeshObject = MeshTech.Object;
	TechMesh->SetStaticMesh(TechMeshObject);
	TechMesh->SetGenerateOverlapEvents(false);

	TechMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	TechMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	TechMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	TechMesh->SetMobility(EComponentMobility::Static);

	InteractableComp = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interacable Comp"));
	ActivableComp = CreateDefaultSubobject<UActivableComponent>(TEXT("Activable Comp"));
}

void ABanLog::BeginPlay(){
	Super::BeginPlay();

	InteractableComp->AddInteractable(MedMesh);
	InteractableComp->AddInteractable(TechMesh);
	InteractableComp->OnInteract.AddDynamic(this, &ABanLog::Interact);

	ActivableComp->OnActivated.AddDynamic(this, &ABanLog::ActivateBanLog);

#if WITH_EDITOR
	if(TchatMessageList.Num() == 0){
		GEngine->AddOnScreenDebugMessage(-1, 9999999.0f, FColor::Yellow,FString::Printf(TEXT("LA MESSAGE LIST DE %s EST VIDE"), *GetName()));
		UE_LOG(LogTemp, Warning, TEXT("LA MESSAGE LIST DE %s EST VIDE"), *GetName());
	}
#endif
}

void ABanLog::OnConstruction(const FTransform& Transform){
	Super::OnConstruction(Transform);

	MedMesh->SetStaticMesh(MedMeshObject);
	TechMesh->SetStaticMesh(TechMeshObject);
}

void ABanLog::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	CurrenController = MainPlayerController;

	ActivableComp->ActivateObject();
}

void ABanLog::ActivateBanLog(){
	WriteMessages();

	RemoveInteraction();
}

void ABanLog::WriteMessages(){
	CurrenController->GetTchatWidget()->AddTchatLine(TchatMessageList[Index].Speaker, TchatMessageList[Index].TextMessage, TchatMessageList[Index].SpeakerColor);

	Index++;

	if(Index + 1 > TchatMessageList.Num()){
		return;
	}

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABanLog::WriteMessages, TchatMessageList[Index - 1].DelayForNextMessage, false);
}

bool ABanLog::IsActivated() const{
	return ActivableComp->IsActivated();
}

void ABanLog::RemoveInteraction() const{
	InteractableComp->Unfeedback();
	InteractableComp->RemoveInteractable(MedMesh);
	InteractableComp->RemoveInteractable(TechMesh);
	InteractableComp->OnInteract.RemoveDynamic(this, &ABanLog::Interact);

	ActivableComp->OnActivated.Clear();
	ActivableComp->ActivateObject();
}

