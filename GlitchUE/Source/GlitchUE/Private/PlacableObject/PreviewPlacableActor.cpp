// Fill out your copyright notice in the Description page of Project Settings.


#include "PlacableObject/PreviewPlacableActor.h"
#include "Kismet/KismetSystemLibrary.h"

APreviewPlacableActor::APreviewPlacableActor(){
	BaseMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	BaseMesh->CastShadow = false;

	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("/Game/VFX/Shaders/Hologram/M_HologramShader"));
	check(Material.Succeeded());

	HologramMaterial = Material.Object;

	BaseMesh->SetMaterial(0, HologramMaterial);
}

void APreviewPlacableActor::BeginPlay(){
	Super::BeginPlay();

	OriginalLocation = GetActorLocation();
}

bool APreviewPlacableActor::CheckSpotSpace() {
	FVector TraceLocation = GetActorLocation();
	TraceLocation.Z += 50;

	FVector HalfSize = FVector(49, 49, 40);

	TArray<AActor*> ActorsToIgnore;

	FHitResult Hit;

	return !UKismetSystemLibrary::BoxTraceSingle(GetWorld(), TraceLocation, TraceLocation, HalfSize, FRotator::ZeroRotator, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, Hit, true, FLinearColor::Red, FLinearColor::Green, 0.1f);
}

void APreviewPlacableActor::SetMesh() {
	if (!IsValid(CurrentData->FullMesh)) {
		Super::SetMesh();
		return;
	}

	BaseMesh->SetStaticMesh(CurrentData->FullMesh);

	for (int i = 0; i < BaseMesh->GetMaterials().Num(); i++) {
		BaseMesh->SetMaterial(i, HologramMaterial);
	}
}

void APreviewPlacableActor::SetInConstructionZone(bool bNewValue) {
	bInConstructionZone = bNewValue;
}

bool APreviewPlacableActor::CanBePlaced() {
	return CheckSpotSpace() && bInConstructionZone;
}

void APreviewPlacableActor::ResetActor() {
	SetActorLocation(OriginalLocation);
}