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
}

void ABasicDoor::BeginPlay(){
	Super::BeginPlay();

	bIsOpen = !MedDoor->GetRelativeRotation().Equals(FRotator::ZeroRotator);

	UpdateNavModiferParameters();

	RotationFactor = MedDoor->GetRelativeRotation().Equals(TargetOpenRotation) && bIsOpen ? 1 : -1;

	OpenDoorTimeline.SetPlaybackPosition(bIsOpen ? 1 : 0, false, true);
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
