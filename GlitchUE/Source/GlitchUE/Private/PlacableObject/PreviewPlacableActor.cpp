// Fill out your copyright notice in the Description page of Project Settings.


#include "PlacableObject/PreviewPlacableActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/MainPlayer.h"

APreviewPlacableActor::APreviewPlacableActor(){
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	SetRootComponent(BaseMesh);

	BaseMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
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

void APreviewPlacableActor::SetPlayer(AMainPlayer* NewPlayer){
	MainPlayer = NewPlayer;
}

void APreviewPlacableActor::SetMesh() {
	if (!IsValid(CurrentData->FullMesh)) {
		Super::SetMesh();
		return;
	}

	Cast<UStaticMeshComponent>(BaseMesh)->SetStaticMesh(CurrentData->FullMesh);

	for (int i = 0; i < BaseMesh->GetMaterials().Num(); i++) {
		BaseMesh->SetMaterial(i, HologramMaterial);
	}
}

void APreviewPlacableActor::SetData(UPlacableActorData* NewData){
	CurrentData = NewData;
	SetMesh();
}

bool APreviewPlacableActor::CanBePlaced() const{
	return CurrentData->HasEnoughGolds(MainPlayer->GetGolds());
}

void APreviewPlacableActor::ChooseColor() const{
	const FVector TargetVectorColor = CanBePlaced() ? CanPlaceColor : CannotPlaceColor;

	BaseMesh->SetVectorParameterValueOnMaterials("Color", TargetVectorColor);

}

void APreviewPlacableActor::ResetActor() {
	SetActorLocation(OriginalLocation);
}

FVector APreviewPlacableActor::GetOriginalLocation() const{
	return OriginalLocation;
}

UStaticMeshComponent* APreviewPlacableActor::GetPreviewMesh() const{
	return BaseMesh;
}
