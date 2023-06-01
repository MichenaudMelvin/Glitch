// Fill out your copyright notice in the Description page of Project Settings.

#include "Helpers/FunctionsLibrary/UsefullFunctions.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AISense_Hearing.h"
#include "Saves/AbstractSave.h"

void UUsefullFunctions::OutlineComponent(bool SetOutline, UPrimitiveComponent* Component){
	if (!IsValid(Component)){
		return;
	}

	Component->SetRenderCustomDepth(SetOutline);
	Component->SetCustomDepthStencilValue(SetOutline ? 2 : 0);
}

bool UUsefullFunctions::CanSee(AActor* SelfActor, FVector StartLocation, AActor* ActorToSee, ECollisionChannel CollisionChannel){
	TArray<AActor*> ActorsToIgnore;
	FHitResult Hit;

	ActorsToIgnore.Add(SelfActor);

	UKismetSystemLibrary::LineTraceSingle(ActorToSee->GetWorld(), StartLocation, ActorToSee->GetActorLocation(), UEngineTypes::ConvertToTraceType(CollisionChannel), false, ActorsToIgnore, EDrawDebugTrace::None, Hit, true, FLinearColor::Red, FLinearColor::Green, 0.1f);

	return Hit.GetActor() == ActorToSee;
}

int UUsefullFunctions::ClampIntToArrayLength(const int IntToClamp, const int ArrayLength){
	if(IntToClamp < 0 ){
		return ArrayLength - 1;
	}

	if(IntToClamp > (ArrayLength - 1)){
		return 0;
	}

	return IntToClamp;
}

int UUsefullFunctions::ClampZeroOne(const int IntToClamp){
	return FMath::Clamp(IntToClamp, 0, 1);
}

float UUsefullFunctions::ClampZeroOneFloat(const float FloatToClamp){
	return FMath::Clamp(FloatToClamp, 0.0f, 1.0f);
}

FVector UUsefullFunctions::AddToVector(FVector Vector, const float X, const float Y, const float Z){
	Vector.X += X;
	Vector.Y += Y;
	Vector.Z += Z;

	return Vector;
}

FRotator UUsefullFunctions::AddToRotator(FRotator Rotator, const float X, const float Y, const float Z){
	Rotator.Roll += X;
	Rotator.Pitch += Y;
	Rotator.Yaw += Z;

	return Rotator;
}

TArray<AActor*> UUsefullFunctions::SortActorsByDistanceToActor(TArray<AActor*> Actors, AActor* Target){
	QuickSortByDistance(Actors, 0, Actors.Num() - 1, Target);
	return Actors;
}

void UUsefullFunctions::QuickSortByDistance(TArray<AActor*>& InArray, const int Low, const int High, const AActor* Actor){
	if (InArray.Num() == 0 ) {
		UE_LOG(LogTemp, Warning, TEXT("Array null"));
		return;
	}

	int I = Low;
	int J = High;
	// Select a pivot
	const double Pivot = FVector::DistSquared(InArray[J]->GetActorLocation(), Actor->GetActorLocation());

	while (I <= J){

		while (FVector::DistSquared(InArray[I]->GetActorLocation(), Actor->GetActorLocation()) < Pivot){
			I++;
		}

		while (FVector::DistSquared(InArray[J]->GetActorLocation(), Actor->GetActorLocation()) > Pivot){
			J--;
		}

		if (I <= J){
			InArray.SwapMemory(I++, J--);
		}
	}

	if (J > Low){
		QuickSortByDistance(InArray, Low, J, Actor);
	}

	if (I < High){
		QuickSortByDistance(InArray, I, High, Actor);
	}
}

UAbstractSave* UUsefullFunctions::CreateSave(const TSubclassOf<UAbstractSave> SaveClass, const int UserIndex){
	UAbstractSave* NewSave = Cast<UAbstractSave>(UGameplayStatics::CreateSaveGameObject(SaveClass));
	NewSave->Index = UserIndex;
	NewSave = SaveToSlot(NewSave, UserIndex);
	return NewSave;
}

UAbstractSave* UUsefullFunctions::SaveToSlot(UAbstractSave* SaveObject, const int UserIndex){
	FString SlotName = SaveObject->GetSlotName();
	SlotName += FString::FromInt(UserIndex);

	UGameplayStatics::AsyncSaveGameToSlot(SaveObject, SlotName, 0);
	return SaveObject;
}

UAbstractSave* UUsefullFunctions::LoadSave(const TSubclassOf<UAbstractSave> SaveClass, const int UserIndex, const bool bCreateNewSaveIfDoesntExist){
	FString SlotName = SaveClass.GetDefaultObject()->GetSlotName();
	SlotName += FString::FromInt(UserIndex);

	UAbstractSave* LoadedSave = Cast<UAbstractSave>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));

	if(IsValid(LoadedSave)){
		LoadedSave->Index = UserIndex;
		return LoadedSave;
	}

	if(bCreateNewSaveIfDoesntExist){
		return CreateSave(SaveClass, UserIndex);
	}

	return nullptr;
}

bool UUsefullFunctions::DeleteSaveSlot(UAbstractSave* SaveObject, const int UserIndex){
	FString SlotName = SaveObject->GetSlotName();
	SlotName += FString::FromInt(UserIndex);

	return UGameplayStatics::DeleteGameInSlot(SlotName, 0);
}

float UUsefullFunctions::GetTimelineDuration(const UTimelineComponent* Timeline){
	return Timeline->GetTimelineLength() / Timeline->GetPlayRate();
}

bool UUsefullFunctions::IsCharacterMoving(const ACharacter* Character){
	return Character->GetVelocity().Size() > 0;
}

bool UUsefullFunctions::IsCharacterMovingOnGround(const ACharacter* Character){
	return IsCharacterMoving(Character) && Character->GetCharacterMovement()->IsMovingOnGround();
}

void UUsefullFunctions::MakeNoise(AActor* Instigator, const FVector NoiseLocation, const float SoundRadius){
	UAISense_Hearing::ReportNoiseEvent(Instigator->GetWorld(), NoiseLocation, 1, Instigator, SoundRadius, "Noise");
}

void UUsefullFunctions::Print(const FString StringToPrint, const bool bPrintToScreen, const bool bPrintToLog, const FLinearColor TextColor, const float Duration){
	if(bPrintToScreen){
		GEngine->AddOnScreenDebugMessage(-1, Duration, TextColor.ToFColor(false), FString::Printf(TEXT("%s"), *StringToPrint));
	}

	if(bPrintToLog){
		UE_LOG(LogTemp, Warning, TEXT("%s"), *StringToPrint);
	}
}

bool UUsefullFunctions::IsEventActionPressed(const FName ActionName, const APlayerController* PlayerController){
	TArray<FInputActionKeyMapping> Inputs;
	UInputSettings::GetInputSettings()->GetActionMappingByName(ActionName, Inputs);

	for(int i = 0; i < Inputs.Num(); i++){
		if(PlayerController->IsInputKeyDown(Inputs[i].Key)){
			return true;
		}

	}

	return false;
}

float UUsefullFunctions::GetAnimLength(UAnimSequenceBase* Anim){
	return Anim->GetPlayLength() * Anim->RateScale;
}
