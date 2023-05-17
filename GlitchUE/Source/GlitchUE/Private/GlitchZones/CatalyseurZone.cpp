// Fill out your copyright notice in the Description page of Project Settings.


#include "GlitchZones/CatalyseurZone.h"
#include "Components/BillboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavAreas/NavArea_Obstacle.h"
#include "Player/MainPlayer.h"

ACatalyseurZone::ACatalyseurZone(){
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/VFX/Particles/Meshes/SM_Demi_Sphere"));
	check(Mesh.Succeeded());

	GetStaticMeshComponent()->SetStaticMesh(Mesh.Object);

	GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetStaticMeshComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetStaticMeshComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	GetStaticMeshComponent()->SetGenerateOverlapEvents(true);

	GetStaticMeshComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACatalyseurZone::EnterZone);
	GetStaticMeshComponent()->OnComponentEndOverlap.AddDynamic(this, &ACatalyseurZone::ExitZone);

	#if WITH_EDITORONLY_DATA

		Billboard = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
		Billboard->SetupAttachment(RootComponent);
		Billboard->bIsEditorOnly = true;

	#endif

	NavModifierComp = CreateDefaultSubobject<UNavModifierComponent>(TEXT("NavModifier"));
	NavModifierComp->AreaClass = UNavArea_Obstacle::StaticClass();
}

void ACatalyseurZone::BeginPlay(){
	Super::BeginPlay();

	GameMode = Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->OnSwitchPhases.AddDynamic(this, &ACatalyseurZone::OnSwitchPhases);
}

void ACatalyseurZone::EnterZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if(OtherActor->IsA(AMainPlayer::StaticClass())){
		Cast<AMainPlayer>(OtherActor)->SetInGlitchZone(true);

		if(GameMode->GetLevelState() == ELevelState::Alerted){
			GetWorld()->GetTimerManager().SetTimer(CatalyeurZoneTimer, [&]() {
				GameMode->SetLevelState(ELevelState::Normal);
			}, ResetLevelStateDuration, false);
		}
	}
}

void ACatalyseurZone::ExitZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
	if(OtherActor->IsA(AMainPlayer::StaticClass())){
		Cast<AMainPlayer>(OtherActor)->SetInGlitchZone(false);

		GetWorld()->GetTimerManager().ClearTimer(CatalyeurZoneTimer);
	}
}

void ACatalyseurZone::OnSwitchPhases(EPhases NewPhase){
	switch (NewPhase){
		case EPhases::Infiltration:
			break;
		case EPhases::TowerDefense:
			Destroy();
			break;
	}
}
