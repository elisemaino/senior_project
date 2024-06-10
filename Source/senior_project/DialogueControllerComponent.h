// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "Misc/Paths.h"

#include <iostream>
#include <fstream>

#include "DialogueControllerComponent.generated.h"

#define MAX_LOOP_ITERATIONS 100000

USTRUCT(Blueprintable, BlueprintType)
struct FDialogueRecord {
	GENERATED_BODY()
	//int64 Key; // position of start of line
	//FString Tag = ""; // meta-name of the record for conversation starting points
	UPROPERTY(BlueprintReadOnly)
	FString Speaker = "";
	UPROPERTY(BlueprintReadOnly)
	FString Text = "";
	UPROPERTY(BlueprintReadOnly)
	int Priority = 0;
	UPROPERTY(BlueprintReadOnly)
	int64 NextKey = -1;
};

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SENIOR_PROJECT_API UDialogueControllerComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	std::ifstream Stream;

	FString ParseField();

	UFUNCTION()
	bool ParseRecord(int64 Key, FDialogueRecord& DialogueRecord);

public:	

	UPROPERTY(Blueprintable, BlueprintReadWrite)
	FString DIALOGUE_FILEPATH = "dialogue/dialogue.csv";

	TMap<FString, int64> TagKeyMap; // initialized by ParseKeys. maps tags to start of line

	// Sets default values for this component's properties
	UDialogueControllerComponent();

	UFUNCTION(BlueprintCallable)
	void ParseKeys();

	UFUNCTION(BlueprintCallable)
	bool ParseTagRecord(FString Tag, UPARAM(ref) FDialogueRecord& DialogueRecord); // position of start of line

	UFUNCTION(BlueprintCallable)
	bool ParseNextRecord(UPARAM(ref) FDialogueRecord& DialogueRecord);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNewDialogue, FString, Tag);
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FNewDialogue OnNewDialogue;

	UFUNCTION(BlueprintCallable)
	void NewDialogue(FString Tag);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
