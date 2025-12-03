// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ATBBotCharacter.h"
#include "Controller/ATBAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

AATBBotCharacter::AATBBotCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AIControllerClass = AATBAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->bOrientRotationToMovement = true;
	Movement->RotationRate = FRotator(0.0f, 540.f, 0.0f);

}

// Called when the game starts or when spawned
void AATBBotCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


