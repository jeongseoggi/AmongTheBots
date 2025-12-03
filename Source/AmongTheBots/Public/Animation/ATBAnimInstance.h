// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ATBAnimInstance.generated.h"

class AATBPlayerCharacter;
class UCharacterMovementComponent;
class AATBBotCharacter;

/**
 * 
 */
UCLASS()
class AMONGTHEBOTS_API UATBAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

	UPROPERTY()
	TObjectPtr<AATBPlayerCharacter> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<AATBBotCharacter> OwnerBotCharacter;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> OwnerCharacterMovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector Velocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bShouldMove : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bIsFalling : 1;

	UFUNCTION()
	void AnimNotify_MontageEnd();
};
