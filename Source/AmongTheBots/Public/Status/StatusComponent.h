// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrentHPChanagedDelegate, float);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AMONGTHEBOTS_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatusComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	float ApplyDamage(float InDamage);

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurrentHP() const { return CurrentHP; }
	void SetCurrentHP(float InCurrentHP);


	FORCEINLINE float GetMaxHP() const { return MaxHP; }
	void SetMaxHP(float InMaxHP);

	UFUNCTION()
	void OnRep_CurrentHP();

	UFUNCTION()
	void BuffApply();

	uint8 bIsBuffApply : 1;

	FOnCurrentHPChanagedDelegate OnCurrentHPChanged;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHP)
	float CurrentHP;



	float MaxHP;


};
