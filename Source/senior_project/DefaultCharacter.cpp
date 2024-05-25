// Fill out your copyright notice in the Description page of Project Settings.


// Elise Maino 2024

#include "DefaultCharacter.h"

// Sets default values
ADefaultCharacter::ADefaultCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//CurrentAction = ActionDefault;

	UCharacterMovementComponent* Movement = Cast<UCharacterMovementComponent>(GetMovementComponent());
	//Movement->Mass = MOVEMENT_MASS;
	Movement->AirControl = MOVEMENT_AIR_CONTROL;
	Movement->MaxAcceleration = MOVEMENT_MAX_ACCELERATION;
	Movement->BrakingDecelerationFalling = MOVEMENT_BRAKING_DECELERATION_FALLING;

	//FAttachmentTransformRules CameraAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//FTransform CameraTransform;
	Camera->SetRelativeTransform(CAMERA_TRANSFORM, false);
	Camera->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
}

// Called when the game starts or when spawned
void ADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// camera
	Camera->SetWorldRotation(GetControlRotation(), false);

	// interact
	InteractTraceHitComponent = nullptr;

	// actions
	switch (CurrentAction) {
	case ActionHolding: {
		TickHolding(DeltaTime);
	} break;
	default: {
		TickDefault(DeltaTime);
	} break;
	}
}

void ADefaultCharacter::TickDefault(float DeltaTime) {
	FVector Start = Camera->GetComponentLocation();
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	FVector End = Start + Direction * InteractMaxDistance;
	FHitResult OutHit;

	//FCollisionQueryParams Params;
	//Params.TraceTag = "Debug";
	//GetWorld()->DebugDrawTraceTag = "Debug";
	//bool Hit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_Visibility, Params);
	bool Hit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_Visibility);
	if (Hit) {
		AActor* HitActor = OutHit.Actor.Get();
		if (HitActor) {
			InteractTraceHitComponent = HitActor->FindComponentByClass<UInteractionComponent>();
		} else {
			InteractTraceHitComponent = nullptr; // redundant for now
		}
	} else {
		InteractTraceHitComponent = nullptr; // redundant for now
	}
}

void ADefaultCharacter::TickHolding(float DeltaTime) {
	if (!HeldActor) return;
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "holding!");
	AStaticMeshActor* MeshActor = Cast<AStaticMeshActor>(HeldActor);
	if (!MeshActor) return;
	//UPrimitiveComponent* PrimitiveComponent = MeshActor->GetStaticMeshComponent();
	FRotator Rot = Controller->GetControlRotation();
	Rot.Pitch = FMath::Clamp(Rot.Pitch, HoldMinPitch, HoldMaxPitch);
	const FVector Dir = FRotationMatrix(Rot).GetScaledAxis(EAxis::X);
	FVector BBOrigin;
	FVector BBExtent;
	HeldActor->GetActorBounds(true, BBOrigin, BBExtent);
	FVector HoldLocation = Camera->GetComponentLocation() + Dir * (BBExtent.Size() + HoldOffset);

	/*
	FHitResult OutHit;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	CollisionQueryParams.AddIgnoredActor(HeldActor);
	bool hit = GetWorld()->SweepSingleByChannel(OutHit, BBOrigin, HoldLocation, HeldActor->GetActorQuat(), ECollisionChannel::ECC_PhysicsBody, FCollisionShape::MakeBox(BBExtent), CollisionQueryParams);
	*/

	//FHitResult HitResult;
	//MeshActor->GetStaticMeshComponent()->SetWorldLocation(HoldLocation, true, &HitResult);
	//HeldActor->SetActorLocation(HoldLocation, true, &HitResult);
	
	FVector Vel = HoldLocation - BBOrigin; // HeldActor->GetActorLocation();
	if (Vel.Size() > HoldMaxDistance) {
		EndHold();
		return;
	}
	Vel = (Vel * HoldVelocityScale).GetClampedToMaxSize(GetGameTimeSinceCreation() - HoldLastHit > HOLD_STICK_TIME ? HoldMaxVelocity : HoldMaxVelocity * HOLD_STICK_FACTOR);

	/*if (hit) {
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("%f"), (OutHit.Location - BBOrigin).Size()));
		Vel = Vel * std::max((OutHit.Location - BBOrigin).Size(), 0.01f);
	}*/

	MeshActor->GetStaticMeshComponent()->SetPhysicsLinearVelocity(Vel);
	//MeshActor->GetStaticMeshComponent()->AddImpulse(Dir * HoldMaxVelocity);
}

void ADefaultCharacter::Hold(AActor* Actor) {
	if (CurrentAction != ActionDefault) return;

	//FCollisionQueryParams CollisionQueryParams;
	//GetWorld()->DebugDrawTraceTag = "Debug";
	//CollisionQueryParams.TraceTag = "Debug";
	TArray<FOverlapResult> OutOverlaps;
	bool Hit = GetWorld()->OverlapMultiByChannel(OutOverlaps, GetActorLocation(), GetActorQuat(), ECollisionChannel::ECC_PhysicsBody, FCollisionShape::MakeCapsule(GetCapsuleComponent()->GetScaledCapsuleRadius() * 1.1, GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + InteractMaxDistance));
	if (Hit) {
		for (int i = 0; i < OutOverlaps.Num(); i++) {
			AActor* OtherActor = OutOverlaps[i].Actor.Get();
			if (!OtherActor) { continue; }
			if (OtherActor == Actor) {
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "initial overlap!");
				return;
			}
		}
	}

	if (HeldActor) {
		EndHold();
	}

	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "hold!");
	CurrentAction = ActionHolding;
	HeldActor = Actor;

	HeldActor->OnActorHit.AddDynamic(this, &ADefaultCharacter::HeldActorHit); // bind actor hit delegate
}

void ADefaultCharacter::EndHold() {
	if (!HeldActor) { return; }
	HeldActor->OnActorHit.RemoveAll(this); // unbind actor hit delegate
	HeldActor = nullptr;
	CurrentAction = ActionDefault;
}

void ADefaultCharacter::Throw() {
	if (!HeldActor) { return; }
	AStaticMeshActor* MeshActor = Cast<AStaticMeshActor>(HeldActor);
	if (!MeshActor) return;
	FRotator Rot = Controller->GetControlRotation();
	Rot.Pitch = FMath::Clamp(Rot.Pitch, HoldMinPitch, HoldMaxPitch);
	const FVector Dir = FRotationMatrix(Rot).GetScaledAxis(EAxis::X);
	MeshActor->GetStaticMeshComponent()->SetPhysicsLinearVelocity(Dir * HoldMaxVelocity);
	EndHold();
}


void ADefaultCharacter::HeldActorHit(AActor* ActorA, AActor* ActorB, FVector Location, const FHitResult& HitResult) {
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "held actor hit!");
	HoldLastHit = GetGameTimeSinceCreation();
	if (ActorB == this) {
		EndHold();
	}

}

// Called to bind functionality to input
void ADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// input bindings
	InputComponent->BindAxis("MovementX", this, &ADefaultCharacter::InputMovementX);
	InputComponent->BindAxis("MovementY", this, &ADefaultCharacter::InputMovementY);
	InputComponent->BindAxis("CameraX", this, &ADefaultCharacter::InputCameraX);
	InputComponent->BindAxis("CameraY", this, &ADefaultCharacter::InputCameraY);
	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ADefaultCharacter::Jump);
	InputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &ADefaultCharacter::InputInteract);
	InputComponent->BindAction("AltInteract", EInputEvent::IE_Pressed, this, &ADefaultCharacter::InputAltInteract);
}

void ADefaultCharacter::InputMovementX(float Value) {
	FRotator Rot = Controller->GetControlRotation();
	Rot.Pitch = 0.0;
	const FVector Dir = FRotationMatrix(Rot).GetScaledAxis(EAxis::X);
	AddMovementInput(Dir, Value);
}

void ADefaultCharacter::InputMovementY(float Value) {
	FRotator Rot = Controller->GetControlRotation();
	Rot.Pitch = 0.0;
	const FVector Dir = FRotationMatrix(Rot).GetScaledAxis(EAxis::Y);
	AddMovementInput(Dir, Value);
}

void ADefaultCharacter::InputCameraX(float Value) {
	FRotator Rot = Controller->GetControlRotation();
	Rot += FRotator(0, Value, 0);
	Controller->SetControlRotation(Rot);
}

void ADefaultCharacter::InputCameraY(float Value) {
	FRotator Rot = Controller->GetControlRotation();
	//Rot += FRotator(Value, 0, 0);
	Rot += FRotator(Rot.Pitch + Value > CAMERA_PITCH_MAX || Rot.Pitch + Value < CAMERA_PITCH_MIN ? 0 : Value, 0, 0);
	Controller->SetControlRotation(Rot);
}

void ADefaultCharacter::InputJump() {
	Jump();
}

void ADefaultCharacter::InputInteract() {
	switch (CurrentAction) {
	case ActionDefault: {
		if (InteractTraceHitComponent) {
			InteractTraceHitComponent->Interact(this);
		}
	} break;
	case ActionHolding: {
		EndHold();
	} break;
	}
}

void ADefaultCharacter::InputAltInteract() {
	switch (CurrentAction) {
	case ActionHolding: {
		Throw();
	} break;
	}
}