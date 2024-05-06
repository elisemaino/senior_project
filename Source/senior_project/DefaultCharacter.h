// Fill out your copyright notice in the Description page of Project Settings.

// Elise Maino 2024

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnrealMathUtility.h"

#include "InteractionComponent.h"

#include "DefaultCharacter.generated.h"

UENUM(BlueprintType)
enum EAction {
	ActionDefault,
	ActionHolding
};

UCLASS(Blueprintable, BlueprintType)
class SENIOR_PROJECT_API ADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADefaultCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(Blueprintable, BlueprintReadOnly)
	TEnumAsByte<EAction> CurrentAction = ActionDefault;
	UPROPERTY(Blueprintable, BlueprintReadOnly)
	AActor* HeldActor;
	float InteractMaxDistance = 1000; // maximum distance which a character can interact with an object
	float HoldOffset = 100; // offset added to bounding sphere radius when determining distance of held object from player
	float HoldVelocityScale = 15;
	float HoldMaxVelocity = 3000;
	float HoldMaxPitch = 30;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void TickHolding(float DeltaTime);

	virtual void Hold(AActor* Actor);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void InputMovementX(float Value);
	virtual void InputMovementY(float Value);
	virtual void InputCameraX(float Value);
	virtual void InputCameraY(float Value);
	virtual void InputJump();
	virtual void InputInteract();
};
