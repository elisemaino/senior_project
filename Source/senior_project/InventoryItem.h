// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/ObjectMacros.h"

#include "InventoryItem.generated.h"


UENUM(BlueprintType)
enum class EItem : uint8 {
	ThrowUpgrade = 0,
	DodgeUpgrade = 1,
	CarryUpgrade = 2,
	KeyGarageGate = 100,
	KeyElevator = 101,
	KeyRoofGate = 100,
	CreditChip = 200,
};

/* UEnum can be glitchy but this isn't worth refactoring at this point.
UCLASS(Blueprintable, BlueprintType)
class SENIOR_PROJECT_API UInventoryItem : public UObject
{
	GENERATED_BODY();
};

UCLASS(Blueprintable, BlueprintType)
class SENIOR_PROJECT_API UThrowUpgrade : public FInventoryItem {};

UCLASS(Blueprintable, BlueprintType)
class SENIOR_PROJECT_API UDodgeUpgrade : public FInventoryItem {};

UCLASS(Blueprintable, BlueprintType)
class SENIOR_PROJECT_API UCarryUpgrade : public FInventoryItem {};

UCLASS(Blueprintable, BlueprintType)
class SENIOR_PROJECT_API UKeyGate : public FInventoryItem {};

UCLASS(Blueprintable, BlueprintType)
class SENIOR_PROJECT_API UKeyElevator : public FInventoryItem {};

UCLASS(Blueprintable, BlueprintType)
class SENIOR_PROJECT_API UCreditChip : public FInventoryItem {};
*/
