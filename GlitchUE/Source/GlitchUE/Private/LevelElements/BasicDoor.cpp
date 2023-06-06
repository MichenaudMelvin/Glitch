// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelElements/BasicDoor.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/MainPlayer.h"
#include "NavAreas/NavArea_Obstacle.h"

ABasicDoor::ABasicDoor(){
	AIObstacle = CreateDefaultSubobject<UBoxComponent>(TEXT("AIObstacle"));
	AIObstacle->SetupAttachment(RootComponent);

	AIObstacle->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AIObstacle->SetCollisionResponseToAllChannels(ECR_Ignore);
	AIObstacle->SetGenerateOverlapEvents(false);
	AIObstacle->SetMobility(EComponentMobility::Static);

	AIObstacle->AreaClass = UNavArea_Obstacle::StaticClass();
	AIObstacle->bDynamicObstacle = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MedFrameStaticMesh(TEXT("/Game/Meshs/ModuilarKit/Door/SM_MED_Stone_DoorFrame"));
	check(MedFrameStaticMesh.Succeeded());

	MedFrame->SetStaticMesh(MedFrameStaticMesh.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> TechFrameStaticMesh(TEXT("/Game/Meshs/ModuilarKit/Door/SM_TECH_DoorFrame"));
	check(TechFrameStaticMesh.Succeeded());

	TechFrame->SetStaticMesh(TechFrameStaticMesh.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MedDoorStaticMesh(TEXT("/Game/Meshs/ModuilarKit/Door/SM_MED_WoodenDoor"));
	check(MedDoorStaticMesh.Succeeded());

	MedDoor->SetStaticMesh(MedDoorStaticMesh.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> TechDoorStaticMesh(TEXT("/Game/Meshs/ModuilarKit/Door/SM_TECH_Door"));
	check(TechDoorStaticMesh.Succeeded());

	TechDoor->SetStaticMesh(TechDoorStaticMesh.Object);

	UpDoorFrameMed = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Up Door Frame Med"));
	UpDoorFrameMed->SetupAttachment(MedFrame);
	UpDoorFrameMed->SetMobility(EComponentMobility::Static);

	UpDoorFrameTech = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Up Door Frame Tech"));
	UpDoorFrameTech->SetupAttachment(TechFrame);
	UpDoorFrameTech->SetMobility(EComponentMobility::Static);

#pragma region MeshAndMaterials

	static ConstructorHelpers::FObjectFinder<UStaticMesh> UpDoorMed1(TEXT("/Game/Meshs/ModuilarKit/Door/SM_Up_Door_Frame_1"));
	check(UpDoorMed1.Succeeded());

	UpDoorFrameMedMeshes.Add(UpDoorMed1.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> UpDoorMed2(TEXT("/Game/Meshs/ModuilarKit/Door/SM_Up_Door_Frame_2"));
	check(UpDoorMed2.Succeeded());

	UpDoorFrameMedMeshes.Add(UpDoorMed2.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> UpDoorMed3(TEXT("/Game/Meshs/ModuilarKit/Door/SM_Up_Door_Frame_3"));
	check(UpDoorMed3.Succeeded());

	UpDoorFrameMedMeshes.Add(UpDoorMed3.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> UpDoorMed4(TEXT("/Game/Meshs/ModuilarKit/Door/SM_Up_Door_Frame_4"));
	check(UpDoorMed4.Succeeded());

	UpDoorFrameMedMeshes.Add(UpDoorMed4.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> UpDoorMed5(TEXT("/Game/Meshs/ModuilarKit/Door/SM_Up_Door_Frame_5"));
	check(UpDoorMed5.Succeeded());

	UpDoorFrameMedMeshes.Add(UpDoorMed5.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> UpDoorMesh(TEXT("/Game/Meshs/ModuilarKit/Door/SM_Up_Door_Frame_Tech"));
	check(UpDoorMesh.Succeeded());

	UpDoorFrameTech->SetStaticMesh(UpDoorMesh.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> UpDoorTech1(TEXT("/Game/Meshs/Materials/ModularKit/Walls/MI_TECH_Wall_1"));
	check(UpDoorTech1.Succeeded());

	UpDoorFrameTechMaterial.Add(UpDoorTech1.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> UpDoorTech2(TEXT("/Game/Meshs/Materials/ModularKit/Walls/MI_TECH_Wall_2"));
	check(UpDoorTech2.Succeeded());

	UpDoorFrameTechMaterial.Add(UpDoorTech2.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> UpDoorTech3(TEXT("/Game/Meshs/Materials/ModularKit/Walls/MI_TECH_Wall_3"));
	check(UpDoorTech3.Succeeded());

	UpDoorFrameTechMaterial.Add(UpDoorTech3.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> UpDoorTech4(TEXT("/Game/Meshs/Materials/ModularKit/Walls/MI_TECH_Wall_4"));
	check(UpDoorTech4.Succeeded());

	UpDoorFrameTechMaterial.Add(UpDoorTech4.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> UpDoorTech5(TEXT("/Game/Meshs/Materials/ModularKit/Walls/MI_TECH_Wall_5"));
	check(UpDoorTech5.Succeeded());

	UpDoorFrameTechMaterial.Add(UpDoorTech5.Object);

#pragma endregion
}

void ABasicDoor::BeginPlay(){
	Super::BeginPlay();

	bIsOpen = !MedDoor->GetRelativeRotation().Equals(FRotator::ZeroRotator);

	UpdateNavModiferParameters();

	RotationFactor = MedDoor->GetRelativeRotation().Equals(TargetOpenRotation) && bIsOpen ? 1 : -1;

	OpenDoorTimeline.SetPlaybackPosition(bIsOpen ? 1 : 0, false, true);
}

void ABasicDoor::OnConstruction(const FTransform& Transform){
	Super::OnConstruction(Transform);

	UpDoorFrameMed->SetStaticMesh(UpDoorFrameMedMeshes[UpDoorFrameIndex]);
	UpDoorFrameTech->SetMaterial(0, UpDoorFrameTechMaterial[UpDoorFrameIndex]);
}

void ABasicDoor::InitializeDoor(const FBasicDoorData Data){
	if(Data.bIsOpen){
		bIsOpen = Data.bIsOpen;
		RotationFactor = Data.RotationFactor;

		OpenDoorTimeline.SetPlaybackPosition(bIsOpen ? 1 : 0, false, true);

		UpdateNavModiferParameters();
	}
}

FBasicDoorData ABasicDoor::SaveDoor() const{
	FBasicDoorData Data;

	Data.bIsOpen = bIsOpen;
	Data.RotationFactor = RotationFactor;

	return Data;
}

void ABasicDoor::OpenDoor(){
	SelectDoorRotationFactor();

	Super::OpenDoor();
}

void ABasicDoor::CloseDoor(){
	SelectDoorParameters();

	Super::CloseDoor();
}

void ABasicDoor::OpenDoorUpdate(float Alpha){
	Super::OpenDoorUpdate(Alpha);

	const FRotator TargetRotation = UKismetMathLibrary::RLerp(FRotator::ZeroRotator, TargetOpenRotation, RotationFactor * Alpha, false);

	MedDoor->SetRelativeRotation(TargetRotation);
	TechDoor->SetRelativeRotation(TargetRotation);
}

void ABasicDoor::OpenDoorEditor(){
	Super::OpenDoorEditor();

	const FRotator TargetRotation = MedDoor->GetRelativeRotation().Equals(TargetOpenRotation) ? TargetOpenRotation * -1 : TargetOpenRotation;

	MedDoor->SetRelativeRotation(TargetRotation);
	TechDoor->SetRelativeRotation(TargetRotation);
}

void ABasicDoor::CloseDoorEditor(){
	Super::CloseDoorEditor();

	MedDoor->SetRelativeRotation(FRotator::ZeroRotator);
	TechDoor->SetRelativeRotation(FRotator::ZeroRotator);
}

void ABasicDoor::SelectDoorRotationFactor(){
	const float DotResult = FVector::DotProduct(MedFrame->GetForwardVector(), SelectDoorParameters());

	RotationFactor = DotResult >= 0 ? -1 : 1;
}

FVector ABasicDoor::SelectDoorParameters(){
	float TargetTimelineDuration = OpenDoorTime;
	FVector TargetForwardVector;

	if(CurrentCharacterOpenningDoor->IsA(AMainPlayer::StaticClass())){
		const AMainPlayer* Player = Cast<AMainPlayer>(CurrentCharacterOpenningDoor);

		switch (Player->GetMovementMode()) {
			case EPlayerMovementMode::Normal:
				TargetTimelineDuration = OpenDoorTime;
				break;
			case EPlayerMovementMode::Sneaking:
				TargetTimelineDuration = OpenDoorTimeWhenPlayerSneak;
				break;
			case EPlayerMovementMode::Sprinting:
				TargetTimelineDuration = OpenDoorTimeWhenPlayerSprint;
				break;
		}

		TargetForwardVector = Player->GetFollowCamera()->GetForwardVector();
	}

	else{
		TargetForwardVector = CurrentCharacterOpenningDoor->GetCapsuleComponent()->GetForwardVector();
	}

	OpenDoorTimeline.SetPlayRate(1/TargetTimelineDuration);
	return TargetForwardVector;
}
