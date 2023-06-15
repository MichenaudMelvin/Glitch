// Fill out your copyright notice in the Description page of Project Settings.


#include "Mark/FakeMark.h"
#include "Mark/GlitchMark.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AFakeMark::AFakeMark(){
	MarkMesh->OnComponentHit.AddDynamic(this, &AFakeMark::OnCompHit);
	MarkMesh->SetHiddenInGame(true);

	MarkMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	MarkMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	MarkMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
}

void AFakeMark::OnConstruction(const FTransform& Transform){
	Super::OnConstruction(Transform);

	if(!IsValid(GlitchMark)){
		GlitchMark = Cast<AGlitchMark>(UGameplayStatics::GetActorOfClass(this, AGlitchMark::StaticClass()));
	}

	SetActorScale3D(GlitchMark->GetActorScale()/10);

	ProjectileMovement->InitialSpeed = GlitchMark->GetProjectileMovementComp()->InitialSpeed;
	ProjectileMovement->MaxSpeed = GlitchMark->GetProjectileMovementComp()->MaxSpeed;
}

void AFakeMark::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit){
	PlaceMark();
}

void AFakeMark::PlaceMark(){
	Super::PlaceMark();

	GlitchMark->PlaceMark();
	GlitchMark->SetActorLocation(GetActorLocation());
}

void AFakeMark::Launch(const FRotator StartRotation){
	Super::Launch(StartRotation);

	GlitchMark->Launch(UKismetMathLibrary::FindLookAtRotation(GlitchMark->GetActorLocation(), TargetPosition));
}

void AFakeMark::SetTargetPosition(const FVector NewTargetPosition){
	TargetPosition = NewTargetPosition;
}
