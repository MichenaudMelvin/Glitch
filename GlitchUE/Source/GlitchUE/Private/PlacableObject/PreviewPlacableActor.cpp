// Fill out your copyright notice in the Description page of Project Settings.


#include "PlacableObject/PreviewPlacableActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlacableObject/TrapData.h"
#include "Player/MainPlayer.h"

APreviewPlacableActor::APreviewPlacableActor(){
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	SetRootComponent(BaseMesh);

	AudioComp->SetupAttachment(RootComponent);

	BaseMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	BaseMesh->CastShadow = false;

	PlacableRange = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Placable Range"));
	PlacableRange->SetupAttachment(BaseMesh);
	PlacableRange->CastShadow = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> TurretMesh(TEXT("/Game/Meshs/System/SM_Turret_Range"));
	check(TurretMesh.Succeeded());

	TurretRangeMesh = TurretMesh.Object;
	PlacableRange->SetStaticMesh(TurretRangeMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> TrapMesh(TEXT("/Game/Meshs/System/SM_Trap_Range"));
	check(TrapMesh.Succeeded());

	TrapRangeMesh = TrapMesh.Object;

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

	SetRangeMesh(CurrentData);

	for (int i = 0; i < BaseMesh->GetMaterials().Num(); i++) {
		BaseMesh->SetMaterial(i, HologramMaterial);
	}
}

void APreviewPlacableActor::SetRangeMesh(const UPlacableActorData* DataToCheck) const{
	UStaticMesh* TargetMesh = DataToCheck->IsA(UTrapData::StaticClass()) ? TrapRangeMesh : TurretRangeMesh; 
	PlacableRange->SetStaticMesh(TargetMesh);

	const float TargetScale = DataToCheck->AttackRange/100;
	PlacableRange->SetWorldScale3D(FVector(TargetScale, TargetScale, 1));
}

void APreviewPlacableActor::SetData(UPlacableActorData* NewData){
	if(!IsValid(NewData)){
		BaseMesh->SetStaticMesh(nullptr);

		if(bShouldRangeUpdate){
			PlacableRange->SetStaticMesh(nullptr);
		}
		return;
	}

	CurrentData = NewData;
	SetMesh();
}

void APreviewPlacableActor::SetShouldRangeUpdate(const bool bNewValue){
	bShouldRangeUpdate = bNewValue;
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
