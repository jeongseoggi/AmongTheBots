// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class AMONGTHEBOTS_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADoor();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_DoorVisible();

	void SetDoorState(bool bIsShow);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> SceneComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;

protected:

	UPROPERTY(ReplicatedUsing = OnRep_DoorVisible)
	uint8 bIsVisible : 1;
};
