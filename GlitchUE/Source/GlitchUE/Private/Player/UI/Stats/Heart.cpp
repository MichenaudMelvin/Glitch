// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UI/Stats/Heart.h"

UHeart::UHeart(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){
	static ConstructorHelpers::FObjectFinder<UTexture2D> FullImage(TEXT("/Game/UI/Player/T_LifeHeart"));
	check(FullImage.Succeeded());

	FullHeart = FullImage.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D> EmptyImage(TEXT("/Game/UI/Player/T_LifeHalf"));
	check(EmptyImage.Succeeded());

	EmptyHeart = EmptyImage.Object;
}

void UHeart::UpdateDamages(const bool TakeDamages, const int DMG) const{
	SetImage(LeftImage, !TakeDamages);
	SetImage(RightImage, !TakeDamages);
}

void UHeart::SetImage(UImage* TargetImage, const bool bFullImage) const{
	FSlateBrush Brush;

	Brush.SetResourceObject(bFullImage ? FullHeart : EmptyHeart);

	TargetImage->SetBrush(Brush);
}
