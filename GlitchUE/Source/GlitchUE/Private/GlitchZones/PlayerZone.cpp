// Fill out your copyright notice in the Description page of Project Settings.


#include "GlitchZones/PlayerZone.h"
#include "Components/BillboardComponent.h"
#include "Gamemodes/GlitchUEGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "NavAreas/NavArea_Null.h"

APlayerZone::APlayerZone(){
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Engine/BasicShapes/Cube1"));
	check(Mesh.Succeeded());

	GetStaticMeshComponent()->SetStaticMesh(Mesh.Object);

	GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetStaticMeshComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetStaticMeshComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	GetStaticMeshComponent()->SetGenerateOverlapEvents(true);

	GetStaticMeshComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerZone::EnterZone);
	GetStaticMeshComponent()->OnComponentEndOverlap.AddDynamic(this, &APlayerZone::ExitZone);

	NavModifier = CreateDefaultSubobject<UNavModifierComponent>(TEXT("Nav Modifier"));
	NavModifier->AreaClass = UNavArea_Null::StaticClass();

#if WITH_EDITORONLY_DATA

	Billboard = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(RootComponent);
	Billboard->bIsEditorOnly = true;

#endif
}

void APlayerZone::BeginPlay(){
	Super::BeginPlay();

	GameMode = Cast<AGlitchUEGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void APlayerZone::OnPlayerEnterZone(){
	MainPlayer->SetInGlitchZone(true);
}

void APlayerZone::OnPlayerExitZone(){
	MainPlayer->SetInGlitchZone(false);

	GetWorld()->GetTimerManager().ClearTimer(ZoneTimer);
}

void APlayerZone::EnterZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if(OtherActor->IsA(AMainPlayer::StaticClass())){
		OnPlayerEnterZone();
	}
}

void APlayerZone::ExitZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
	if(OtherActor->IsA(AMainPlayer::StaticClass())){
		OnPlayerExitZone();
	}
}
