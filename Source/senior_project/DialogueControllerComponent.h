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
	FString Speaker = "";
	FString Text = "";
	int Priority = 0;
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
	FString DIALOGUE_FILEPATH = "dialogue.csv";

	TMap<FString, int64> TagKeyMap; // initialized by ParseKeys. maps tags to start of line

	UFUNCTION(BlueprintCallable)
	void ParseKeys();

	UFUNCTION(BlueprintCallable)
	bool ParseTagRecord(FString Tag, FDialogueRecord& DialogueRecord); // position of start of line

	UFUNCTION(BlueprintCallable)
	bool ParseNextRecord(FDialogueRecord& DialogueRecord);

	// Sets default values for this component's properties
	UDialogueControllerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
