// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/Catalyseur.h"
#include "PaperSpriteComponent.h"
#include "AI/Waves/Spawner.h"
#include "AI/Waves/WaveManager.h"
#include "Engine/Selection.h"
#include "Helpers/FunctionsLibrary/UsefullFunctions.h"
#include "Objectives/Inhibiteur.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MainPlayer.h"
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

ACatalyseur::ACatalyseur(){
	TECHMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TECHMesh"));
	TECHMesh->SetCanEverAffectNavigation(false);
	TECHMesh->SetupAttachment(RootComponent);

	CatalyeurZone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CatalyseurZone"));
	CatalyeurZone->SetupAttachment(RootComponent);
	CatalyeurZone->SetCollisionResponseToAllChannels(ECR_Ignore);
	CatalyeurZone->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	CatalyeurZone->OnComponentBeginOverlap.AddDynamic(this, &ACatalyseur::EnterCatalyseurZone);
	CatalyeurZone->OnComponentEndOverlap.AddDynamic(this, &ACatalyseur::ExitCatalyseurZone);

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> ActivAnim(TEXT("/Game/Meshs/Objectives/Catalyseur/AS_MED_Catalyser_Open"));
	check(ActivAnim.Succeeded());

	ActivationAnim = ActivAnim.Object;

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> DesactivAnim(TEXT("/Game/Meshs/Objectives/Catalyseur/AS_Tech_Catalyser_Close"));
	check(DesactivAnim.Succeeded());

	DesactivationAnim = DesactivAnim.Object;

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

	GenerateCompass();

	GameMode->OnSwitchPhases.AddDynamic(this, &ACatalyseur::OnSwitchPhases);

	Player = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void ACatalyseur::ActiveObjectif(){
	MeshObjectif->PlayAnimation(ActivationAnim, false);
	TECHMesh->PlayAnimation(ActivationAnim, false);

	GameMode->UpdateActivatedCatalyseurAmount();

	switch (GameMode->GetPhases()){
		case EPhases::Infiltration:
			break;
		case EPhases::TowerDefense:
			StartGeneratingMoney();
			Nexus->UpdateDissolver();
			break;
	}
}

void ACatalyseur::DesactivateObjectif(){
	MeshObjectif->PlayAnimation(DesactivationAnim, false);
	TECHMesh->PlayAnimation(DesactivationAnim, false);

	GameMode->UpdateActivatedCatalyseurAmount(false);

	switch (GameMode->GetPhases()){
		case EPhases::Infiltration:
			break;
		case EPhases::TowerDefense:
			GetWorld()->GetTimerManager().ClearTimer(MoneyTimerHandle);
			Nexus->UpdateDissolver();
			break;
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

void ACatalyseur::OnConstruction(const FTransform& Transform){
	Super::OnConstruction(Transform);

	for(int i = 0; i < LinkedInhibiteur.Num(); i++){
		if(IsValid(LinkedInhibiteur[i])){
			LinkedInhibiteur[i]->SetOwnerCatalyseur(this);
		}
	}
}

void ACatalyseur::OnSwitchPhases(EPhases CurrentPhase){
	switch (CurrentPhase){
	case EPhases::Infiltration:
		break;
	case EPhases::TowerDefense:
		DeleteCompass();

		if(ActivableComp->IsActivated()){
			StartGeneratingMoney();
		}

		break;
	}
}

void ACatalyseur::HealthNull(){
	ActivableComp->DesactivateObject();
}

void ACatalyseur::GenerateCompass(){
	const FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules::KeepWorldTransform;

	const FVector ActorOffset = FVector(0, 0, 80);

	const FTransform SpriteTransform = FTransform(FRotator(0, 90, 0), FVector(CompassRadius, 0, 0), FVector::OneVector);

	for(int i = 0; i < LinkedInhibiteur.Num(); i++){
		USceneComponent* CurrentSceneComp = Cast<USceneComponent>(AddComponentByClass(USceneComponent::StaticClass(), false, FTransform::Identity, false));

		CurrentSceneComp->AttachToComponent(MeshObjectif, AttachmentTransformRules);

		CurrentSceneComp->SetRelativeLocation(ActorOffset);

		UPaperSpriteComponent* CurrentIconComp = Cast<UPaperSpriteComponent>(AddComponentByClass(UPaperSpriteComponent::StaticClass(), false, FTransform::Identity, false));

		CurrentIconComp->AttachToComponent(CurrentSceneComp, AttachmentTransformRules);

		CurrentIconComp->SetSprite(InhibiteurSprite);
		CurrentIconComp->SetRelativeTransform(SpriteTransform);
		CurrentIconComp->SetCollisionResponseToAllChannels(ECR_Ignore);

		CurrentSceneComp->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(CurrentSceneComp->GetComponentLocation(), LinkedInhibiteur[i]->GetActorLocation()));

		const FCompassSprite NewCompassSprite = FCompassSprite(CurrentSceneComp, CurrentIconComp);
		CompassSpriteList.Add(NewCompassSprite);

		LinkedInhibiteur[i]->SetSpriteReference(CompassSpriteList[i]);
	}
}

void ACatalyseur::DeleteCompass(){
	for(int i = 0; i < CompassSpriteList.Num(); i++){
		CompassSpriteList[i].DestroyComponents();
	}

	CompassSpriteList.Empty();
}

void ACatalyseur::EnterCatalyseurZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if(OtherActor->IsA(AMainPlayer::StaticClass())){
		Cast<AMainPlayer>(OtherActor)->SetInGlitchZone(true);

		if(GameMode->GetLevelState() == ELevelState::Alerted){
			GetWorld()->GetTimerManager().SetTimer(CatalyeurZoneTimer, [&]() {
				GameMode->SetLevelState(ELevelState::Normal);
			}, ResetLevelStateDuration, false);
		}
	}
}

void ACatalyseur::ExitCatalyseurZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
	if(OtherActor->IsA(AMainPlayer::StaticClass())){
		Cast<AMainPlayer>(OtherActor)->SetInGlitchZone(false);

		GetWorld()->GetTimerManager().ClearTimer(CatalyeurZoneTimer);
	}
}

void ACatalyseur::GenerateMoney(){
	Player->UpdateGolds(GeneratedGolds * ActivatedInhibiteurs, EGoldsUpdateMethod::ReceiveGolds);
}

void ACatalyseur::StartGeneratingMoney(){
	GetWorld()->GetTimerManager().SetTimer(MoneyTimerHandle, this, &ACatalyseur::GenerateMoney, GoldsTick, true);
}

void ACatalyseur::UpdateActivatedInhibiteurs(const bool Increase){
	Increase ? ActivatedInhibiteurs++ : ActivatedInhibiteurs--;

	if(ActivatedInhibiteurs == LinkedInhibiteur.Num()){
		Player->UpdateGolds(GoldsBonus, EGoldsUpdateMethod::ReceiveGolds);
	}
}

#if WITH_EDITORONLY_DATA
void ACatalyseur::PreEditChange(FProperty* PropertyAboutToChange){
	Super::PreEditChange(PropertyAboutToChange);

	OutlineLinkedObjects(false);

	for(int i = 0; i < LinkedInhibiteur.Num(); i++){
		if(IsValid(LinkedInhibiteur[i])){
			LinkedInhibiteur[i]->SetOwnerCatalyseur(nullptr);
		}
	}
}

void ACatalyseur::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent){
	Super::PostEditChangeProperty(PropertyChangedEvent);

	OutlineLinkedObjects(true);

	for(int i = 0; i < LinkedInhibiteur.Num(); i++){
		if(IsValid(LinkedInhibiteur[i])){
			LinkedInhibiteur[i]->SetOwnerCatalyseur(this);
		}
	}
}


void ACatalyseur::OnObjectSelected(UObject* Object){
	if(Object == this){

		OutlineLinkedObjects(true);

	} else if (!IsSelected()){

		OutlineLinkedObjects(false);
	}
}

void ACatalyseur::OutlineLinkedObjects(const bool bOutline){
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
