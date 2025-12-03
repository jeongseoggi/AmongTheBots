// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "PlayerState/LobbyPlayerState.h"
#include "JobPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class AMONGTHEBOTS_API AJobPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerJob SpawnJob;
};
