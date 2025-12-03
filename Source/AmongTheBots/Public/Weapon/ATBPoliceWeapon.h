// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ATBPoliceWeapon.generated.h"

class AWeaponActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AMONGTHEBOTS_API UATBPoliceWeapon : public UActorComponent
{
	GENERATED_BODY()

public:	
	UATBPoliceWeapon();

	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeapon)
	TObjectPtr<AWeaponActor> CurrentWeapon;

	void EquipWeapon(TSubclassOf<AWeaponActor> WeaponClass);
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_CurrentWeapon();
};
