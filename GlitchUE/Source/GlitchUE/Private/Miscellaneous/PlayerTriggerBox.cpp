// Fill out your copyright notice in the Description page of Project Settings.


#include "Miscellaneous/PlayerTriggerBox.h"
#include "Components/ShapeComponent.h"


APlayerTriggerBox::APlayerTriggerBox(){
	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerTriggerBox::EnterTriggerBox);
	GetCollisionComponent()->OnComponentEndOverlap.AddDynamic(this, &APlayerTriggerBox::ExitTriggerBox);
}

void APlayerTriggerBox::EnterTriggerBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if(OtherActor->IsA(AMainPlayer::StaticClass())){
		OnPlayerEnter.Broadcast(Cast<AMainPlayer>(OtherActor));
	}
}

void APlayerTriggerBox::ExitTriggerBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
	if(OtherActor->IsA(AMainPlayer::StaticClass())){
		OnPlayerExit.Broadcast(Cast<AMainPlayer>(OtherActor));
	}
}
