// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/Nexus.h"
#include "Kismet/GameplayStatics.h"
#include "PopcornFXAttributeFunctions.h"
#include "Helpers/FunctionsLibrary/UsefulFunctions.h"
#include "Objectives/Catalyseur.h"
#include "Audio/AudioManager.h"
#include "FX/Dissolver.h"
#include "FMODBlueprintStatics.h"
#include "Player/MainPlayerController.h"

ANexus::ANexus() {
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> Idle(TEXT("/Game/Meshs/Objectives/Nexus/AS_Nexus"));
	check(Idle.Succeeded());

	IdleAnim = Idle.Object;

	NavModifier->SetBoxExtent(FVector(125, 125, 10));

	TechFXEmitter = CreateDefaultSubobject<UPopcornFXEmitterComponent>(TEXT("Tech FX"));
	TechFXEmitter->SetupAttachment(RootComponent);
	TechFXEmitter->SetRelativeLocation(FVector(0, 0, 200));

	static ConstructorHelpers::FObjectFinder<UPopcornFXEffect> TechFX(TEXT("/Game/VFX/Particles/FX_Environment/Pk_NexusFromTechToMed"));
	check(TechFX.Succeeded());

	TechFXEmitter->SetEffect(TechFX.Object);

	MedFXEmitter = CreateDefaultSubobject<UPopcornFXEmitterComponent>(TEXT("Medival FX"));
	MedFXEmitter->SetupAttachment(RootComponent);
	MedFXEmitter->bPlayOnLoad = false;
	MedFXEmitter->SetRelativeLocation(FVector(0, 0, 200));

	static ConstructorHelpers::FObjectFinder<UPopcornFXEffect> MedFX(TEXT("/Game/VFX/Particles/FX_Environment/Pk_NexusMED"));
	check(MedFX.Succeeded());;

	MedFXEmitter->SetEffect(MedFX.Object);
}

void ANexus::BeginPlay(){
	Super::BeginPlay();

	HealthComp->OnReciveDamages.RemoveDynamic(this, &ANexus::TakeDamages);
	HealthComp->OnHealthChange.AddDynamic(this, &ANexus::TakeDamages);

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

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&](){
		AMainPlayer* MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if(IsValid(MainPlayer)){
			HealthComp->OnHealthNull.AddDynamic(MainPlayer, &AMainPlayer::Loose);

			PlayerStatsWidget = MainPlayer->GetMainPlayerController()->GetPlayerStatsWidget();
			PlayerStatsWidget->UpdateNexusHealth(HealthComp->GetCurrentHealth());
		}
	}, 0.01f, false);
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

	if(IsValid(PlayerStatsWidget)){
		PlayerStatsWidget->UpdateNexusHealth(HealthComp->GetCurrentHealth());
	}
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

	TArray<AActor*> SortedCatalyseurs = UUsefulFunctions::SortActorsByDistanceToActor(ActivatedCatalyseurList, this);

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
}

void ANexus::Interact(AMainPlayerController* MainPlayerController, AMainPlayer* MainPlayer){
	if (!ActivableComp->IsActivated() && GameMode->CanStartTowerDefense()){
		ActivableComp->ActivateObject();
	}
}
