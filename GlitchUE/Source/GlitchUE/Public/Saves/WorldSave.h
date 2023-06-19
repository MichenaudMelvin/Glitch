// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Helpers/UsefulStructs.h"
#include "LevelElements/AbstractDoor.h"
#include "Saves/AbstractSave.h"
#include "WorldSave.generated.h"

UCLASS()
class GLITCHUE_API UWorldSave : public UAbstractSave{
	GENERATED_BODY()

public:
	UWorldSave();

	UPROPERTY(BlueprintReadWrite, Category = "World")
	FString WorldName;

	UPROPERTY(BlueprintReadWrite, Category = "Player")
	int PlayerGolds;

	UPROPERTY(BlueprintReadWrite, Category = "Player")
	FTransform PlayerTransform;

	UPROPERTY(BlueprintReadWrite, Category = "Player")
	FRotator PlayerCameraRotation;

	UPROPERTY(BlueprintReadWrite, Category = "Mark")
	FTransform MarkTransform;

	UPROPERTY(BlueprintReadWrite, Category = "Mark")
	bool bIsMarkPlaced;

	UPROPERTY(BlueprintReadWrite, Category = "World")
	float GlitchValue;

	UPROPERTY(BlueprintReadWrite, Category = "Door")
	TMap<FString, FBasicDoorData> DoorDataList;

	UPROPERTY(BlueprintReadWrite, Category = "Tchat")
	TArray<FString> TchatTriggerNameList;

	UPROPERTY(BlueprintReadWrite, Category = "Tchat")
	TArray<FTchatStruct> TchatLinesList;

	UPROPERTY(BlueprintReadWrite, Category = "BanLogs")
	TMap<FString, bool> BanLogList;
};
