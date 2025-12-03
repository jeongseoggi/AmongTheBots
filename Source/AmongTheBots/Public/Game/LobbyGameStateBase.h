// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGameStateBase.generated.h"


struct FPlayerInfo;
/**
 * 
 */



UCLASS()
class AMONGTHEBOTS_API ALobbyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerInfos)
	TArray<FPlayerInfo> PlayerInfos;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_PlayerInfos();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCSuccessTravel();

	void RemovePlayerInfos(int32 Index);

protected:

};
