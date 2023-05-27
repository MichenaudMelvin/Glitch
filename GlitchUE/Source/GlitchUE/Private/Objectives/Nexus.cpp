// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/Nexus.h"
#include "Kismet/GameplayStatics.h"
#include "PopcornFXAttributeFunctions.h"
#include "PopcornFXFunctions.h"
#include "Helpers/FunctionsLibrary/UsefullFunctions.h"
#include "Objectives/Catalyseur.h"
#include "Audio/AudioManager.h"
#include "FX/Dissolver.h"
#include "FMODBlueprintStatics.h"

ANexus::ANexus() {
	static ConstructorHelpers::FObjectFinder<UPopcornFXEffect> TechEffect(TEXT("/Game/VFX/Particles/FX_Environment/Pk_NexusFromTechToMed"));
	check(TechEffect.Succeeded());

	TechFX = TechEffect.Object;

	static ConstructorHelpers::FObjectFinder<UPopcornFXEffect> MedEffect(TEXT("/Game/VFX/Particles/FX_Environment/Pk_NexusMED"));
	check(MedEffect.Succeeded());

	MedFX = MedEffect.Object;

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> Idle(TEXT("/Game/Meshs/Objectives/Nexus/AS_Nexus"));
	check(Idle.Succeeded());

	IdleAnim = Idle.Object;
}

void ANexus::BeginPlay(){
	Super::BeginPlay();

	TArray<AActor*> DissolverArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADissolver::StaticClass(), DissolverArray);

	Dissolver = Cast<ADissolver>(DissolverArray[0]);
	Dissolver->SetActorLocation(GetActorLocation());
	Dissolver->UpdateShaderFX();

	MeshObjectif->PlayAnimation(IdleAnim, true);

	TArray<AActor*> CatalyseurArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACatalyseur::StaticClass(), CatalyseurArray);

	for(int i = 0; i < CatalyseurArray.Num(); i++){
		CatalyseursList.Add(Cast<ACatalyseur>(CatalyseurArray[i]));
	}

	TArray<AActor*> AudioManagerArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAudioManager::StaticClass(), AudioManagerArray);

#if WITH_EDITOR

	if(AudioManagerArray.Num() == 0){
		UE_LOG(LogTemp, Fatal, TEXT("AUCUN AUDIO MANAGER N'EST PLACE DANS LA SCENE"));
	}

#endif

	AudioManager = Cast<AAudioManager>(AudioManagerArray[0]);

	AMainPlayer* MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if(IsValid(MainPlayer)){
		HealthComp->OnHealthNull.AddDynamic(MainPlayer, &AMainPlayer::Loose);
	}

	FVector SpawnFXLocation = GetActorLocation();
	SpawnFXLocation.Z += 200;

	TechFXEmitter = UPopcornFXFunctions::SpawnEmitterAtLocation(GetWorld(), TechFX, "PopcornFX_DefaultScene", SpawnFXLocation, FRotator::ZeroRotator, true, false);
	MedFXEmitter = UPopcornFXFunctions::SpawnEmitterAtLocation(GetWorld(), MedFX, "PopcornFX_DefaultScene", SpawnFXLocation, FRotator::ZeroRotator, false, false);
}

void ANexus::OnConstruction(const FTransform& Transform){
	Super::OnConstruction(Transform);

	TArray<AActor*> DissolverArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADissolver::StaticClass(), DissolverArray);

	if(DissolverArray.Num() != 1){
		return;
	}


	ADissolver* DissolveActor = Cast<ADissolver>(DissolverArray[0]);

	DissolveActor->SetActorLocation(GetActorLocation());
	DissolveActor->UpdateShaderFX();
}

void ANexus::TakeDamages(){
	Super::TakeDamages();

	// nexus health in percent
	const float NexusHealth = (HealthComp->GetCurrentHealth() * HealthComp->GetMaxHealth()) / 100.0f;

	Dissolver->DissolveTo(NexusHealth * Dissolver->GetRadius() / 100.0f);
}

void ANexus::UpdateDissolver(){
	const float CatalyseurCompletionPercent = 100.0f/CatalyseursList.Num() * GameMode->GetActivatedCatalyseurNum();

	Dissolver->DissolveTo(CatalyseurCompletionPercent * Dissolver->GetMaxRadius() / 100.0f);
}

AActor* ANexus::GetFarestActivatedCatalyseur(){
	TArray<AActor*> ActivatedCatalyseurList;

	for(int i = 0; i < CatalyseursList.Num(); i++){
		if(CatalyseursList[i]->GetActivableComp()->IsActivated()){
			ActivatedCatalyseurList.Add(CatalyseursList[i]);
		}
	}

	TArray<AActor*> SortedCatalyseurs = UUsefullFunctions::SortActorsByDistanceToActor(ActivatedCatalyseurList, this);

	return SortedCatalyseurs[SortedCatalyseurs.Num() - 1];
}

void ANexus::ActiveObjectif(){
	GameMode->SetNewPhase(EPhases::TowerDefense);

	UpdateDissolver();

	UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), ActivationSFX, GetActorTransform(), true);

	AudioManager->SwitchToTowerDefenseMusic();

	const int Index = UPopcornFXAttributeFunctions::FindAttributeIndex(TechFXEmitter, "ExplodingChains");

	UPopcornFXAttributeFunctions::SetAttributeAsBool(TechFXEmitter, Index, true);

	MedFXEmitter->StartEmitter();

	FTimerHandle TimerHandle;

	GetWorldTimerManager().SetTimer(TimerHandle, [&]() {
		TechFXEmitter->StopEmitter(true);
	}, 2, false);
}

void ANexus::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	if (!ActivableComp->IsActivated() && GameMode->CanStartTowerDefense()){
		ActivableComp->ActivateObject();
	}
}
