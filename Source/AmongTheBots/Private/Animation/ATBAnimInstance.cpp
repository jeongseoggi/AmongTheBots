// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ATBAnimInstance.h"
#include "Character/ATBPlayerCharacter.h"
#include "Character/ATBBotCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


void UATBAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerCharacter = Cast<AATBPlayerCharacter>(GetOwningActor());
	OwnerBotCharacter = Cast<AATBBotCharacter>(GetOwningActor());

	if (IsValid(OwnerCharacter))
	{
		OwnerCharacterMovementComp = OwnerCharacter->GetCharacterMovement();
	}
	else if(IsValid(OwnerBotCharacter))
	{
		OwnerCharacterMovementComp = OwnerBotCharacter->GetCharacterMovement();
	}
}

void UATBAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IsValid(OwnerCharacterMovementComp))
	{
		Velocity = OwnerCharacterMovementComp->Velocity;
		GroundSpeed = FVector(Velocity.X, Velocity.Y, 0).Size();

		if (APawn* Pawn = TryGetPawnOwner())
		{
			if (Pawn->IsPlayerControlled())
			{
				bShouldMove = ((OwnerCharacterMovementComp->GetCurrentAcceleration().IsNearlyZero()) == false && (3.f < GroundSpeed));
			}
			else
			{
				bShouldMove = (3.f < GroundSpeed);
			}
		}

		//bShouldMove = ((OwnerCharacterMovementComp->GetCurrentAcceleration().IsNearlyZero()) == false && (3.f < GroundSpeed));
		bIsFalling = OwnerCharacterMovementComp->IsFalling();
	}
}

void UATBAnimInstance::AnimNotify_MontageEnd()
{
	if (IsValid(OwnerCharacter))
	{
		OwnerCharacter->MeleeAttackEnd();
	}
}

