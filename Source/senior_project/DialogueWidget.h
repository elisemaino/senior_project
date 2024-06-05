// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"

#include "DialogueWidget.generated.h"

/**
 * widget which displays dialogue
 */
UCLASS(Blueprintable, BlueprintType)
class SENIOR_PROJECT_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	TArray<FText>* Dialogue;
	int DialogueIndex;
	float DialogueStartTime;
	int DialoguePriority;

	UCanvasPanel* CanvasPanel;
	UBorder* Border;
	UTextBlock* Text;

public:
	// letters per second
	UPROPERTY(BlueprintReadWrite)
	float TextSpeed = 5;

	virtual void NativeConstruct() override;

	// returns true if the dialogue is displayed
	virtual bool AddDialogue(TArray<FText>* NewDialogue, int NewPriority);

	UFUNCTION()
	virtual FText GetText();
};
