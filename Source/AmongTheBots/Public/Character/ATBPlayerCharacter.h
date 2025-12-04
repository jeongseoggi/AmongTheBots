// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ATBPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;
class UATBPoliceWeapon;
class AWeaponActor;
class UAnimMontage;
class UStatusComponent;
class UMainHUD;
class AATBPlayerController;

UCLASS()
class AMONGTHEBOTS_API AATBPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AATBPlayerCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;


	UFUNCTION(BlueprintCallable)
	void SetCanAttack(bool bIsCanAttack);

	void MeleeAttackEnd();

	void ApplyAttack(AATBPlayerCharacter* InDamagedCharacter);

	void SetOverlappedEnemies(TArray<TObjectPtr<AActor>> OverlappedEnemies);

	void InitHUD(UMainHUD* HUDWidget);

	void DeadProcessing();

	UFUNCTION(Server, Reliable)
	void ServerRPCDeathProcessing(AATBPlayerController* MyController);

	void SetMoveSpeed();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetMoveSpeed();

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UCameraComponent> Camera;

	UFUNCTION(BlueprintCallable)
	void MoveAction(const FInputActionValue& MoveValue);

	UFUNCTION(BlueprintCallable)
	void LookAction(const FInputActionValue& LookValue);

	UFUNCTION(BlueprintCallable)
	void JumpAction(const FInputActionValue& JumpValue);

	UFUNCTION(BlueprintCallable)
	void AttackAction(const FInputActionValue& AttackValue);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> MeleeAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<UATBPoliceWeapon> PoliceWeaponComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AWeaponActor> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<UStatusComponent> StatusComponent;

	UPROPERTY(ReplicatedUsing = OnRep_CanAttack)
	bool bCanAttack;

	UFUNCTION()
	void OnRep_CanAttack();

	UFUNCTION(Server,Reliable)
	void ServerRPCMeleeAttack();

	UFUNCTION(Server, Reliable)
	void ServerRPCMeleeAttackEnd();

	UFUNCTION(Client, Reliable)
	void ClientRPCPlayMeleeAttackMontage(AATBPlayerCharacter* InTargetCharacter);

	UFUNCTION(Server,Reliable)
	void ServerRPCPerformMeleeHit(AATBPlayerCharacter* InDamagedCharacter, float Damage);


private:
	void DrawDebugMeleeAttack(const FColor& DrawColor, FVector TraceStart, FVector TraceEnd, FVector Forward);
	TArray<TObjectPtr<AActor>> OverlappedEnemiesArray;

	float WeaponDamage;
	float SelfTakeDamage;
};
