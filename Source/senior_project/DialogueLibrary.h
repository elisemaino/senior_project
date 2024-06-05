// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"

#include "DialogueLibrary.generated.h"

USTRUCT(BlueprintType)
struct FDialogue {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<FText> Messages;
	UPROPERTY(BlueprintReadWrite)
	int Index = 0;
	UPROPERTY(BlueprintReadWrite)
	float StartTime = 0;
	UPROPERTY(BlueprintReadWrite)
	int Priority = 0;
	UPROPERTY(BlueprintReadWrite)
	float TextSpeed = 5;
};

/**
 * helper functions for dialogue
 */
UCLASS()
class SENIOR_PROJECT_API UDialogueLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	static bool AddDialogue(float Time, FDialogue& Dialogue, TArray<FText> NewMessages, int NewPriority);

	UFUNCTION(BlueprintCallable, Category="Dialogue")
	static FText GetText(float Time, FDialogue& Dialogue);
};
