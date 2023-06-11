// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Gameplay/Tchat/TchatTriggerBox.h"
#include "Components/ShapeComponent.h"
#include "Player/MainPlayer.h"
#include "Player/MainPlayerController.h"

ATchatTriggerBox::ATchatTriggerBox(){
	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATchatTriggerBox::EnterTriggerBox);
}

void ATchatTriggerBox::BeginPlay(){
	Super::BeginPlay();

#if WITH_EDITOR
	if(TchatMessageList.Num() == 0){
		GEngine->AddOnScreenDebugMessage(-1, 9999999.0f, FColor::Yellow,FString::Printf(TEXT("LA MESSAGE LIST DE %s EST VIDE"), *GetName()));
		UE_LOG(LogTemp, Warning, TEXT("LA MESSAGE LIST DE %s EST VIDE"), *GetName());
	}
#endif
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

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATchatTriggerBox::WriteMessages, TchatMessageList[Index - 1].DelayForNextMessage, false);
}
