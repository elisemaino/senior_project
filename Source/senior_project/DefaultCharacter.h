// Fill out your copyright notice in the Description page of Project Settings.

// Elise Maino 2024

#pragma once

#include <algorithm>
#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Components/CapsuleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Camera/CameraComponent.h"
#include "CollisionShape.h"
#include "AI/Navigation/NavigationTypes.h"

#include "InteractionComponent.h"
#include "InventoryItem.h"
#include "DialogueWidget.h"
#include "DialogueControllerComponent.h"

#include "DefaultCharacter.generated.h"

// default movement values
#define MOVEMENT_MASS 1000
#define MOVEMENT_AIR_CONTROL 0.1
#define MOVEMENT_MAX_ACCELERATION 3300
#define MOVEMENT_BRAKING_DECELERATION_FALLING 800
#define MOVEMENT_CROUCH_HEIGHT_SCALE 0.75

// min and max pitch of the camera
#define CAMERA_PITCH_MAX 89.0
#define CAMERA_PITCH_MIN -89.0
#define CAMERA_CROUCH_SPEED 150.0

// initial transform of camera
#define CAMERA_HEIGHT 76.0
#define CAMERA_TRANSFORM FTransform(FVector(0, 0, CAMERA_HEIGHT))

// params for when a held object "sticks" to a surface (to avoid weird physics glitches)
#define HOLD_STICK_TIME 0.05
#define HOLD_STICK_FACTOR 0.15

UENUM(BlueprintType)
enum class EAction : uint8 {
	Default,
	Holding,
	Frozen,
	Dash,
};

UCLASS(Blueprintable, BlueprintType)
class SENIOR_PROJECT_API ADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADefaultCharacter();


private:
	// NOah stuff
	float lastDodgeTime;
	// 
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UCameraComponent* Camera;

	UPROPERTY(Blueprintable, BlueprintReadOnly)
	class UInteractionComponent* InteractTraceHitComponent; // updated every frame. nullptr if no component was found.

	bool CrouchToggle = true;
	
	float InteractMaxDistance = 300; // maximum distance which a character can interact with an object
	float HoldMaxDistance = 275; // maximum distance from an object before forcibly letting go of it
	float HoldOffset = 75; // offset added to bounding sphere radius when determining distance of held object from player
	float HoldVelocityScale = 25;
	float HoldMaxVelocity = 1000; // maximum velocity of a held object. also the throw velocity
	float UpgradedHoldMaxVelocity = 2000;
	float HoldMaxPitch = 70;
	float HoldMinPitch = -45;
	float HoldLastHit = -999;

	float FreezeRotationSpeed = 5;


	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(Blueprintable, BlueprintReadOnly)
	EAction CurrentAction = EAction::Default;

	UPROPERTY(BlueprintReadOnly)
	class UDialogueControllerComponent* DialogueController;

	//UPROPERTY(Blueprintable, BlueprintReadOnly)
	//FText Hint = FText::FromString("");

	UPROPERTY(Blueprintable, BlueprintReadOnly)
	AActor* HeldActor;

	UPROPERTY(Blueprintable, BlueprintReadOnly)
	TArray<EItem> Items;

	UPROPERTY(Blueprintable, BlueprintReadOnly)
	UDialogueWidget* DialogueWidget;

	UPROPERTY(Blueprintable, BlueprintReadWrite)
	FRotator FreezeRotation;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void TickDefault(float DeltaTime);

	virtual void TickHolding(float DeltaTime);

	virtual void TickFrozen(float DeltaTime);

	// Hold an actor
	UFUNCTION(BlueprintCallable)
	virtual void Hold(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	virtual void EndHold();

	UFUNCTION()
	void HeldActorHit(AActor* ActorA, AActor* ActorB, FVector Location, const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable)
	virtual void Throw();

	UFUNCTION(BlueprintCallable)
	virtual void Freeze();

	UFUNCTION(BlueprintCallable)
	virtual void Unfreeze();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void InputMovementX(float Value);
	virtual void InputMovementY(float Value);
	virtual void InputCameraX(float Value);
	virtual void InputCameraY(float Value);
	virtual void InputJump();
	virtual void InputCrouchPress();
	virtual void InputCrouchRelease();
	virtual void InputInteract();
	virtual void InputAltInteract();
	
	// Noah stuff
	virtual void Dodge();
};
