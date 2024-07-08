// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/ObjectMacros.h"

UENUM(BlueprintType)
enum class EItem : uint8 {
	ThrowUpgrade = 0,
	DodgeUpgrade = 1,
	CarryUpgrade = 2,
	KeyGate = 100,
	KeyElevator = 101
};

