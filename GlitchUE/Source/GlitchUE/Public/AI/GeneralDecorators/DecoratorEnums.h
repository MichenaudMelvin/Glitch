// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ECheckMethod : uint8 {
	Equal,
	NotEqual,
	GreaterThanOrEqualTo,
	LessOrEqual,
	GreaterThan,
	LessThan,
};