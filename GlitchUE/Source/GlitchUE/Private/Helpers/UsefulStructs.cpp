// Fill out your copyright notice in the Description page of Project Settings.


#include "Helpers/UsefulStructs.h"

FTchatStruct::FTchatStruct(){
	Speaker = "Speaker";
	TextMessage = "CoolMessage";
	SpeakerColor = FLinearColor::Yellow;
	DelayForNextMessage = 0.5;
}

FTchatStruct::FTchatStruct(FString NewSpeaker, FString NewTextMessage, FLinearColor NewSpeakerColor, float NewDelayForNextMessage){
	Speaker = NewSpeaker;
	TextMessage = NewTextMessage;
	SpeakerColor = NewSpeakerColor;
	DelayForNextMessage = NewDelayForNextMessage;
}
