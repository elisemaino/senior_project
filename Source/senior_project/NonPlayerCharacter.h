// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "DefaultCharacter.h"
#include "InteractionComponent.h"

#include "NonPlayerCharacter.generated.h"

UCLASS(Blueprintable, BlueprintType)
class SENIOR_PROJECT_API ANonPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANonPlayerCharacter();

protected:
	UPROPERTY(Blueprintable, BlueprintReadOnly)
	class ADefaultCharacter* TargetCharacter;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(Blueprintable, BlueprintReadOnly)
	class UInteractionComponent* Interaction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Hint = FText::FromString("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dialogue")
	FString DialogueTag = "";

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	UFUNCTION()
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// updates dialogue, based on TargetCharacter properties. override this wherever necessary
	UFUNCTION()
	virtual void UpdateDialogue();

	UFUNCTION(BlueprintCallable)
	virtual void FaceTargetCharacter();

	// display the next dialogue
	//virtual void Speak();
	// bound to the InteractionComponent's Interact event. updates TargetCharacter.
	UFUNCTION(BlueprintCallable)
	void Speak(ACharacter* Character);
};
