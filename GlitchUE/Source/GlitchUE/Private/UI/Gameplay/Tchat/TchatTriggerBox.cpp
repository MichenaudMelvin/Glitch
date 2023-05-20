// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/Tchat/TchatTriggerBox.h"
#include "Components/ShapeComponent.h"
#include "Player/MainPlayer.h"
#include "Player/MainPlayerController.h"

ATchatTriggerBox::ATchatTriggerBox(){
	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATchatTriggerBox::EnterTriggerBox);
}

void ATchatTriggerBox::EnterTriggerBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if(OtherActor->IsA(AMainPlayer::StaticClass()) && Index == 0){
		CurrentController = Cast<AMainPlayer>(OtherActor)->GetMainPlayerController();
		WriteMessages();
	}
}

void ATchatTriggerBox::WriteMessages(){

	CurrentController->GetTchatWidget()->AddTchatLine(TchatMessageList[Index].Speaker, TchatMessageList[Index].TextMessage, TchatMessageList[Index].SpeakerColor);

	Index++;

	if(Index + 1 > TchatMessageList.Num()){
		Destroy();
		return;
	}

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATchatTriggerBox::WriteMessages, DelayBetweenEachMessage, false);
}
