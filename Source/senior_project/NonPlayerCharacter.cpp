// Fill out your copyright notice in the Description page of Project Settings.


#include "NonPlayerCharacter.h"

// Sets default values
ANonPlayerCharacter::ANonPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Interaction = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction"));

	//Dialogue = TSharedPtr<TArray<FText>>(new TArray<FText>());
}

// Called when the game starts or when spawned
void ANonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Interaction->OnInteract.AddDynamic(this, &ANonPlayerCharacter::Speak);
}

// Called every frame
void ANonPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANonPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANonPlayerCharacter::UpdateDialogue() {
	
}

//void ANonPlayerCharacter::Speak() {
	
//}

void ANonPlayerCharacter::Speak(ACharacter* Character) {
	ADefaultCharacter* NewTargetCharacter = Cast<ADefaultCharacter>(Character);
	if (!NewTargetCharacter) return;
	TargetCharacter = NewTargetCharacter;
	UpdateDialogue();
	if (Dialogue.Num() <= 0) return;
	NewTargetCharacter->DialogueWidget->AddDialogue(&Dialogue, DialoguePriority);

		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, Dialogue[DialogueIndex].ToString());
	//Speak();
	//DialogueIndex = (DialogueIndex + 1) % Dialogue.Num();
}
