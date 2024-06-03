// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"

#include "InteractionComponent.generated.h"

//UENUM(BlueprintType)
//enum EInteractionType {
//	InteractionCustom,
//	InteractionHold
//};

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SENIOR_PROJECT_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	//TEnumAsByte<EInteractionType> InteractionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FText Hint;

	// interact with the object. returns whether the interaction was successful
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void Interact(ACharacter* Character);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteract, ACharacter*, Character);
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FInteract OnInteract;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
};
