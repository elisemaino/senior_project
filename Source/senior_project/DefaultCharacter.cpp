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
	Movement->AirControl = 0.55;
	Movement->MaxAcceleration = 3300;
	Movement->BrakingDecelerationFalling = 1500;
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
	switch (CurrentAction) {
		case ActionHolding:
			TickHolding(DeltaTime);
			break;
		default:
			break;
	}
}

void ADefaultCharacter::TickHolding(float DeltaTime) {
	if (!HeldActor) return;
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "holding!");
	AStaticMeshActor* MeshActor = Cast<AStaticMeshActor>(HeldActor);
	if (!MeshActor) return;
	//UPrimitiveComponent* PrimitiveComponent = MeshActor->GetStaticMeshComponent();
	FRotator Rot = Controller->GetControlRotation();
	Rot.Pitch = FMath::Clamp(Rot.Pitch, -HoldMaxPitch, HoldMaxPitch);
	const FVector Dir = FRotationMatrix(Rot).GetScaledAxis(EAxis::X);
	FVector BBOrigin;
	FVector BBExtent;
	HeldActor->GetActorBounds(true, BBOrigin, BBExtent);
	FVector HoldLocation = GetActorLocation() + Dir * (BBExtent.Size() + HoldOffset);
	
	FVector Vel = HoldLocation - HeldActor->GetActorLocation();
	Vel = (Vel * HoldVelocityScale).GetClampedToMaxSize(HoldMaxVelocity);
	MeshActor->GetStaticMeshComponent()->SetPhysicsLinearVelocity(Vel);
	//MeshActor->GetStaticMeshComponent()->AddImpulse(Dir * HoldMaxVelocity);
}

void ADefaultCharacter::Hold(AActor* Actor) {
	if(CurrentAction != ActionDefault) return;

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "hold!");
	CurrentAction = ActionHolding;
	HeldActor = Actor;
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
	Rot += FRotator(Value, 0, 0);
	Controller->SetControlRotation(Rot);
}

void ADefaultCharacter::InputJump() {
	Jump();
}

void ADefaultCharacter::InputInteract() {
	if (CurrentAction != ActionDefault) return;

	FVector Start = GetActorLocation();
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	FVector End = Start + Direction * InteractMaxDistance;
	struct FHitResult OutHit;

	FCollisionQueryParams Params;
	Params.TraceTag = "Debug";
	GetWorld()->DebugDrawTraceTag = "Debug";

	bool Hit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_Visibility, Params);
	AActor* HitActor = OutHit.Actor.Get();

	UInteractionComponent* InteractionComponent = HitActor->FindComponentByClass<UInteractionComponent>();
	if (InteractionComponent) {
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "interactive");
		switch (InteractionComponent->InteractionType) {
			case InteractionHold:
				Hold(HitActor);
				break;
		}
	}
	/*
	//IInteractive* InteractiveActor = Cast<IInteractive>(HitActor);
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TCHAR("ptr: %u"), InteractiveActor));
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, HitActor->GetClass()->GetFName().ToString());
	bool InteractiveActor = HitActor->GetClass()->ImplementsInterface(UInterface::StaticClass());
	//bool InteractiveActor = HitActor->Implements<UInterface>();
	//bool InteractiveActor = UKismetSystemLibrary::DoesImplementInterface(HitActor, UInterface::StaticClass());
	if (InteractiveActor) {
		//IInteractive* InteractiveActor = Cast<IInteractive>(HitActor);
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "valid interactive actor!");
		//InteractionType T = InteractiveActor->Execute_Interact(this);
		InteractionType T = IInteractive::Execute_Interact(HitActor, this);
		switch (T) {
			case InteractionType::InteractionHold:
				Hold(HitActor);
				break;
			default:
				break;
		}
	}
	*/
}